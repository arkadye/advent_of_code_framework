#pragma once

#include <algorithm>

namespace utils
{

	template <typename Cont, typename Pred>
	inline void erase_remove_if(Cont& c, Pred p)
	{
		c.erase(std::remove_if(begin(c), end(c), p), end(c));
	}

}