#pragma once


#include "Graph.h"

#include <vector>
#include <map>
#include <utility>
#include <algorithm>


template <typename NodeType, typename EdgeType>
class AdjacencyMatrix :public Graph<NodeType, EdgeType>
{
	std::vector<std::vector<EdgeType>>matrix;
public:

	AdjacencyMatrix(EdgeType zero_edge, EdgeType max_edge);

	size_t add_node(NodeType node) override;
	void add_edge(size_t first_node, size_t second_node, EdgeType edge) override;

	EdgeType delete_edge(size_t first_node, size_t second_node) override;
	NodeType delete_node(size_t node) override;

	bool is_connected_graph() override;
	std::vector<std::vector<size_t>> find_components() override;

	std::vector<EdgeType> dijkstra_algorithm(size_t node) override;

	bool is_acyclic() override;

	bool is_tree() override;

	Graph<NodeType, EdgeType> minimum_spanning_tree_kruskal() override;
	Graph<NodeType, EdgeType> minimum_spanning_reverse_delete() override;

	NodeType get_node(size_t node) override;
	EdgeType get_edge(size_t begin, size_t end) override;

	void clear_edges() override;

	void clear() override;
private:
	void find_nodes(size_t node, std::vector<bool>& for_check, std::vector<size_t>& component) override;
	bool check_cyclic(size_t node, std::vector<char>& checked, size_t parent) override;
};

template<typename NodeType, typename EdgeType>
AdjacencyMatrix<NodeType, EdgeType>::AdjacencyMatrix(EdgeType zero_edge, EdgeType max_edge)
	: Graph<NodeType, EdgeType>(zero_edge,max_edge) {}

template<typename NodeType, typename EdgeType>
size_t AdjacencyMatrix<NodeType, EdgeType>::add_node(NodeType node)
{
	this->Nodes[this->Nodes.size()] = node;
	for (auto& i : matrix) {
		i.resize(matrix.size() + 1, this->max_edge);
	}
	std::vector<EdgeType>to_add(matrix.size(), this->max_edge);
	to_add.push_back(this->zero_edge);
	matrix.push_back(to_add);
	return this->Nodes.size()-1;
}

template<typename NodeType, typename EdgeType>
void AdjacencyMatrix<NodeType, EdgeType>::add_edge(size_t first_node, size_t second_node, EdgeType edge)
{
	if (first_node < matrix.size() && second_node < matrix.size()) {
		if (first_node != second_node) {
			matrix[first_node][second_node] = edge;
			/*if (matrix[first_node][second_node] == INT_MAX) {
				matrix[first_node][second_node] = weight;
				if (!this->directed) {
					matrix[second_node][first_node] = weight;
				}
			}
			else {
				throw GraphErr("Edge with those nodes already exists");
			}*/
		}
		else {
			//throw GraphErr("Graph cannot contain self-loops");
		}
	}
	else {
		//throw GraphErr("Incorrect first or second node");
	}
}

template<typename NodeType, typename EdgeType>
EdgeType AdjacencyMatrix<NodeType, EdgeType>::delete_edge(size_t first_node, size_t second_node)
{
	if (first_node < matrix.size() && second_node < matrix.size()) {
		EdgeType weight = matrix[first_node][second_node];
		matrix[first_node][second_node] = this->max_edge;
		matrix[second_node][first_node] = this->max_edge;
		return weight;
	}
	else {
		//throw GraphErr("Incorrect first or second node");
	}
}

