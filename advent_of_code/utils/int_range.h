#pragma once

#include <cstddef>
#include <iterator>
#include <compare>
#include <type_traits>

#include "index_iterator.h"

namespace utils
{

	template <std::integral INT>
	class int_range
	{
		INT min;
		INT max;
		INT stride;
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

		constexpr bool empty() const noexcept { return size() == 0u; }

		constexpr INT operator[](std::size_t pos) const noexcept {
			AdventCheck(pos < size());
			return min + (stride * static_cast<INT>(pos));
		}

		constexpr INT front() const noexcept {
			return min;
		}

		constexpr INT back() const noexcept {
			return max - stride;
		}

		// Operations
		constexpr size_type size() const noexcept {
			return static_cast<size_type>((max - min) / stride);
		}

		constexpr int_range<INT> reverse() const noexcept requires std::signed_integral<INT>
		{
			return int_range<INT>{ max - stride, min - stride, -stride };
		}

		INDEX_ITERATOR_MEMBER_BOILERPLATE_CONST(int_range<INT>)
	};

	template <typename AdaptorFn> requires std::regular_invocable<AdaptorFn,std::size_t>
	class int_range_adaptor
	{
		int_range<std::ptrdiff_t> range;
		AdaptorFn adaptor_fn;
		constexpr int_range_adaptor(AdaptorFn fn, int_range<std::ptrdiff_t> rng) noexcept : adaptor_fn{ std::move(fn) }, range{ rng } {}
	public:
		// Typedefs
		using reference_type = std::invoke_result_t<AdaptorFn, std::size_t>;
		using value_type = std::remove_cvref_t<reference_type>;
		using pointer_type = value_type*;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		const AdaptorFn& get_adapter() const noexcept { return adaptor_fn; }
		const int_range<std::size_t>& get_underlying_range() const noexcept { return range; }

		// Constructors
		constexpr int_range_adaptor(AdaptorFn fn, std::ptrdiff_t start, std::ptrdiff_t finish, std::ptrdiff_t stride_length) noexcept :
			int_range_adaptor{ std::move(fn) , int_range<std::ptrdiff_t>{start,finish,stride_length} } {}
		constexpr int_range_adaptor(AdaptorFn fn, std::ptrdiff_t start, std::ptrdiff_t finish) noexcept
			: int_range_adaptor{ std::move(fn), start,finish,1 } {}
		constexpr int_range_adaptor(AdaptorFn fn, std::ptrdiff_t finish) noexcept : int_range_adaptor{ std::move(fn), 0,finish } {}
		constexpr int_range_adaptor(AdaptorFn fn) : int_range_adaptor{ std::move(fn), 0 } {}

		constexpr decltype(auto) operator[](std::size_t pos) const noexcept {
			AdventCheck(pos < size());
			return adaptor_fn(range[pos]);
		}

		constexpr decltype(auto) operator[](std::size_t pos) noexcept {
			AdventCheck(pos < size());
			return adaptor_fn(range[pos]);
		}

		constexpr decltype(auto) front() const noexcept {
			return (*this)[range.front()];
		}

		constexpr decltype(auto) front() noexcept
		{
			return (*this)[range.front()];
		}

		constexpr decltype(auto) back() const noexcept {
			return (*this)[range.back()];
		}

		constexpr decltype(auto) back() noexcept {
			return (*this)[range.back()];
		}

		// Operations
		constexpr size_type  size() const noexcept {
			return range.size();
		}

		constexpr bool empty() const noexcept
		{
			return size() == size_type{ 0 };
		}

		constexpr int_range_adaptor<AdaptorFn> reverse() const noexcept
		{
			return int_range_adaptor<AdaptorFn>{ adaptor_fn, range.reverse() };
		}

		INDEX_ITERATOR_MEMBER_BOILERPLATE_CONST(int_range_adaptor<AdaptorFn>)
	};
}

