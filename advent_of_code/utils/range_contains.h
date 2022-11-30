#pragma once

#include <algorithm>
#include <cassert>

namespace utils
{
	template <typename T> 
	inline bool range_contains_inc(T val, T min, T max)
	{
		if (min > max)
		{
			return range_contains_inc(val, max, min);
		}
		return min <= val && val <= max;
	}

	template <typename T>
	inline bool range_contains_exc(T val, T min, T max)
	{
		if (min > max)
		{
			return range_contains_exc(val, max, min);
		}
		return min <= val && val < max;
	}

	template <typename T>
	inline bool region_contains(T val, T comp_a, T comp_b)
	{
		const auto [min, max] = std::minmax(comp_a, comp_b);
		return range_contains_inc(val, min, max);
	}
}