template<typename NodeType, typename EdgeType>
NodeType AdjacencyMatrix<NodeType, EdgeType>::delete_node(size_t node)
{
	if (node < matrix.size()) {
		for (size_t i = node; i < matrix.size() - 1; i++) {
			for (size_t j = 0; j < matrix.size(); j++) {
				matrix[i][j] = matrix[i + 1][j];
			}
		}
		for (size_t i = node; i < matrix.size() - 1; i++) {
			for (size_t j = 0; j < matrix.size(); j++) {
				matrix[j][i] = matrix[j][i + 1];
			}
		}
		matrix.resize(matrix.size() - 1);
		for (size_t i = 0; i < matrix.size(); i++) {
			matrix[i].resize(matrix.size());
		}
		try
		{
			NodeType to_delete = this->Nodes.at(node);
			for (auto it = this->Nodes.find(node),  next = this->Nodes.find(node + 1); next != this->Nodes.end(); it++, next++) {
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
		//throw GraphErr("Incorrect node");
	}
}

template<typename NodeType, typename EdgeType>
bool AdjacencyMatrix<NodeType, EdgeType>::is_connected_graph()
{
	if (find_components().size() == 1) {
		return 1;
	}
	else {
		return 0;
	}
}

template<typename NodeType, typename EdgeType>
std::vector<std::vector<size_t>> AdjacencyMatrix<NodeType, EdgeType>::find_components()
{
	std::vector<std::vector<size_t>> answer;
	std::vector<bool> for_check(matrix.size(), 0);
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
std::vector<EdgeType> AdjacencyMatrix<NodeType, EdgeType>::dijkstra_algorithm(size_t node)
{
	std::vector <EdgeType> answer(matrix.size(), this->max_edge);
	answer[node] = this->zero_edge;
	std::vector <bool> checked(matrix.size(), 0);
	EdgeType min_distance = this->zero_edge;
	size_t min_node = node;
	while (min_distance < this->max_edge)
	{
		size_t current = min_node;
		checked[current] = true;
		for (size_t i = 0; i < matrix.size(); i++) {
			if (current != i && !(answer[current] == this->max_edge) && !(matrix[current][i] == this->max_edge)) {
				if (answer[current] + matrix[current][i] < answer[i]) {
					answer[i] = answer[current] + matrix[current][i];
				}
			}
		}
		min_distance = this->max_edge;
		for (size_t i = 0; i < matrix.size(); i++) {
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
bool AdjacencyMatrix<NodeType, EdgeType>::is_acyclic()
{
	std::vector<char>checked;
	for (size_t i = 0; i < matrix.size(); i++) {
		checked.assign(matrix.size(), 1);
		if (check_cyclic(i, checked, (size_t)LLONG_MAX)) {
			return false;
		}
	}
	return true;
}

template<typename NodeType, typename EdgeType>
bool AdjacencyMatrix<NodeType, EdgeType>::is_tree()
{
	return is_acyclic() && is_connected_graph();
}

template<typename NodeType, typename EdgeType>
Graph<NodeType, EdgeType> AdjacencyMatrix<NodeType, EdgeType>::minimum_spanning_tree_kruskal()
{
	if (is_connected_graph()) {
		std::vector<way_struct<EdgeType>>ways;
		AdjacencyMatrix<NodeType, EdgeType> answer = *this;
		answer.clear_edges();
		std::vector<size_t>components(matrix.size());
		for (size_t i = 0; i < matrix.size(); i++) {
			components[i] = i;
		}
		for (size_t i = 0; i < matrix.size(); i++) {
			for (size_t j = i + 1; j < matrix.size(); j++) {
				if (!(matrix[i][j] == this->max_edge)) {
					ways.push_back({ matrix[i][j] ,i,j });
				}
			}
		}
		std::sort(ways.begin(), ways.end());
		for (size_t i = 0; i < ways.size(); i++) {
			if (components[ways[i].begin] != components[ways[i].end]) {
				answer.add_edge(ways[i].begin, ways[i].end, ways[i].weight);
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
Graph<NodeType, EdgeType> AdjacencyMatrix<NodeType, EdgeType>::minimum_spanning_reverse_delete()
{
	if (is_connected_graph()) {
		AdjacencyMatrix answer = *this;
		std::vector<way_struct<EdgeType>>edges;
		for (size_t i = 0; i < matrix.size(); i++) {
			for (size_t j = i + 1; j < matrix.size(); j++) {
				if (!(matrix[i][j] == this->max_edge)) {
					edges.push_back({ matrix[i][j] ,i,j });
				}
			}
		}
		std::sort(edges.begin(), edges.end());
		for (size_t i = edges.size() - 1; i > 0; i--) {
			EdgeType weight = answer.delete_edge(edges[i].begin, edges[i].end);
			if (!answer.is_connected_graph()) {
				answer.add_edge(edges[i].begin, edges[i].end, weight);
			}
		}
		return answer;
	}
	else {
		//throw GraphErr("This function only for weighted, undirected and connected graphs");
	}
}

template<typename NodeType, typename EdgeType>
NodeType AdjacencyMatrix<NodeType, EdgeType>::get_node(size_t node)
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
EdgeType AdjacencyMatrix<NodeType, EdgeType>::get_edge(size_t begin, size_t end)
{
	if (end < matrix.size() && begin < matrix.size()) {
		return matrix[begin][end];
	}
	else {
		//throw
	}
}

template<typename NodeType, typename EdgeType>
void AdjacencyMatrix<NodeType, EdgeType>::clear_edges()
{
	for (size_t i = 0; i < matrix.size(); i++) {
		for (size_t j = 0; j < matrix.size(); j++) {
			matrix[i][j] = this->max_edge;
		}
	}
}

template<typename NodeType, typename EdgeType>
void AdjacencyMatrix<NodeType, EdgeType>::clear()
{
	this->Nodes.clear();
	this->matrix.clear();
}

template<typename NodeType, typename EdgeType>
void AdjacencyMatrix<NodeType, EdgeType>::find_nodes(size_t node, std::vector<bool>& for_check, std::vector<size_t>& component)
{
	for_check[node] = 1;
	component.push_back(node);
	for (size_t i = 0; i < matrix[node].size(); i++) {
		if (!(matrix[node][i] == this->max_edge) && node != i && for_check[i] != 1) {
			find_nodes(i, for_check, component);
		}
	}
}

template<typename NodeType, typename EdgeType>
bool AdjacencyMatrix<NodeType, EdgeType>::check_cyclic(size_t node, std::vector<char>& checked, size_t parent)
{
	checked[node] = 2;
	for (size_t i = 0; i < matrix.size(); i++) {
		if (!(matrix[node][i] == this->max_edge) && node != i && i != parent && checked[i] == 2) {
			return true;
		}
		else if (!(matrix[node][i] == this->max_edge) && node != i && i != parent) {
			check_cyclic(i, checked, node);
		}
	}
	checked[node] = 3;
	return false;
}
