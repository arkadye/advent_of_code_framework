#pragma once

#include <string_view>
#include <vector>
#include <numeric>
#include <array>

namespace utils
{
	[[nodiscard]] inline std::pair<std::string_view, std::string_view> split_string_at_point(std::string_view str, std::size_t split_point)
	{
		if (split_point < str.size())
		{
			return std::pair{ str.substr(0,split_point),str.substr(split_point + 1) };
		}
		else
		{
			return std::pair{ str,std::string{} };
		}
	}

	[[nodiscard]] inline std::pair<std::string_view, std::string_view> split_string_at_first(std::string_view str, std::string_view delim)
	{
		const auto split_point = str.find(delim);
		if (split_point < str.size())
		{
			return std::pair{ str.substr(0,split_point),str.substr(split_point + delim.size()) };
		}
		else
		{
			return std::pair{ str,std::string{} };
		}
	}

	[[nodiscard]] inline std::pair<std::string_view, std::string_view> split_string_at_first(std::string_view str, char delim)
	{
		return split_string_at_first(str, std::string_view(&delim, 1));
	}

	[[nodiscard]] inline std::pair<std::string_view, std::string_view> split_string_at_last(std::string_view str, std::string_view delim)
	{
		const auto split_point = str.rfind(delim);
		if (split_point < str.size())
		{
			return std::pair{ str.substr(0,split_point),str.substr(split_point + delim.size()) };
		}
		else
		{
			return std::pair{ str,std::string{} };
		}
	}

	[[nodiscard]] inline std::pair<std::string_view, std::string_view> split_string_at_last(std::string_view str, char delim)
	{
		return split_string_at_last(str, std::string_view(&delim, 1));
	}

	[[nodiscard]] inline std::vector<std::string_view> split_string(std::string_view str, std::string_view delim)
	{
		std::vector<std::string_view> result;
		std::string_view split_state = str;
		while (!split_state.empty())
		{
			const auto [next_section, next_state] = split_string_at_first(split_state, delim);
			result.push_back(next_section);
			split_state = next_state;
		}
		return result;
	}

	[[nodiscard]] inline std::vector<std::string_view> split_string(std::string_view str, char delim)
	{
		return split_string(str, std::string_view{ &delim,1 });
	}

	[[nodiscard]] inline std::vector<std::string_view> split_string(std::string_view str)
	{
		return split_string(str, ' ');
	}
}