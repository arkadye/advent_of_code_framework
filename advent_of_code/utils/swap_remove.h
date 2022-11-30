#pragma once

#include <vector>
#include <type_traits>

#include "../advent/advent_utils.h"

namespace utils
{
	template <typename VecType>
	void swap_remove(VecType& vector, std::size_t idx)
	{
		AdventCheck(idx < vector.size());
		vector[idx] = std::move(vector.back());
		vector.pop_back();
	}

	template <typename VecType>
	void swap_remove(VecType& vector, typename VecType::iterator loc)
	{
		static_assert(!std::is_const_v<VecType>, "Input must be non-const.");
		*loc = std::move(vector.back());
		vector.pop_back();
	}

	// Returns true if it successfully removes a value.
	template <typename VecType>
	bool swap_remove_single(VecType& vector, typename VecType::const_reference value)
	{
		static_assert(!std::is_const_v<VecType>, "Input must be non-const.");
		const VecType::iterator loc = std::find(begin(vector), end(vector), value);
		if (loc != end(vector))
		{
			swap_remove(vector, loc);
			return true;
		}
		return false;
	}

	template <typename VecType, typename Pred>
	typename VecType::size_type swap_remove_if(VecType& vector, const Pred& pred)
	{
		static_assert(!std::is_const_v<VecType>, "Input must be non-const.");
		const auto original_size = vector.size();
		typename VecType::size_type i = 0;
		while (i < vector.size())
		{
			if (pred(vector[i]))
			{
				auto it = begin(vector);
				std::advance(it, i);
				swap_remove(vector, it);
			}
			else
			{
				++i;
			}
		}
		return vector.size() - original_size;
	}

	template <typename VecType>
	typename VecType::size_type swap_remove_all(VecType& vector, typename VecType::const_reference value)
	{
		static_assert(!std::is_const_v<VecType>, "Input must be non-const.");
		return swap_remove_if(vector, [&value](const VecType::value_type& v) {return v == value; });
	}
}