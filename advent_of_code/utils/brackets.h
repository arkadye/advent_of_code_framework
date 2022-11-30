#pragma once

#include <string_view>
#include "../advent/advent_utils.h"

namespace utils
{
	[[nodiscard]] inline std::size_t find_closing_bracket(
		std::string_view input,
		char open_bracket, char close_bracket,
		std::size_t open_pos = 0)
	{
		AdventCheck(input[open_pos] == open_bracket);
		int num_open_brackets = 1;
		for (std::size_t i = open_pos + 1; i < input.size(); ++i)
			if (input[i] == open_bracket) ++num_open_brackets;
			else if (input[i] == close_bracket)
			{
				--num_open_brackets;
				if (num_open_brackets == 0)
				{
					return i;
				}
			}
		return std::string_view::npos;
	}

	[[nodiscard]] inline std::size_t bracket_aware_find_first_of(
		std::string_view input,
		std::string_view open_brackets,
		std::string_view closing_brackets,
		std::string_view find_targets,
		std::size_t search_start = 0)
	{
		AdventCheck(open_brackets.size() == closing_brackets.size());
		AdventCheck(!find_targets.empty());
		for (std::size_t i = search_start; i < input.size(); ++i)
		{
			const char current_char = input[i];
			if (find_targets.find(current_char) < find_targets.size())
			{
				return i;
			}
			const auto open_bracket_find_result = open_brackets.find(current_char);
			if (open_bracket_find_result < open_brackets.size())
			{
				const char close_bracket = closing_brackets[open_bracket_find_result];
				i = find_closing_bracket(input, current_char, close_bracket, i);
				if (i >= input.size())
				{
					break;
				}
			}
		}
		return std::string_view::npos;
	}

	[[nodiscard]] inline std::size_t bracket_aware_find(
		std::string_view input,
		std::string_view open_brackets,
		std::string_view closing_brackets,
		char find_target,
		std::size_t search_start = 0)
	{
		std::string_view targets(&find_target, 1);
		return bracket_aware_find_first_of(input, open_brackets, closing_brackets, targets, search_start);
	}

	[[nodiscard]] inline std::size_t bracket_aware_find(
		std::string_view input,
		char open_bracket,
		char close_bracket,
		char find_target,
		std::size_t start_search = 0)
	{
		std::string_view open(&open_bracket, 1);
		std::string_view close(&close_bracket, 1);
		return bracket_aware_find(input, open, close, find_target, start_search);
	}
}