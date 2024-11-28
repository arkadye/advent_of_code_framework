#pragma once

#include <iosfwd>
#include <type_traits>
#include <optional>
#include <algorithm>
#include <concepts>

#include "advent/advent_assert.h"
#include "istream_line_iterator.h"
#include "coords.h"
#include "coords_iterators.h"
#include "int_range.h"
#include "small_vector.h"
#include "range_contains.h"

#define AOC_GRID_DEBUG_DEFAULT 0
#if NDEBUG
#define AOC_GRID_DEBUG 0
#else
#define AOC_GRID_DEBUG AOC_GRID_DEBUG_DEFAULT
#endif

#if AOC_GRID_DEBUG
#include <iostream>
#endif

namespace utils
{
	template <typename NodeType>
	class grid
	{
		utils::small_vector<NodeType,1> m_nodes;
		utils::coords m_max_point;
		std::size_t get_idx(std::integral auto x, std::integral auto y) const;
	public:
		using value_type = NodeType;
		using reference = NodeType&;
		using const_reference = const NodeType&;
		auto operator==(const grid& other) const noexcept requires std::equality_comparable<NodeType>
		{
			return m_max_point == other.m_max_point && stdr::equal(m_nodes, other.m_nodes);
		}
		bool is_on_grid(std::integral auto x, std::integral auto y) const;
		template <std::integral T>
		bool is_on_grid(utils::basic_coords<T> coords) const { return is_on_grid(coords.x,coords.y); }
		utils::coords get_max_point() const noexcept { return m_max_point; }
		utils::coords bottom_left() const noexcept { return utils::coords{ 0,0 }; }
		utils::coords top_left() const noexcept { return utils::coords{ 0, m_max_point.y - 1 }; }
		utils::coords bottom_right() const noexcept { return utils::coords{ m_max_point.x - 1 , 0 }; }
		utils::coords top_right() const noexcept { return m_max_point - utils::coords{ 1,1 }; }
		void resize(const utils::coords& new_max, const NodeType& fill_value)
		{
			m_max_point = new_max;
			m_nodes.resize(new_max.x * new_max.y, fill_value);
		}
		void resize(int new_x, int new_y, const NodeType& fill_value)
		{
			resize(utils::coords{ new_x,new_y });
		}

		std::size_t size() const noexcept {	return m_nodes.size(); }

		NodeType& at(std::integral auto x, std::integral auto y) { return m_nodes[get_idx(x,y)]; }
		const NodeType& at(std::integral auto x, std::integral auto y) const { return m_nodes[get_idx(x,y)]; }
		template <std::integral T>
		NodeType& at(utils::basic_coords<T> coords) { return at(coords.x,coords.y); }
		template <std::integral T>
		const NodeType& at(utils::basic_coords<T> coords) const { return at(coords.x,coords.y); }

		// Get all nodes that meet a predicate
		utils::small_vector<utils::coords,1> get_all_coordinates_by_predicate(const auto& predicate) const
		{
			using utils::coords;
			utils::small_vector<coords, 1> result;
			auto projection = [this](const coords& c) -> const NodeType& { return at(c); };
			stdr::copy_if(utils::coords_iterators::elem_range{m_max_point}, std::back_inserter(result), predicate, projection);
			return result;
		}

		// Get a node using a predicate
		std::optional<utils::coords> get_coordinates_by_predicate(const auto& predicate) const
		{

			const utils::coords_iterators::elem_range range{m_max_point};
			const auto projection = [this](const coords& c) -> const NodeType& { return at(c); };
			const auto result = stdr::find_if(range, predicate, projection);
			if (result != end(range)) return *result;
			return std::nullopt;
		}

		// Get a node using NodeType::operator==
		std::optional<utils::coords> get_coordinates(const NodeType& node) const
		{
			return get_coordinates_by_predicate([&node](const NodeType& other)
			{
				return node == other;
			});
		}

