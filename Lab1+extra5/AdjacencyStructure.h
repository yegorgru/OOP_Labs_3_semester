#pragma once

#include "Graph.h"
#include <vector>
#include <utility>
#include <map>
#include <algorithm>

template <typename NodeType, typename EdgeType>
class AdjacencyStructure:public Graph<NodeType,EdgeType>
{
	std::vector<std::map<size_t,EdgeType>>structure;
public:
	AdjacencyStructure(EdgeType zero_edge, EdgeType max_edge);
	size_t add_node(NodeType node) override;
	void add_redge(size_t first_node, size_t second_node, EdgeType edge) override;

	EdgeType delete_redge(size_t first_node, size_t second_node) override;
	void delete_node(size_t node) override;

	bool is_connected_graph() override;
	std::vector<std::vector<size_t>> find_components() override;

	std::vector<EdgeType> dijkstra_algorithm(size_t node) override;

	bool is_acyclic() override;

	bool is_tree() override;

	AdjacencyStructure<NodeType, EdgeType> minimum_spanning_tree_kruskal() override;
	AdjacencyStructure<NodeType, EdgeType> minimum_spanning_reverse_delete() override;
private:
	void find_nodes(size_t node, std::vector<bool>& for_check, std::vector<size_t>& component) override;	
	bool check_cyclic(size_t node, std::vector<char>& checked, size_t parent) override;
};

template<typename NodeType, typename EdgeType>
AdjacencyStructure<NodeType, EdgeType>::AdjacencyStructure(EdgeType zero_edge, EdgeType max_edge)
	: Graph(zero_edge, max_edge) {}

template<typename NodeType, typename EdgeType>
size_t AdjacencyStructure<NodeType, EdgeType>::add_node(NodeType node)
{
	this->Nodes[this->Nodes.size()] = node;
	structure.push_back(std::map<size_t, EdgeType >());
	return this->Nodes.size() - 1;
}

template<typename NodeType, typename EdgeType>
void AdjacencyStructure<NodeType, EdgeType>::add_redge(size_t first_node, size_t second_node, EdgeType edge)
{
	/*if (first_node < nodes.size() && second_node < nodes.size()) {
		if (first_node != second_node) {
			if (this->weighted) {
				if (!(this->find(first_node, second_node))) {
					nodes[first_node].push_back(second_node);
					weights[first_node].push_back(weight);
					if (!directed) {
						nodes[second_node].push_back(first_node);
						weights[second_node].push_back(weight);
					}
				}
				else {
					throw GraphErr("Edge with those nodes already exists");
				}
			}
			else {
				throw GraphErr("This type of function only for weighted graphs");
			}
		}
		else {
			throw GraphErr("Graph cannot contain self-loops");
		}
	}
	else {
		throw GraphErr("Incorrect first or second node");
	}*/
	if (first_node != second_node) {
		structure[first_node][second_node] = edge;
		structure[second_node][first_node] = edge;
	}
	else {
		//throw GraphErr("Graph cannot contain self-loops");
	}
}

template<typename NodeType, typename EdgeType>
EdgeType AdjacencyStructure<NodeType, EdgeType>::delete_redge(size_t first_node, size_t second_node)
{
	if (first_node < structure.size() && second_node < structure.size()) {
		try
		{
			EdgeType weight = structure[first_node].at(second_node);
			structure[first_node].erase(second_node);
			structure[second_node].erase(first_node);
			return weight;
		}
		catch (const std::exception&)
		{

		}
		
	}
	else {
		//throw GraphErr("Incorrect first or second node");
	}
}

template<typename NodeType, typename EdgeType>
void AdjacencyStructure<NodeType, EdgeType>::delete_node(size_t node)
{
	if (node < structure.size()) {
		this->Nodes.erase(node);
		structure.erase(structure.begin() + node);
		for (auto& i: structure) {
			i.erase(node);
		}
	}
	else {
		//throw GraphErr("Incorrect node");
	}
}

template<typename NodeType, typename EdgeType>
bool AdjacencyStructure<NodeType, EdgeType>::is_connected_graph()
{
	if (find_components().size() == 1) {
		return 1;
	}
	else {
		return 0;
	}
}

template<typename NodeType, typename EdgeType>
std::vector<std::vector<size_t>> AdjacencyStructure<NodeType, EdgeType>::find_components()
{
	std::vector<std::vector<size_t>>answer;
	std::vector<bool> for_check(structure.size(), 0);
	for (size_t i = 0; i < for_check.size(); i++) {
		if (for_check[i] == false) {
			std::vector<size_t>component;
			find_nodes(i, for_check, component);
			answer.push_back(std::move(component));
		}
	}
	return answer;
}

