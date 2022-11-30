#pragma once

#include <map>
#include <algorithm>
#include <cassert>
#include <vector>
#include <iterator>
#include <array>
#include <execution>
#include <shared_mutex>
#include <mutex>

#include "sorted_vector.h"
#include "range_contains.h"
#include "erase_remove_if.h"
#include "shared_lock_guard.h"

namespace utils::conway_simulation
{
	// CoordType: a type describing coordinates
	// UpdateCellFunc: a function with the signature: bool(const CoordType& coord, bool is_on, std::size_t number_of_on_neighbours)
	// GatherNeighboursFunc: a function with the signature std::vector<CoordType>(const CoordType&)
	//							Results are cached and assumed to not change tick-to-tick.
	template <typename CoordType, typename UpdateCellFunc, typename GatherNeighboursFunc>
	class state
	{
	public:
		using coord_type = CoordType;

		state(UpdateCellFunc update_func, GatherNeighboursFunc gather_neighbours_func)
			: m_on_cells{}
			, m_update_cell{ std::move(update_func) }
			, m_gather_neighbours{ std::move(gather_neighbours_func) }
		{}

		template <typename ItType>
		state(ItType init_start, ItType init_end, UpdateCellFunc update, GatherNeighboursFunc gather)
			: m_on_cells(init_start,init_end)
			, m_update_cell{ std::move(update) }
			, m_gather_neighbours{ std::move(gather) }
		{
			m_on_cells.unique();
		}

		[[nodiscard]] bool is_cell_on(const CoordType& cell) const noexcept { return m_on_cells.contains(cell); }
		[[nodiscard]] std::size_t number_of_cells_on() const { return m_on_cells.size(); }
		void tick();
		void tick_n_times(std::size_t num_ticks);
		template <typename ItType>
		void set_state(ItType first, ItType last)
		{
			m_on_cells = sorted_vector<CoordType>(first, last);
			m_on_cells.unique();
		}
	private:
		// Primary state
		sorted_vector<CoordType> m_on_cells;
		UpdateCellFunc m_update_cell;
		GatherNeighboursFunc m_gather_neighbours;

		// Spare stuff for optimisation
		mutable std::map<CoordType, std::vector<CoordType>> m_cached_neighbours;
		mutable std::shared_mutex m_cached_neighbours_lock;
		sorted_vector<CoordType> m_next_cells;
		sorted_vector<CoordType> m_relevant_cells;

		// Private functions

		const std::vector<CoordType>& get_neighbours(const CoordType& coords) const;
	};

	template <typename CoordType>
	auto make_range_update(
		const std::pair<std::size_t,std::size_t>& turn_on_range,
		const std::pair<std::size_t,std::size_t>& turn_off_range)
	{
		assert(turn_on_range.first <= turn_on_range.second);
		assert(turn_off_range.first <= turn_off_range.second);
		return [=](const CoordType&, bool is_on, std::size_t num_neighbours_on)
		{
			if (is_on)
			{
				return range_contains(num_neighbours_on, turn_off_range.first, turn_off_range.second);
			}
			// else
			return range_contains(num_neighbours_on, turn_on_range.first, turn_on_range.second);
		};
	}

	template <std::size_t DIMS>
	auto make_default_gather_func(int range)
	{
		assert(range > 0);
		using CoordType = std::array<int, DIMS>;
		return [range](const CoordType& cell)
		{
			std::vector<CoordType> result;
			result.reserve(static_cast<std::size_t>(std::pow(2 * range + 1, DIMS)) - 1);
			CoordType current_offset;
			std::fill(begin(current_offset), end(current_offset), 0 - range);
			auto is_current_all = [&current_offset](int val)
			{
				return std::all_of(begin(current_offset), end(current_offset), [val](int v) {return v == val; });
			};
			while (true)
			{
				if (!is_current_all(0))
				{
					result.push_back(current_offset);
				}

				// Increment.
				const auto inc_it = std::find_if(begin(current_offset), end(current_offset),
					[range](int val) {return val != range; });
				if (inc_it == end(current_offset)) // We reached the end.
				{
					break;
				}
				assert(*inc_it < range);
				++(*inc_it);
				std::fill(begin(current_offset), inc_it, 0 - range);
			}

			std::transform(begin(result), end(result), begin(result),
				[&cell](CoordType offset)
			{
				std::transform(begin(cell), end(cell), begin(offset), begin(offset), std::plus<int>{});
				return offset;
			});

			return result;
		};
	}