		template <typename Convert>
		void stream_row(std::ostream& oss, int row_idx, const Convert& convert) const;
		void stream_row(std::ostream& oss, int row_idx) const
		{
			auto impl = [](std::ostream& oss, const NodeType& node)
				{
					oss << node;
				};
			stream_row(oss, row_idx, impl);
		}

		template <typename Convert>
		void stream_column(std::ostream& oss, int column_idx, const Convert& convert) const;
		void stream_column(std::ostream& oss, int column_idx) const
		{
			auto impl = [](std::ostream& oss, const NodeType& node)
				{
					oss << node;
				};
			stream_column(oss, column_idx, impl);
		}

		template <typename Convert>
		void stream_grid(std::ostream& oss, const Convert& convert) const;
		void stream_grid(std::ostream& oss) const
		{
			auto impl = [](std::ostream& oss, const NodeType& node)
				{
					oss << node;
				};
			stream_grid(oss, impl);
		}

		void build_from_stream(std::istream& iss, const auto& char_to_node_fn)
		{
			for (auto line : utils::istream_line_range{ iss })
			{
				if(m_max_point.x == 0)
				{
					m_max_point.x = static_cast<int>(line.size());
					m_nodes.reserve(m_max_point.x * m_max_point.x);
				}
				AdventCheck(m_max_point.x == static_cast<int>(line.size()));
				std::transform(begin(line),end(line),std::back_inserter(m_nodes),char_to_node_fn);
				++m_max_point.y;
			}
#if AOC_GRID_DEBUG
			std::cout << "Created grid with dimensions [" << m_max_point << '\n';
#endif
		}

		utils::small_vector<utils::coords,1> get_path(const utils::coords& start, const auto& is_end_fn,
			const auto& traverse_cost_fn,
			const auto& heuristic_fn) const;

		utils::small_vector<utils::coords,1> get_path(const utils::coords& start, const auto& is_end_fn,
			const auto& cost_or_heuristic_fn) const;

		utils::small_vector<utils::coords,1> get_path(const utils::coords& start, const auto& is_end_fn) const;

		utils::small_vector<utils::coords,1> get_path(const utils::coords& start, const utils::coords& end,
			const auto& traverse_cost_fn,
			const auto& heuristic_fn) const;

		utils::small_vector<utils::coords,1> get_path(const utils::coords& start, const utils::coords& end,
			const auto& cost_or_heuristic_fn) const;

		utils::small_vector<utils::coords,1> get_path(const utils::coords& start, const utils::coords& end) const;
	};

	namespace grid_helpers
	{
		template <typename NodeType, typename FnType>
		constexpr bool is_end_fn()
		{
			return std::is_invocable_r_v<bool, FnType, utils::coords, NodeType>;
		}

		template <typename NodeType, typename FnType>
		constexpr bool is_cost_fn()
		{
			return std::is_invocable_r_v < std::optional<float>,
				FnType,
				utils::coords, NodeType,
				utils::coords, NodeType>;
		}

		template <typename NodeType, typename FnType>
		constexpr bool is_heuristic_fn()
		{
			return std::is_invocable_r_v<float, FnType, utils::coords, NodeType>;
		}

		auto build(std::istream& iss, const auto& char_to_node_fn)
		{
			using NodeType = decltype(char_to_node_fn(' '));
			grid<NodeType> result;
			result.build_from_stream(iss, char_to_node_fn);
			return result;
		}

		template <typename NodeType>
		struct DefaultHeuristicFunctor
		{
		private:
			std::optional<utils::coords> target;
		public:
			DefaultHeuristicFunctor() {}
			explicit DefaultHeuristicFunctor(utils::coords target_init) : target{target_init}{}
			float operator()(utils::coords coords, const NodeType& node) const
			{
				return static_cast<float>(target.has_value() ? coords.manhatten_distance(target.value()) : 0);
			}
		};

