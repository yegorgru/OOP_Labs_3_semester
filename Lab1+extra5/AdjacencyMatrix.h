#pragma once


#include "Graph.h"

#include <vector>
#include <map>
#include <utility>



template <typename NodeType, typename EdgeType>
class AdjacencyMatrix :public Graph<NodeType, EdgeType>
{
	std::vector<std::vector<EdgeType>>matrix;
public:

	AdjacencyMatrix(EdgeType zero_edge, EdgeType max_edge);
	size_t add_node(NodeType node) override;
	void add_redge(size_t first_node, size_t second_node, EdgeType edge) override;

	void delete_redge(size_t first_node, size_t second_node) override;
	void delete_node(size_t node) override;

	bool is_connected_graph() override;
	std::vector<std::vector<size_t>> find_components() override;
private:
	void find_nodes(size_t node, std::vector<bool>& for_check, std::vector<size_t>& component) override;
};

template<typename NodeType, typename EdgeType>
AdjacencyMatrix<NodeType, EdgeType>::AdjacencyMatrix(EdgeType zero_edge, EdgeType max_edge)
	: Graph(zero_edge,max_edge) {}

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
void AdjacencyMatrix<NodeType, EdgeType>::add_redge(size_t first_node, size_t second_node, EdgeType edge)
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
void AdjacencyMatrix<NodeType, EdgeType>::delete_redge(size_t first_node, size_t second_node)
{
	if (first_node < matrix.size() && second_node < matrix.size()) {
		matrix[first_node][second_node] = this->zero_edge;
		matrix[second_node][first_node] = this->zero_edge;
	}
	else {
		//throw GraphErr("Incorrect first or second node");
	}
}

template<typename NodeType, typename EdgeType>
void AdjacencyMatrix<NodeType, EdgeType>::delete_node(size_t node)
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
	std::vector<std::vector<size_t>> answer();
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
void AdjacencyMatrix<NodeType, EdgeType>::find_nodes(size_t node, std::vector<bool>& for_check, std::vector<size_t>& component)
{
	for_check[node] = 1;
	component.push_back(node);
	for (size_t i = 0; i < matrix[node].size(); i++) {
		if (matrix[node][i] != this->max_edge && node != i && for_check[i] != 1) {
			find_nodes(i, for_check, component);
		}
	}
}
