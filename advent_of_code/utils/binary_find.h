#pragma once

#include <algorithm>

namespace utils
{
	template <typename FwdIt, typename T, typename Pred>
	inline FwdIt binary_find(FwdIt start, FwdIt finish, const T& val, const Pred& pred) noexcept
	{
		assert(std::is_sorted(start, finish, pred));
		const auto result = std::lower_bound(start, finish, val, pred);
		if (result == finish)
		{
			return finish;
		}
		if (pred(val, *result))
		{
			return finish;
		}
		if (pred(*result, val))
		{
			return finish;
		}
		return result;
	}

	template <typename FwdIt, typename T>
	inline FwdIt binary_find(FwdIt start, FwdIt finish, const T& val) noexcept
	{
		return binary_find(start, finish, val, std::less<T>{});
	}

}