		template <typename NodeType, bool allow_diagonal>
		struct DefaultCostFunctor
		{
		public:
			std::optional<float> operator()(utils::coords from, const NodeType& from_node, utils::coords to, const NodeType& to_node) const
			{
				const utils::coords diff = to - from;
				const int distance = diff.manhatten_distance();
				if (distance == 0)
				{
					return std::nullopt;
				}
				if constexpr (allow_diagonal)
				{
					auto square = [](int val)
					{
						const int result = val * val;
						return static_cast<float>(result);
					};
					const bool allowed = std::max(std::abs(diff.x),std::abs(diff.y)) <= 1;
					if (allowed)
					{
						return std::sqrt(square(diff.x) + square(diff.y));
					}
				}
				if constexpr (!allow_diagonal)
				{
					const bool allowed = distance <= 1;
					if (allowed)
					{
						return static_cast<float>(distance);
					}
				}
				return std::nullopt;
			}
		};

		template <typename T>
		concept grid_type = requires { std::is_same_v<T,grid<typename T::value_type>> || std::is_same_v<T,const grid<typename T::value_type>>; };

		namespace internal_helpers
		{
			template <grid_type T>
			struct node_ref_type {};

			template <typename T>
			struct node_ref_type<const grid<T>>
			{
				using type = typename grid<T>::const_reference;
			};

			template <typename T>
			struct node_ref_type<grid<T>>
			{
				using type = typename grid<T>::reference;
			};

			template <grid_type T>
			using node_ref_type_t = typename node_ref_type<T>::type;

			using underlying_range_t = utils::coords_iterators::elem_range<int>;
			template <grid_type GridType>
			class elem_adaptor
			{
				GridType& underlying_grid;
				underlying_range_t underlying_range;
			public:
				elem_adaptor(GridType& in_grid, const underlying_range_t& in_range) noexcept
					: underlying_grid{in_grid}, underlying_range{in_range}{}
				node_ref_type_t<GridType> operator()(std::size_t idx) const noexcept
				{
					const auto coords = underlying_range[idx];
					return underlying_grid.at(coords);
				}
			};

			template <grid_type T>
			using elem_view_base = int_range_adaptor<elem_adaptor<T>>;
		}

		template <grid_type T>
		class elem_view : public internal_helpers::elem_view_base<T>
		{
		public:
			elem_view(T& in_grid, const internal_helpers::underlying_range_t& range)
				: internal_helpers::elem_view_base<T>{ internal_helpers::elem_adaptor{in_grid , range} , std::ssize(range) }{}
			elem_view(T& in_grid, const utils::coords& first, const utils::coords& last)
				: elem_view(in_grid, internal_helpers::underlying_range_t{ first,last }) {}
			elem_view(T& in_grid, const utils::coords& last)
				: elem_view(in_grid, utils::coords{ 0,0 }, last) {}
			explicit elem_view(T& in_grid) : elem_view(in_grid, in_grid.get_max_point()) {}
		};

		template <grid_type T>
		inline elem_view<T> get_row_elem_view(T& in_grid, int x_min, int x_max, int y_const) noexcept
		{
			return elem_view<T>{in_grid, internal_helpers::underlying_range_t::get_row(x_min, x_max, y_const) };
		}

		template <grid_type T>
		inline elem_view<T> get_row_elem_view(T& in_grid, int x_max, int y_const) noexcept
		{
			return get_row_elem_view(in_grid, 0, x_max, y_const);
		}

		template <grid_type T>
		inline elem_view<T> get_row_elem_view(T& in_grid,int y_const) noexcept
		{
			return get_row_elem_view(in_grid, in_grid.get_max_point().x, y_const);
		}

		template <grid_type T>
		inline elem_view<T> get_column_elem_view(T& in_grid, int x_const, int y_min, int y_max) noexcept
		{
			return elem_view<T>{in_grid, internal_helpers::underlying_range_t::get_column(x_const, y_min, y_max) };
		}

		template <grid_type T>
		inline elem_view<T> get_column_elem_view(T& in_grid, int x_const, int y_max) noexcept
		{
			return get_column_elem_view(in_grid,x_const,0,y_max);
		}

		template <grid_type T>
		inline elem_view<T> get_column_elem_view(T& in_grid, int x_const) noexcept
		{
			return get_column_elem_view(in_grid, x_const, in_grid.get_max_point().y);
		}

