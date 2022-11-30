#pragma once

#include <cstddef>
#include <iterator>
#include <compare>
#include "index_iterator.h"

namespace utils
{

	template <typename INT,
		std::enable_if_t<std::is_integral<INT>::value, int> = 0
	>
		class int_range
	{
		INT min, max, stride;
		static constexpr INT get_max(INT min, INT max, INT stride)
		{
			const INT base_size = max - min;
			const bool even = (base_size % stride == 0);
			if (even)
			{
				return max;
			}
			else
			{
				const INT real_size = (base_size / stride) + 1;
				return min + (real_size * stride);
			}
		}
	public:
		// Typedefs
		using value_type = INT;
		using reference_type = INT;
		using pointer_type = INT*;
		using size_type = std::size_t;

		// Constructors
		constexpr int_range(INT start, INT finish, INT stride_length) noexcept :
			min{ start },
			max{ get_max(start,finish,stride_length) },
			stride{ stride_length }
		{}
		constexpr int_range(INT start, INT finish) noexcept : int_range{ start,finish,1 } {}
		constexpr int_range(INT finish) noexcept : int_range{ 0,finish } {}
		constexpr int_range() : int_range{ 0 } {}

		constexpr INT operator[](std::size_t pos) const noexcept {
			assert(pos < size());
			return min + (stride * static_cast<INT>(pos));
		}

		constexpr INT front() const noexcept {
			return min;
		}

		constexpr INT back() const noexcept {
			return max - stride;
		}

		// Operations
		constexpr size_type  size() const noexcept {
			return static_cast<size_type>((max - min) / stride);
		}

		constexpr int_range<INT> reverse() const noexcept
		{
			return int_range<INT>{ max, min - 1, -stride };
		}

		INDEX_ITERATOR_MEMBER_BOILERPLATE_CONST(int_range<INT>)
	};

}

