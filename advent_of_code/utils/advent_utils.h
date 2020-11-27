#pragma once

#include <map>
#include <algorithm>

// Returns a map with items from both inputs. Where a key is duplicated, combine_func is used to combine
// the values pointed to.
template <typename Key, typename Val, typename BinaryOp>
inline std::map<Key,Val> combine_maps(std::map<Key, Val> a, std::map<Key, Val> b, BinaryOp combine_func)
{
	if (a.size() < b.size()) return combine_maps(std::move(b), std::move(a),combine_func);

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

template <typename Cont, typename Pred>
inline void erase_remove_if(Cont& c, Pred p)
{
	c.erase(std::remove_if(begin(c), end(c), p), end(c));
}

template <typename Container, typename ElemType>
bool push_back_unique(Container& cont, ElemType&& elem)
{
	const auto find_result = std::find(begin(cont), end(cont), elem);
	if (find_result == end(cont))
	{
		cont.push_back(std::forward<ElemType>(elem));
		return true;
	}
	return false;
}