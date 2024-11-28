#pragma once

#include <algorithm>
#include <string_view>
#include <cctype>

namespace utils
{
	inline std::string_view trim_left(std::string_view sv)
	{
		while (!sv.empty() && std::isspace(sv.front()))
		{
			sv.remove_prefix(1);
		}
		return sv;
	}

	inline std::string_view trim_right(std::string_view sv)
	{
		while (!sv.empty() && std::isspace(sv.back()))
		{
			sv.remove_suffix(1);
		}
		return sv;
	}

	inline std::string_view trim_string(std::string_view sv)
	{
		return trim_left(trim_right(sv));
	}
}