		template <grid_type LeftGrid, grid_type RightGrid> requires std::three_way_comparable_with<typename LeftGrid::value_type, typename RightGrid::value_type>
		auto operator<=>(const elem_view<LeftGrid>& left, const elem_view<RightGrid>& right) noexcept
		{
			return stdr::lexicographical_compare(left, right);
		}

		template <grid_type LeftGrid, grid_type RightGrid> requires std::equality_comparable_with<typename LeftGrid::value_type, typename RightGrid::value_type>
		bool operator==(const elem_view<LeftGrid>& left, const elem_view<RightGrid>& right) noexcept
		{
			return stdr::equal(left, right);
		}

		namespace internal_helpers
		{
			template <grid_type T>
			class row_view_adaptor
			{
				T& underlying_grid;
				utils::coords start, finish;
			public:
				row_view_adaptor(T& in_grid, const utils::coords& first, const utils::coords& last)
					: underlying_grid{ in_grid }, start{ first }, finish{ last }{}
				elem_view<T> operator()(std::size_t idx) const noexcept
				{
					AdventCheck(idx < coords_iterators::internal_helpers::height(start,finish));
					const auto y = coords_iterators::internal_helpers::get_at_idx(start.y,finish.y,idx);
					return get_row_elem_view(underlying_grid,start.x,finish.x,y);
				}
			};

			template <grid_type T>
			class column_view_adaptor
			{
				T& underlying_grid;
				utils::coords start, finish;
			public:
				column_view_adaptor(T& in_grid, const utils::coords& first, const utils::coords& last)
					: underlying_grid{in_grid},start{first},finish{last}{}
				elem_view<T> operator()(std::size_t idx) const noexcept
				{
					AdventCheck(idx < coords_iterators::internal_helpers::width(start, finish));
					const auto x = coords_iterators::internal_helpers::get_at_idx(start.x, finish.x, idx);
					return get_column_elem_view(underlying_grid, x, start.y,finish.y);
				}
			};

			template <grid_type T>
			using row_view_base = int_range_adaptor<row_view_adaptor<T>>;

			template <grid_type T>
			using column_view_base = int_range_adaptor<column_view_adaptor<T>>;
		}

		template <grid_type T>
		class row_view : public internal_helpers::row_view_base<T>
		{	
		public:
			row_view(T& in_grid, const utils::coords& first, const utils::coords& last) noexcept
				: internal_helpers::row_view_base<T>{ internal_helpers::row_view_adaptor{ in_grid,first,last } , coords_iterators::internal_helpers::sheight(first,last) } {}
			row_view(T& in_grid, const utils::coords& last) noexcept
				: row_view{in_grid , utils::coords{0,0} , last}{}
			explicit row_view(T& in_grid)
				: row_view{ in_grid , in_grid.get_max_point() } {}
		};

		template <grid_type T, typename ConvertType> requires std::invocable<ConvertType,std::ostream&,typename T::value_type>
		void stream_view(std::ostream& oss, elem_view<T> view, const ConvertType& conversion)
		{
			for (const auto& elem : view)
			{
				conversion(oss, elem);
			}
		}

		template <grid_type T, typename ConvertType> requires std::invocable<ConvertType, typename T::value_type>
		void stream_view(std::ostream& oss, elem_view<T> view, const ConvertType& conversion)
		{
			auto impl = [&conversion](std::ostream& oss, const T::value_type& elem)
				{
					oss << conversion(elem);
				};
			stream_view(oss, view, impl);
		}

		template <grid_type T>
		void stream_view(std::ostream& oss, elem_view<T> view)
		{
			auto impl = [](std::ostream& oss, const T::value_type& elem)
				{
					oss << elem;
				};
			stream_view(oss, view, impl);
		}

		template <grid_type T>
		std::ostream& operator<<(std::ostream& oss, elem_view<T> view)
		{
			stream_view(oss, view);
			return oss;
		}

