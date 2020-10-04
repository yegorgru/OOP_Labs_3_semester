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
	void add_edge(size_t first_node, size_t second_node, EdgeType edge) override;

	EdgeType delete_edge(size_t first_node, size_t second_node) override;
	NodeType delete_node(size_t node) override;

	bool is_connected_graph() override;
	std::vector<std::vector<size_t>> find_components() override;

	std::vector<EdgeType> dijkstra_algorithm(size_t node) override;

	bool is_acyclic() override;

	bool is_tree() override;

	NodeType get_node(size_t node) override;
	EdgeType get_edge(size_t begin, size_t end) override;

	void clear_edges() override;
	void clear() override;
private:
	void find_nodes(size_t node, std::vector<bool>& for_check, std::vector<size_t>& component) override;	
	bool check_cyclic(size_t node, std::vector<char>& checked, size_t parent) override;

	friend std::ostream& operator<<(std::ostream& os, AdjacencyStructure<NodeType, EdgeType>);
};

template <typename NodeType, typename EdgeType>
std::ostream& operator<<(std::ostream& os, AdjacencyStructure<NodeType, EdgeType>);

template<typename NodeType, typename EdgeType>
AdjacencyStructure<NodeType, EdgeType>::AdjacencyStructure(EdgeType zero_edge, EdgeType max_edge)
	: Graph<NodeType, EdgeType>(zero_edge, max_edge) {}

template<typename NodeType, typename EdgeType>
size_t AdjacencyStructure<NodeType, EdgeType>::add_node(NodeType node)
{
	this->Nodes[this->Nodes.size()] = node;
	structure.push_back(std::map<size_t, EdgeType >());
	return this->Nodes.size() - 1;
}

template<typename NodeType, typename EdgeType>
void AdjacencyStructure<NodeType, EdgeType>::add_edge(size_t first_node, size_t second_node, EdgeType edge)
{
	if (first_node != second_node) {
		structure[first_node][second_node] = edge;
		structure[second_node][first_node] = edge;
	}
	else {
		throw std::invalid_argument("incorrect node");
	}
}

template<typename NodeType, typename EdgeType>
EdgeType AdjacencyStructure<NodeType, EdgeType>::delete_edge(size_t first_node, size_t second_node)
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
			return this->max_edge;
		}
	}
	else {
		throw std::invalid_argument("incorrect node");
	}
}

template<typename NodeType, typename EdgeType>
NodeType AdjacencyStructure<NodeType, EdgeType>::delete_node(size_t node)
{
	if (node < structure.size()) {
		structure.erase(structure.begin() + node);
		for (auto& i: structure) {
			i.erase(node);
		}
		try
		{
			NodeType to_delete = this->Nodes.at(node);
			for (auto it = this->Nodes.find(node), next = this->Nodes.find(node + 1); next != this->Nodes.end(); it++, next++) {
				it->second = next->second;
			}
			auto tmp = this->Nodes.end();
			tmp--;
			this->Nodes.erase(tmp);
			return to_delete;
		}
		catch (const std::exception&)
		{
			return NodeType();
		}
	}
	else {
		throw std::invalid_argument("incorrect node");
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
	answer[node] = this->zero_edge;
	std::vector <bool> checked(structure.size(), 0);
	EdgeType min_distance = this->zero_edge;
	size_t min_node = node;
	while (min_distance < this->max_edge)
	{
		size_t current = min_node;
		checked[current] = true;
		for (auto it = structure[current].begin(); it != structure[current].end(); it++) {
			if (!(answer[current] == this->max_edge) && !(it->second == this->max_edge)) {
				if (answer[current] + it->second < answer[it->first]) {
					answer[it->first] = answer[current] + it->second;
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
NodeType AdjacencyStructure<NodeType, EdgeType>::get_node(size_t node)
{
	try
	{
		return this->Nodes.at(node);
	}
	catch (const std::exception&)
	{
		return NodeType();
	}
}

template<typename NodeType, typename EdgeType>
EdgeType AdjacencyStructure<NodeType, EdgeType>::get_edge(size_t begin, size_t end)
{
	if (end < structure.size() && begin < structure.size()) {
		try
		{
			return structure[begin].at(end);
		}
		catch (const std::exception&)
		{
			return this->max_edge;
		}
	}
	else {
		throw std::invalid_argument("incorrect node");
	}
}

template<typename NodeType, typename EdgeType>
void AdjacencyStructure<NodeType, EdgeType>::clear_edges()
{
	for (size_t i = 0; i < structure.size(); i++) {
		structure[i].clear();
	}
}

template<typename NodeType, typename EdgeType>
void AdjacencyStructure<NodeType, EdgeType>::clear()
{
	this->Nodes.clear();
	this->structure.clear();
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
	for (auto it = structure[node].begin(); it != structure[node].end(); it++) {
		if (it->first != parent && checked[it->first] == 2) {
			return true;
		}
		else if (it->first != parent) {
			check_cyclic(it->first, checked, node);
		}
	}
	checked[node] = 3;
	return false;
}

template<typename NodeType, typename EdgeType>
std::ostream& operator<<(std::ostream& os, AdjacencyStructure<NodeType, EdgeType> graph)
{
	os << "Nodes:" << std::endl;
	for (auto it = graph.Nodes.begin(); it != graph.Nodes.end(); it++) {
		os << it->first << " " << it->second << std::endl;
	}
	os << "Edges:" << std::endl;
	for (size_t i = 0; i != graph.matrix.size(); i++) {
		for (auto it = graph.matrix[i].begin(); it != graph.matrix[i].end(); it++) {
			os << i << " " << it->first << " " << it->second << std::endl;
		}
	}
	return os;
}
