#pragma once

namespace utils
{

	// Returns a map with items from both inputs. Where a key is duplicated, combine_func is used to combine
	// the values pointed to.
	template <typename MapType, typename BinaryOp>
	inline MapType combine_maps(MapType a, MapType b, BinaryOp combine_func)
	{
		if (a.size() < b.size()) return combine_maps(std::move(b), std::move(a), combine_func);

		while (!b.empty())
		{
			auto node = b.extract(cbegin(b));
			const auto it_to = a.find(node.key());
			if (it_to == end(a))
			{
				a.insert(std::move(node));
			}
			else
			{
				it_to->second = combine_func(it_to->second, node.mapped());
			}
		}
		return a;
	}
}