	template <std::size_t DIMS>
	auto make_default_gather_func(int range, const std::array<std::size_t, DIMS>& limits)
	{
		using CoordType = std::array<int, DIMS>;
		return [range, limits](const CoordType& cell)
		{
			const auto base_func = make_default_gather_func<DIMS>(range);
			auto base_result = base_func(cell);
			erase_remove_if(base_result,
				[&limits](const CoordType& c)
			{
				for (std::size_t i = 0; i < c.size(); ++i)
				{
					if (!range_contains(c[i], 0, static_cast<int>(limits[i]-1)))
					{
						return true;
					}
				}
				return false;
			});
			return base_result;
		};
	}

	template <std::size_t DIMS>
	auto make_default_gather_func(int range, std::size_t limit)
	{
		std::array<std::size_t, DIMS> limits;
		std::fill(begin(limits), end(limits), limit);
		return make_default_gather_func(range, limits);
	}

	template<typename CoordType, typename UpdateCellFunc, typename GatherNeighboursFunc>
	auto make_conway_state(UpdateCellFunc update, GatherNeighboursFunc gather)
	{
		return state<CoordType, UpdateCellFunc, GatherNeighboursFunc>(std::move(update), std::move(gather));
	}

	template<typename CoordType, typename ItType, typename UpdateCellFunc, typename GatherNeighboursFunc>
	auto make_conway_state(ItType first, ItType last, UpdateCellFunc update, GatherNeighboursFunc gather)
	{
		return state<CoordType, UpdateCellFunc, GatherNeighboursFunc>(first,last,std::move(update), std::move(gather));
	}

	template<typename CoordType, typename UpdateCellFunc, typename GatherNeighboursFunc>
	inline void state<CoordType, UpdateCellFunc, GatherNeighboursFunc>::tick_n_times(std::size_t num_ticks)
	{
		for (std::size_t i = 0; i < num_ticks; ++i)
		{
			tick();
		}
	}

	template<typename CoordType, typename UpdateCellFunc, typename GatherNeighboursFunc>
	inline void conway_simulation::state<CoordType, UpdateCellFunc, GatherNeighboursFunc>::tick()
	{
		// Clear the cached containers.
		m_next_cells.clear();
		m_relevant_cells.clear();

		// Gather all relevant cells
		std::mutex relevant_cells_lock;
		auto gather_relevant = [this,&relevant_cells_lock](const CoordType& on_cell)
		{
			const auto& neighbours = get_neighbours(on_cell);
			std::lock_guard<std::mutex> relevant_cells_guard{ relevant_cells_lock };
			m_relevant_cells.reserve((neighbours.size() + 1) * m_on_cells.size());
			m_relevant_cells.push_back(on_cell);
			std::copy(begin(neighbours), end(neighbours), std::back_inserter(m_relevant_cells));
		};
		std::for_each(std::execution::unseq, begin(m_on_cells), end(m_on_cells), gather_relevant);

		m_relevant_cells.unique();

		std::copy_if(begin(m_relevant_cells), end(m_relevant_cells), std::back_inserter(m_next_cells),
			[this](const CoordType& cell)
		{
			const auto& neighbours = get_neighbours(cell);
			const std::size_t num_neighbours_on = std::count_if(begin(neighbours), end(neighbours),
				[this](const CoordType& neighbour)
			{
				return is_cell_on(neighbour);
			});
			return m_update_cell(cell, is_cell_on(cell), num_neighbours_on);
		});

		m_on_cells.swap(m_next_cells);
	}

	template<typename CoordType, typename UpdateCellFunc, typename GatherNeighboursFunc>
	inline const std::vector<CoordType>& conway_simulation::state<CoordType, UpdateCellFunc, GatherNeighboursFunc>::get_neighbours(const CoordType& coords) const
	{
		{
			utils::shared_lock_guard guard{ m_cached_neighbours_lock };
			const auto find_result = m_cached_neighbours.lower_bound(coords);

			// If we have a chached result, return that.
			if (find_result != end(m_cached_neighbours) && find_result->first == coords)
			{
				return find_result->second;
			}
		}

		// Otherwise gather neighbours and cache the result.
		auto neighbours = std::make_pair(coords, m_gather_neighbours(coords));
		std::lock_guard guard{ m_cached_neighbours_lock };
		const auto insert_it = m_cached_neighbours.insert(std::move(neighbours));
		assert(insert_it.second);
		return insert_it.first->second;
	}
}