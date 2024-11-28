#pragma once

namespace utils
{
	template <typename FwdIt, typename OutputIt, typename Transform, typename PrePred, typename PostPred>
	OutputIt transform_if(FwdIt start, FwdIt end, OutputIt destination, Transform transform_func, PrePred pre_pred, PostPred post_pred)
	{
		for (auto it = start; it != end; ++it)
		{
			const auto& value = *it;
			if (pre_pred(value))
			{
				auto transform_result = transform_func(value);
				if (post_pred(transform_result))
				{
					*destination = std::move(transform_result);
					++destination;
				}
			}
		}
		return destination;
	}

	template <typename FwdIt, typename OutputIt, typename Transform, typename Pred>
	OutputIt transform_if_pre(FwdIt start, FwdIt end, OutputIt destination, Transform transform_func, Pred predicate_func)
	{
		return transform_if(start, end, destination, transform_func, predicate_func, [](const decltype(transform_func(*start))&) {return true; });
	}
	
	template <typename FwdIt, typename OutputIt, typename Transform, typename Pred>
	OutputIt transform_if_post(FwdIt start, FwdIt end, OutputIt destination, Transform transform_func, Pred predicate_func)
	{
		return transform_if(start, end, destination, transform_func, [](const decltype(*start)&) {return true; }, predicate_func);
	}

	namespace ranges
	{
		template <typename RangeType, typename OutputIt, typename Transform, typename PrePred, typename PostPred>
		OutputIt transform_if(RangeType&& range, OutputIt destination, Transform transform_func, PrePred pre_pred, PostPred post_pred)
		{
			return utils::transform_if(begin(range), end(range), destination, transform_func, pre_pred, post_pred);
		}

		template <typename RangeType, typename OutputIt, typename Transform, typename Pred>
		OutputIt transform_if_pre(RangeType&& range, OutputIt destination, Transform transform_func, Pred predicate_func)
		{
			return utils::transform_if_pre(begin(range), end(range), destination, transform_func, predicate_func);
		}

		template <typename RangeType, typename OutputIt, typename Transform, typename Pred>
		OutputIt transform_if_post(RangeType&& range, OutputIt destination, Transform transform_func, Pred predicate_func)
		{
			return utils::transform_if_post(begin(range), end(range), destination, transform_func, predicate_func);
		}
	}
}