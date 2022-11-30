#pragma once

#include <iterator>
#include <cassert>

namespace utils
{
	template <typename ItType>
	class span
	{
		ItType first, last;
	public:
		ItType begin() const noexcept { return first; }
		ItType end() const noexcept { return last; }
		span(const span& other) = default;
		span& operator=(const span& other) = default;
		span(ItType f, ItType l) : first{ f }, last{ l }{}
		std::size_t size() const noexcept
		{
			return std::distance(first, last);
		}
		bool empty() const noexcept { return first == last; }
		span remove_prefix(std::size_t num) const
		{
			assert(num <= size());
			ItType new_first = first;
			std::advance(new_first, num);
			return span{ new_first,last };
		}
		span remove_suffix(std::size_t num) const noexcept
		{
			assert(num <= size());
			ItType new_last = last;
			std::ptrdiff_t offset = 0 - static_cast<std::ptrdiff_t>(num);
			std::advance(new_last, offset);
			return span{ first,new_last };
		}
		auto operator[](std::size_t idx) const noexcept
		{
			assert(idx < size());
			ItType location = first;
			std::advance(location, idx);
			return *location;
		}
	};

	template <typename Container>
	auto make_span(const Container& container)
	{
		using ItType = decltype(begin(container));
		return span<ItType>{begin(container), end(container)};
	}
}