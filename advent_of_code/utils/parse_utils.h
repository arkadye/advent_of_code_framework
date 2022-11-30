#pragma once

#include <string_view>
#include <array>

#include "split_string.h"

namespace utils
{
	std::string_view remove_specific_prefix(std::string_view input, std::string_view prefix);
	std::string_view remove_specific_suffix(std::string_view input, std::string_view suffix);

	namespace internal
	{
		inline std::array<std::string_view, 0> get_elements_impl(
			std::string_view input,
			char delim,
			std::size_t current_idx)
		{
			return std::array<std::string_view, 0>{};
		}


		template <typename...Indices>
		inline std::array<std::string_view, sizeof...(Indices)+1> get_elements_impl(
			std::string_view input,
			char delim,
			std::size_t current_idx,
			std::size_t first_elem,
			Indices... rest
		)
		{
			const auto [elem, input_rest] = split_string_at_first(input, delim);
			if (current_idx == first_elem)
			{
				std::array<std::string_view, sizeof...(Indices) + 1> result;
				result.front() = elem;
				const auto results_rest = get_elements_impl(input_rest, delim, current_idx + 1, rest...);
				std::copy(begin(results_rest), end(results_rest), begin(result) + 1);
				return result;
			}
			// Else
			return get_elements_impl(input_rest, delim, current_idx + 1, first_elem, rest...);
		}
	}

	template <typename...Indices>
	inline std::array<std::string_view, sizeof...(Indices)> get_string_elements(std::string_view input, char delim,Indices...indices)
	{
		static_assert(sizeof...(indices) > 0);
		return internal::get_elements_impl(input, delim, 0, indices...);
	}

	template <typename...Indices>
	inline std::array<std::string_view, sizeof...(Indices)> get_string_elements(std::string_view input, Indices...indices)
	{
		static_assert(sizeof...(indices) > 0);
		return get_string_elements(input, ' ', indices...);
	}

	inline std::string_view get_string_element(std::string_view input, char delim, std::size_t index)
	{
		return get_string_elements(input, delim, index)[0];
	}

	inline std::string_view get_string_element(std::string_view input, std::size_t index)
	{
		return get_string_element(input, ' ', index);
	}
}