#pragma once

#include <charconv>
#include <string_view>
#include <cassert>
#include <algorithm>

#include "trim_string.h"
#include "advent/advent_utils.h"

namespace utils
{
	template <int base = 10>
	inline bool is_value(std::string_view sv)
	{
		static_assert(0 < base, "is_value only supprts positive bases.");
		static_assert(base <= 16, "is_value only supports bases up to 16.");
		if (sv.empty())
		{
			return true;
		}

		switch (sv.front())
		{
		case '+':
		case '-':
			sv.remove_prefix(1);
			break;
		default:
			break;
		}

		
		auto is_allowed_char = [sbase = std::size_t{base}](char c)
			{
				constexpr std::string_view allowed_chars{ "0123456789ABCDEF" };
				c = std::toupper(c);
				const auto find_result = allowed_chars.find(c);
				return find_result < sbase;
			};

		return stdr::all_of(sv, is_allowed_char);
	}

	template <std::integral T, int base = 10>
	inline T to_value(std::string_view sv)
	{
		sv = trim_string(sv);
		AdventCheckMsg(is_value(sv),"Could not convert string to value: " , sv);
		if (sv.empty())
		{
			return T{ 0 };
		}
		if (sv.front() == '+')
		{
			sv.remove_prefix(1);
		}
		
		const char* first = sv.data();
		const char* last = first + sv.size();
		T value{};
		const std::from_chars_result result = std::from_chars(first, last, value, base);
		AdventCheckMsg(result.ec == std::errc{},"ErrNo return parsing string '", sv);
		AdventCheckMsg(result.ptr == last,"Could not convert string to value: " , sv);
		return value;
	}
}