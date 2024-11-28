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
	
	// Predicate is a function that can be called predicate(*FwdIt).
	// predicate returns:
	// equivalent - return this value
	// less - this iterator points to a value that is too small
	// greater - this iterator points to a value that is too large
	template <typename FwdIt, typename ThreeWayPredicate>
	inline FwdIt binary_find_by_predicate(FwdIt start, FwdIt finish, const ThreeWayPredicate& predicate)
	{
		static_assert(std::is_invocable_r_v<std::weak_ordering, ThreeWayPredicate, decltype(*start)>, "predicate must accept FwdIt::value_type as an argument, and return a std::weak_ordering");
		if (start == finish) return finish;
		const auto range_len = std::distance(start, finish);
		const FwdIt midpoint = stdr::next(start, range_len / 2);

		const bool is_final_check = (start == midpoint);

		const std::weak_ordering position = predicate(*midpoint);

		if (position == std::weak_ordering::equivalent) // Found it
		{
			return midpoint;
		}
		else if (midpoint == start)
		{
			return finish;
		}

		FwdIt next_start = start;
		FwdIt next_finish = finish;
		
		if (position == std::weak_ordering::less)
		{
			next_start = midpoint;
		}
		else if (position == std::weak_ordering::greater) // Iterator's value to too big
		{
			next_finish = midpoint;
		}
		else // That should cover all the cases
		{
			AdventUnreachable();
		}

		const FwdIt result = utils::binary_find_by_predicate(next_start, next_finish, predicate);
		return (result != next_finish) ? result : finish;
	}

	// This helper version of find_by_predicate allows a reference value to be passed in.
	template <typename FwdIt, typename RefType, typename ThreeWayPredicate>
	inline FwdIt binary_find_by_predicate(FwdIt start, FwdIt finish, const RefType& ref, const ThreeWayPredicate& predicate)
	{
		using ValueType = decltype(*start);
		static_assert(std::is_invocable_r_v<std::weak_ordering, ThreeWayPredicate, ValueType, RefType>, "predicate must have a signature like std::weak_ordering predicate(decltype(*FwdIt,RefType)");
		auto unary_predicate = [&ref, &predicate](const ValueType& val)
		{
			return predicate(val, ref);
		};
		return utils::binary_find_by_predicate(start, finish, unary_predicate);
	}

	namespace ranges
	{
		template <typename RangeType, typename T, typename Pred>
		inline auto binary_find(RangeType&& range, const T& val, const Pred& pred) noexcept
		{
			return utils::binary_find(begin(range), end(range), val, pred);
		}

		template <typename RangeType, typename T>
		inline auto binary_find(RangeType&& range, const T& val) noexcept
		{
			return binary_find(begin(range),end(range), val, std::less<T>{});
		}

		// Predicate is a function that can be called predicate(*FwdIt).
		// predicate returns:
		// equivalent - return this value
		// less - this iterator points to a value that is too small
		// greater - this iterator points to a value that is too large
		template <typename RangeType, typename ThreeWayPredicate>
		inline auto binary_find_by_predicate(RangeType&& range, const ThreeWayPredicate& predicate)
		{
			return utils::binary_find_by_predicate(begin(range), end(range), predicate);
		}

		// This helper version of find_by_predicate allows a reference value to be passed in.
		template <typename RangeType, typename RefType, typename ThreeWayPredicate>
		inline auto binary_find_by_predicate(RangeType&& range, const RefType& ref, const ThreeWayPredicate& predicate)
		{
			return utils::binary_find_by_predicate(begin(range), end(range), ref, predicate);
		}
	}
}