		template <grid_type T>
		class column_view : public internal_helpers::column_view_base<T>
		{
		public:
			column_view(T& in_grid, const utils::coords& first, const utils::coords& last) noexcept
				: internal_helpers::column_view_base<T>{ internal_helpers::column_view_adaptor{ in_grid,first,last } , coords_iterators::internal_helpers::swidth(first,last) } {}
			column_view(T& in_grid, const utils::coords& last) noexcept
				: column_view{ in_grid , utils::coords{0,0} , last } {}
			explicit column_view(T& in_grid)
				: column_view{ in_grid , in_grid.get_max_point() } {}
		};
	}

	template <typename NodeType>
	inline std::ostream& operator<<(std::ostream& oss, const utils::grid<NodeType>& grid)
	{
		grid.stream_grid(oss);
		return oss;
	}
}

template <typename NodeType>
inline bool utils::grid<NodeType>::is_on_grid(std::integral auto x, std::integral auto y) const
{
	if(x < 0) return false;
	if(y < 0) return false;
	if(x >= m_max_point.x) return false;
	if(y >= m_max_point.y) return false;
	return true;
}

template <typename NodeType>
inline std::size_t utils::grid<NodeType>::get_idx(std::integral auto x, std::integral auto y) const
{
	AdventCheck(is_on_grid(x,y));
	const auto inverted_y = m_max_point.y - y - 1;
	const auto result = m_max_point.x * inverted_y + x;
	return result;
}

