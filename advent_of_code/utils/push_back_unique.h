#pragma once

#include <algorithm>

namespace utils
{

	template <typename Container, typename ElemType>
	auto push_back_unique(Container& cont, ElemType&& elem)
	{
		const auto find_result = std::find_if(begin(cont), end(cont),
			[&elem](const auto& c)
		{
			return c == elem;
		});
		if (find_result == end(cont))
		{
			cont.emplace_back(std::forward<ElemType>(elem));
			auto last = end(cont);
			std::advance(last, -1);
			return last;
		}
		return find_result;
	}
}