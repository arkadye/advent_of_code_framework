#pragma once

namespace utils
{
	template <typename ItType, typename Predicate>
	bool is_sorted(ItType first, ItType last, Predicate pred) noexcept
	{
		if (first == last)
		{
			return true;
		}
		ItType second = first;
		++second;
		while (second != last)
		{
			if (pred(*second, *first))
			{
				return false;
			}
			++first;
			++second;
		}
		return true;
	}

	template <typename ItType>
	bool is_sorted(ItType first, ItType last)
	{
		using ValueType = decltype(*first);
		auto predicate = [](const ValueType& l, const ValueType& r)
		{
			return l < r;
		};
		return utils::is_sorted(first, last, predicate);
	}
}