#pragma once

#include "Graph.h"
#include <vector>
#include <utility>
#include <map>

template <typename NodeType, typename EdgeType>
class AdjacencyStructure:public Graph<NodeType,EdgeType>
{
	std::vector<std::map<size_t,EdgeType>>structure;
public:
	AdjacencyStructure(EdgeType zero_edge, EdgeType max_edge);
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
void AdjacencyStructure<NodeType, EdgeType>::delete_redge(size_t first_node, size_t second_node)
{
	if (first_node < structure.size() && second_node < structure.size()) {
		structure[first_node].erase(second_node);
		structure[second_node].erase(first_node);
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
