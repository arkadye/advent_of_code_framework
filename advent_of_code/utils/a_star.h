#pragma once

#include <vector>
#include <algorithm>
#include <cassert>
#include <iterator>

#include "swap_remove.h"
#include "sorted_vector.h"

namespace utils
{
	// NodeType: An arbitrary node. No particular requirements. User provided functors are used to interact.
	// IsEndPointFunc: A function bool f(Node) that returns true if the argument is an end-point.
	// GetNextNodesFunc: Return any iterable type containing NodeTypes that can be reached from a NodeType argument.
	// GetCostBetweenNodesFunc: Functor with the signature: CostType f(NodeType,NodeType).
	// GetHeuristicForNode: Functor with CostType f(NodeType) to get the heuristic.
	// AreNodesEqual: A function bool f(NodeType,NodeType) that returns true if both nodes are equal
	template <
		typename NodeType,
		typename IsEndPointFunc,
		typename GetNextNodesFunc,
		typename GetCostBetweenNodesFunc,
		typename GetHeuristicForNode,
		typename AreNodesEqual>
		auto a_star(
			const NodeType& start_point,
			const IsEndPointFunc& is_end_point,
			const GetNextNodesFunc& get_next_nodes,
			const GetCostBetweenNodesFunc& get_cost_between_nodes,
			const GetHeuristicForNode& get_heuristic,
			const AreNodesEqual& are_nodes_equal,
			std::size_t estimated_number_of_nodes = 1)
	{
		using ID = std::size_t;
		using CostType = decltype(get_cost_between_nodes(start_point, start_point));
		struct AStarNode
		{
			NodeType node;
			CostType cost;
			CostType with_heuristic;
			ID id;
			ID previous_id;
		};

		ID latest_id = 0;
		auto checked_nodes = utils::make_sorted_vector<AStarNode>([](const AStarNode & l, const AStarNode & r)
		{
			return l.id < r.id;
		});

		auto get_checked_node_by_id = [&checked_nodes](ID id) -> AStarNode&
		{
			AStarNode dummy;
			dummy.id = id;
			const auto find_result = checked_nodes.find(dummy);
			assert(find_result != end(checked_nodes));
			return *find_result;
		};

		auto nodes_to_search = utils::make_sorted_vector<AStarNode>([](const AStarNode& l, const AStarNode& r)
		{
			return l.with_heuristic > r.with_heuristic;
		});
		nodes_to_search.reserve(estimated_number_of_nodes);
		checked_nodes.reserve(estimated_number_of_nodes);

		nodes_to_search.insert(AStarNode{ start_point,0,0,++latest_id,0 });

		while (!nodes_to_search.empty())
		{
			AStarNode current_node = std::move(nodes_to_search.back());
			nodes_to_search.pop_back();

			// Handle end-point
			if (is_end_point(current_node.node))
			{
				std::vector<NodeType> result;
				const auto final_cost = current_node.cost;
				while (true)
				{
					result.push_back(std::move(current_node.node));
					if (current_node.previous_id == 0)
					{
						std::reverse(begin(result), end(result));
						return std::make_pair(result, final_cost);
					}
					current_node = get_checked_node_by_id(current_node.previous_id);
				}

			}

			// Get next nodes
			auto next_nodes = get_next_nodes(current_node.node);
			for (auto& n : next_nodes)
			{
				const auto prev_node_check = std::any_of(begin(checked_nodes), end(checked_nodes),
					[&n,&are_nodes_equal](const AStarNode& asn)
				{
					return are_nodes_equal(n, asn.node);
				});
				if (prev_node_check)
				{
					continue;
				}
				AStarNode node;
				node.cost = current_node.cost + get_cost_between_nodes(current_node.node, n);
				node.with_heuristic = node.cost + get_heuristic(n);
				auto new_it = nodes_to_search.insert(nodes_to_search.upper_bound(node), std::move(node));
				new_it->node = std::move(n);
				new_it->previous_id = current_node.id;
				new_it->id = ++latest_id;
			}

			// Update checked nodes
			checked_nodes.push_back(std::move(current_node));
		}

		// If we run out of nodes, there's no path.
		return std::make_pair(std::vector<NodeType>{}, CostType{});
	}
}