template<typename NodeType>
inline utils::small_vector<utils::coords,1> utils::grid<NodeType>::get_path(const utils::coords& start, const auto& is_end_fn, const auto& traverse_cost_fn, const auto& heuristic_fn) const
{
	AdventCheck(is_on_grid(start));
	constexpr bool check_end_fn = utils::grid_helpers::is_end_fn<NodeType,decltype(is_end_fn)>();
	constexpr bool check_traverse_fn = utils::grid_helpers::is_cost_fn<NodeType,decltype(traverse_cost_fn)>();
	constexpr bool check_heuristic_fn = utils::grid_helpers::is_heuristic_fn<NodeType,decltype(heuristic_fn)>();
	static_assert(check_end_fn, "is_end_fn must have the signature bool(utils::coords,NodeType)");
	static_assert(check_traverse_fn, "traverse_fn must have the signature std::optional<float>(utils::coords,NodeType,utils::coords,NodeType)");
	static_assert(check_heuristic_fn, "heuristic_fn must have the signature float(utils::coords,NodeType)");

	utils::small_vector<utils::coords,1> result;

	struct SearchNode
	{
		int previous_node_id = -1;
		utils::coords position;
		float cost = 0.0f;
		float cost_and_heuristic = 0.0f;
	};

	auto order_on_heuristic = [](const SearchNode& left, const SearchNode& right)
	{
		return left.cost_and_heuristic > right.cost_and_heuristic;
	};

	utils::small_vector<SearchNode,1> searched_nodes;
	utils::small_vector<SearchNode,1> unsearched_nodes;

	auto try_add_node = [this,&unsearched_nodes,&searched_nodes, &traverse_cost_fn, &heuristic_fn]
		(int previous_node_id, const SearchNode& from, utils::coords to)
	{
		if (!is_on_grid(to))
		{
#if AOC_GRID_DEBUG
			std::cout << "    Skip adding node at " << to << ": Not on grid.\n";
#endif
			return;
		}
		const auto has_checked_result = std::find_if(begin(searched_nodes), end(searched_nodes), [this,&to](const SearchNode& node)
			{
				return node.position == to;
			});
		if (has_checked_result != end(searched_nodes))
		{
#if AOC_GRID_DEBUG
			std::cout << "    Skip adding node at " << to << ": Already checked.\n";
#endif
			return;
		}
		SearchNode result;
		result.previous_node_id = previous_node_id;
		result.position = to;

		const NodeType& to_node(at(to));
		if (previous_node_id >= 0)
		{
			const NodeType& from_node = at(from.position);
			const std::optional<float> latest_cost_opt = traverse_cost_fn(from.position, from_node, to, to_node);
			if (!latest_cost_opt.has_value())
			{
#if AOC_GRID_DEBUG
				std::cout << "    Skip adding node at " << to << ": Cannot traverse.\n";
#endif
				return;
			}
			const float latest_cost = *latest_cost_opt;
			result.cost = from.cost + latest_cost;
		}
		else
		{
			result.cost = 0.0f;
		}
		const float heuristic = heuristic_fn(to, to_node);
		result.cost_and_heuristic = result.cost + heuristic;
#if AOC_GRID_DEBUG
		std::cout << "    Adding node to search: Loc="
			<< result.position << " C=" << result.cost << " H=" << result.cost_and_heuristic << '\n';
#endif
		unsearched_nodes.push_back(result);
	};

	{
		SearchNode first_node;
		first_node.position = start;
		try_add_node(-1, SearchNode{}, start);
	}

	while (!unsearched_nodes.empty())
	{
#if AOC_GRID_DEBUG
		std::cout << "Searched nodes: " << searched_nodes.size() << " Unsearched nodes: " << unsearched_nodes.size() << '\n';
#endif
		const SearchNode next_node = unsearched_nodes.back();
#if AOC_GRID_DEBUG
		std::cout << "Expanding node: " << next_node.position << " with cost=" << next_node.cost
			<< " heuristic=" << next_node.cost_and_heuristic << '\n';
#endif
		unsearched_nodes.pop_back();

		{
			const auto already_checked_result = std::find_if(begin(searched_nodes), end(searched_nodes),
				[&next_node](const SearchNode& previous_node)
				{
					return next_node.position == previous_node.position;
				});
			if (already_checked_result != end(searched_nodes))
			{
#if AOC_GRID_DEBUG
				std::cout << "    Skipping node " << next_node.position << ": already searched here.\n";
#endif
				continue;
			}
		}

		const bool node_is_end = is_end_fn(next_node.position,at(next_node.position));
		if (node_is_end)
		{
			SearchNode path_node = next_node;
			while(true)
			{
				result.push_back(path_node.position);
				if (path_node.previous_node_id < 0)
				{
					break;
				}
				else
				{
					const std::size_t previous_idx = static_cast<std::size_t>(path_node.previous_node_id);
					AdventCheck(previous_idx < searched_nodes.size());
					path_node = searched_nodes[previous_idx];
				}
			}
#if AOC_GRID_DEBUG
			std::cout << "Found target node: " << next_node.position << " Total path len=" << result.size() << '\n';
#endif
			break;
		}

		const int previous_id = static_cast<int>(searched_nodes.size());
		for (int dx : utils::int_range{ -1,2 })
		{
			for (int dy : utils::int_range{ -1,2 })
			{
				if (dx == 0 && dy == 0) continue;
				const utils::coords delta_pos{ dx,dy };
				const utils::coords next_coords = next_node.position + delta_pos;
				try_add_node(previous_id, next_node, next_coords);
			}
		}

		std::sort(begin(unsearched_nodes), end(unsearched_nodes), order_on_heuristic);
		searched_nodes.push_back(next_node);
	}

	return result;
}

template<typename NodeType>
inline utils::small_vector<utils::coords,1> utils::grid<NodeType>::get_path(const utils::coords& start, const auto& is_end_fn, const auto& cost_or_heuristic_fn) const
{
	constexpr bool is_cost_fn = utils::grid_helpers::is_cost_fn<NodeType,decltype(cost_or_heuristic_fn)>();
	constexpr bool is_heuristic_fn = utils::grid_helpers::is_heuristic_fn<NodeType, decltype(cost_or_heuristic_fn)>();
	static_assert(is_cost_fn || is_heuristic_fn, "cost_or_heuristic_fn must be a cost [std::optional<float>(utils::coords,utils::coords)] or a heuristic [float(utils::coords)] function");
	if constexpr (is_cost_fn)
	{
		auto heuristic_fn = utils::grid_helpers::DefaultHeuristicFunctor<NodeType>{};
		return get_path(start, is_end_fn, cost_or_heuristic_fn, heuristic_fn);
	}
	if constexpr (is_heuristic_fn)
	{
		auto cost_fn = utils::grid_helpers::DefaultCostFunctor<false>{};
		return get_path(start, is_end_fn, cost_fn, cost_or_heuristic_fn);
	}
	AdventUnreachable();
	return utils::small_vector<utils::coords,1>{};
}

