#pragma once

namespace utils
{
	template <typename FwdIt, typename Transform>
	[[nodiscard]] inline FwdIt min_element_transform(FwdIt first, FwdIt last, const Transform& transform) noexcept
	{
		if (first == last)
		{
			return last;
		}
		using ValType = decltype(*first);
		FwdIt result = first;
		for (FwdIt it = first; it != last; ++it)
		{
			if (transform(*it) < transform(*result))
			{
				result = it;
			}
		}
		return result;
	}

	template <typename FwdIt, typename Transform>
	[[nodiscard]] inline FwdIt max_element_transform(FwdIt first, FwdIt last, const Transform& transform) noexcept
	{
		if (first == last)
		{
			return last;
		}
		using ValType = decltype(*first);
		FwdIt result = first;
		for (FwdIt it = first; it != last; std::advance(it,1))
		{
			if (transform(*it) > transform(*result))
			{
				result = it;
			}
		}
		return result;
	}

	template <typename FwdIt, typename Transform>
	[[nodiscard]] inline auto min_transform(FwdIt first, FwdIt last, const Transform& transform) noexcept
	{
		return *min_element_transform(first, last, transform);
	}

	template <typename FwdIt, typename Transform>
	[[nodiscard]] inline auto max_transform(FwdIt first, FwdIt last, const Transform& transform) noexcept
	{
		return *max_element_transform(first, last, transform);
	}
}