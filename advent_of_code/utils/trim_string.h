#pragma once

#include <algorithm>
#include <string_view>
#include <cctype>

namespace utils
{
	namespace utils_internal
	{
		inline std::size_t trim_left_result(const std::string_view& sv)
		{
			const auto left_it = std::find_if_not(begin(sv), end(sv), std::isspace);
			return std::distance(begin(sv), left_it);
		}

		inline std::size_t trim_right_result(const std::string_view& sv)
		{
			const auto right_it = std::find_if_not(rbegin(sv), rend(sv), std::isspace);
			return std::distance(rbegin(sv), right_it);
		}
	}

	inline std::string_view trim_left(std::string_view sv)
	{
		const auto prefix_size = utils_internal::trim_left_result(sv);
		sv.remove_prefix(prefix_size);
		return sv;
	}

	inline std::string_view trim_right(std::string_view sv)
	{
		const auto suffix_size = utils_internal::trim_right_result(sv);
		sv.remove_suffix(suffix_size);
		return sv;
	}

	inline std::string_view trim_string(std::string_view sv)
	{
		return trim_left(trim_right(sv));
	}
}