template<typename NodeType, typename EdgeType>
std::vector<EdgeType> AdjacencyStructure<NodeType, EdgeType>::dijkstra_algorithm(size_t node)
{
	std::vector <EdgeType> answer(structure.size(), this->max_edge);
	answer[node] = 0;
	std::vector <bool> checked(structure.size(), 0);
	EdgeType min_distance = this->zero_edge;
	size_t min_node = node;
	while (min_distance < INT_MAX)
	{
		size_t current = min_node;
		checked[current] = true;
		for (auto it = structure[current].begin(); it != structure[current].end(); it++) {
			if (answer[current] != this->max_edge && it->second != this->max_edge) {
				if (answer[current] + it->second < answer[structure[current][it->first]]) {
					answer[structure[current][it->first]] = answer[current] + it->second;
				}
			}
		}
		min_distance = this->max_edge;
		for (size_t i = 0; i < structure.size(); i++) {
			if (checked[i] == false && answer[i] < min_distance)
			{
				min_distance = answer[i];
				min_node = i;
			}
		}
	}
	return answer;
}

template<typename NodeType, typename EdgeType>
bool AdjacencyStructure<NodeType, EdgeType>::is_acyclic()
{
	std::vector<char>checked;
	for (size_t i = 0; i < structure.size(); i++) {
		checked.assign(structure.size(), 1);
		if (check_cyclic(i, checked, (size_t)LLONG_MAX)) {
			return false;
		}
	}
	return true;
}

template<typename NodeType, typename EdgeType>
bool AdjacencyStructure<NodeType, EdgeType>::is_tree()
{
	return is_acyclic() && is_connected_graph();
}

template<typename NodeType, typename EdgeType>
AdjacencyStructure<NodeType, EdgeType> AdjacencyStructure<NodeType, EdgeType>::minimum_spanning_tree_kruskal()
{
	if (is_connected_graph()) {
		std::vector<way_struct<EdgeType>>ways;
		AdjacencyStructure answer(structure.size());
		std::vector<size_t>components(structure.size());
		for (size_t i = 0; i < structure.size(); i++) {
			components[i] = i;
		}
		for (size_t i = 0; i < structure.size(); i++) {
			for (auto it = structure[i].begin(); it != structure[i].end(); it++) {
				if (i < it->first) {
					ways.push_back({ it->second ,i,it->first });
				}
			}
		}
		std::sort(ways.begin(), ways.end());
		for (size_t i = 0; i < ways.size(); i++) {
			if (components[ways[i].begin] != components[ways[i].end]) {
				answer.add_redge(ways[i].begin, ways[i].end, ways[i].weight);
				size_t second = components[ways[i].end];
				size_t first = components[ways[i].begin];
				for (size_t j = 0; j < components.size(); j++) {
					if (components[j] == second) {
						components[j] = first;
					}
				}
			}
		}
		return answer;
	}
	else {
		//throw GraphErr("This function only for weighted, undirected and connected graphs");
	}
}

template<typename NodeType, typename EdgeType>
AdjacencyStructure<NodeType, EdgeType> AdjacencyStructure<NodeType, EdgeType>::minimum_spanning_reverse_delete()
{
	if (this->is_connected_graph()) {
		AdjacencyStructure<NodeType, EdgeType> answer = *this;
		std::vector<way_struct<EdgeType>>edges;
		for (size_t i = 0; i < structure.size(); i++) {
			for (auto it = structure[i].begin(); it!= structure[i].end(); it++) {
				if (i < it->first) {
					edges.push_back({ it->second ,i, it->first });
				}
			}
		}
		std::sort(edges.begin(), edges.end());
		for (size_t i = edges.size() - 1; i > 0; i--) {
			EdgeType weight = answer.delete_redge(edges[i].begin, edges[i].end);
			if (!answer.is_connected_graph()) {
				answer.add_redge(edges[i].begin, edges[i].end, weight);
			}
		}
		return answer;
	}
	else {
		//throw GraphErr("This function only for weighted, undirected and connected graphs");
	}
}

template<typename NodeType, typename EdgeType>
void AdjacencyStructure<NodeType, EdgeType>::find_nodes(size_t node, std::vector<bool>& for_check, std::vector<size_t>& component)
{
	for_check[node] = 1;
	component.push_back(node);
	for (auto& i : structure[node]) {
		if (for_check[i.first] != 1) {
			find_nodes(i.first, for_check, component);
		}
	}
}

template<typename NodeType, typename EdgeType>
bool AdjacencyStructure<NodeType, EdgeType>::check_cyclic(size_t node, std::vector<char>& checked, size_t parent)
{
	checked[node] = 2;
	for (auto it = structure[node].begin(); it != structure[node].begin(); it++) {
		if (it->first != parent && checked[it->first] == 2) {
			return true;
		}
		else if (node != parent) {
			check_cyclic(it->first, checked, node);
		}
	}
	checked[node] = 3;
	return false;
}