template<typename NodeType>
inline utils::small_vector<utils::coords,1> utils::grid<NodeType>::get_path(const utils::coords& start, const auto& is_end_fn) const
{
	return get_path(start, is_end_fn, utils::grid_helpers::DefaultCostFunctor<false>{}, utils::grid_helpers::DefaultHeuristicFunctor<NodeType>{});
}

template<typename NodeType>
inline utils::small_vector<utils::coords,1> utils::grid<NodeType>::get_path(const utils::coords& start, const utils::coords& end, const auto& traverse_cost_fn, const auto& heuristic_fn) const
{
	auto is_end_fn = [&end](const utils::coords& test, const NodeType& node)
	{
		return test == end;
	};
	return get_path(start, is_end_fn, traverse_cost_fn, heuristic_fn);
}

template<typename NodeType>
inline utils::small_vector<utils::coords,1> utils::grid<NodeType>::get_path(const utils::coords& start, const utils::coords& end, const auto& cost_or_heuristic_fn) const
{
	constexpr bool is_cost_fn = utils::grid_helpers::is_cost_fn<NodeType,decltype(cost_or_heuristic_fn)>();
	constexpr bool is_heuristic_fn = utils::grid_helpers::is_heuristic_fn<NodeType,decltype(cost_or_heuristic_fn)>();
	static_assert(is_cost_fn || is_heuristic_fn, "cost_or_heuristic_fn must be a cost [std::optional<float>(utils::coords,utils::coords)] or a heuristic [float(utils::coords)] function");
	if constexpr (is_cost_fn)
	{
		auto heuristic_fn = utils::grid_helpers::DefaultHeuristicFunctor<NodeType>{end};
		return get_path(start, end, cost_or_heuristic_fn, heuristic_fn);
	}
	if constexpr (is_heuristic_fn)
	{
		auto cost_fn = utils::grid_helpers::DefaultCostFunctor<false>{};
		return get_path(start, end, cost_fn, cost_or_heuristic_fn);
	}
	AdventUnreachable();
	return utils::small_vector<utils::coords,1>{};
}

template<typename NodeType>
inline utils::small_vector<utils::coords,1> utils::grid<NodeType>::get_path(const utils::coords& start, const utils::coords& end) const
{
	return get_path(start, end, utils::grid_helpers::DefaultCostFunctor{}, utils::grid_helpers::DefaultHeuristicFunctor<NodeType>{ end });
}

template<typename NodeType>
template<typename Convert>
inline void utils::grid<NodeType>::stream_row(std::ostream& oss, int row_idx, const Convert& convert) const
{
	AdventCheck(utils::range_contains_exc(row_idx, 0, m_max_point.y));
	const auto row_view = grid_helpers::get_row_elem_view(*this, row_idx);
	grid_helpers::stream_view(oss, row_view, convert);
}

template<typename NodeType>
template<typename Convert>
inline void utils::grid<NodeType>::stream_column(std::ostream& oss, int column_idx, const Convert& convert) const
{
	AdventCheck(utils::range_contains_exc(column_idx, 0, m_max_point.x));
	const auto column_view = grid_helpers::get_column_elem_view(*this, column_idx);
	grid_helpers::stream_view(oss, column_view, convert);
}

template<typename NodeType>
template<typename Convert>
inline void utils::grid<NodeType>::stream_grid(std::ostream& oss, const Convert& convert) const
{
	for (int row_idx : utils::int_range{ m_max_point.y }.reverse())
	{
		oss << '\n';
		stream_row(oss, row_idx, convert);
	}
}