#pragma once

#include <charconv>
#include <string_view>
#include <cassert>
#include <algorithm>

#include "trim_string.h"
#include "../advent/advent_utils.h"

namespace utils
{
	inline bool is_value(std::string_view sv)
	{
		if (sv.empty())
		{
			return true;
		}
		return (std::string_view("0123456789+-").find(sv[0]) != std::string_view::npos)
			&& (std::all_of(begin(sv) + 1, end(sv), ::isdigit));
	}

	template <typename T>
	inline T to_value(std::string_view sv)
	{
		sv = trim_string(sv);
		AdventCheckMsg(is_value(sv),"Could not convert string to value: " , sv);
		if (!sv.empty() && sv.front() == '+')
		{
			sv.remove_prefix(1);
		}
		if (sv.empty())
		{
			return T{ 0 };
		}
		
		const char* first = sv.data();
		const char* last = first + sv.size();
		T value{};
		const std::from_chars_result result = std::from_chars(first, last, value);
		AdventCheckMsg(result.ec == std::errc{},"ErrNo return parsing string '", sv);
		AdventCheckMsg(result.ptr == last,"Could not convert string to value: " , sv);
		return value;
	}
}