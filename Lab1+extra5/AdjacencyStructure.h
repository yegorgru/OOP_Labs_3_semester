/**
\file
\brief h file of class AdjacencyStructure

definitions and implementations of AdjacencyStructure methods and operators are here
*/

#pragma once

#include "Graph.h"
#include <vector>
#include <utility>
#include <map>
#include <algorithm>

/**
\brief Adjacency structure graph imlementation. Inheritor of Graph
*/

template <typename NodeType, typename EdgeType>
class AdjacencyStructure:public Graph<NodeType,EdgeType>
{
	/**
	\brief structure which consists of edges (EdgeType).

	structure[pos1][pos2] means edge from pos1 node to pos2 node. If edge doesn't exist, it isn't stored in the structure
	*/
	std::vector<std::map<size_t,EdgeType>>structure;
public:

	/**
	\brief AdjacencyMatrix constructor

	\param zero_edge All edges in graph must be bigger than zero_edge
	\param max_edge All edges in graph must be lesser than max_edge
	*/
	AdjacencyStructure(EdgeType zero_edge, EdgeType max_edge);

	/**
	\brief adds new node in graph

	\param node NodeType value of new node
	\return current number of node. For new node this is size of matrix - 1
	*/
	size_t add_node(NodeType node) override;

	/**
	\brief adds new edge in graph

	\param first_node number of node that will be begin of new edge
	\param second_node number of node that will be end of new edge
	\param edge EdgeType value of new edge
	\throw std::invalid_argument("incorrect node") if first_node == second_node or they are lesser than Nodes.size()
	*/
	void add_edge(size_t first_node, size_t second_node, EdgeType edge) override;

	/**
	\brief deletes edge from graph

	\param first_node number of node begin of deleted edge
	\param first_node number of node end of deleted edge
	\throw std::invalid_argument("incorrect node") if first_node == second_node or they are lesser than Nodes.size()
	\return EdgeType value of deleted edge
	*/
	EdgeType delete_edge(size_t first_node, size_t second_node) override;

	/**
	\brief deletes node from graph

	Removes node and all edges where this node was begin or end
	Numbers of all nodes after node will be decreased
	\param node number of deleted node
	\throw std::invalid_argument("incorrect node") if node is lesser than Nodes.size()
	\return NodeType value of deleted node
	*/
	NodeType delete_node(size_t node) override;

	/**
	\brief checks whether this graph is connected
	*/
	bool is_connected_graph() override;

	/**
	\brief finds all components of graph

	\return std::vector<std::vector<size_t>> where each std::vector<size_t> is numbers of nodes that are in one component
	*/
	std::vector<std::vector<size_t>> find_components() override;

	/**
	\brief finds minimal ways from one node to all others

	\param node number of basic node
	\throw std::invalid_argument("incorrect node") if node is lesser than Nodes.size()
	\return std::vector<EdgeType>ways, where ways[pos] means minimal way from basic node to node with number pos
	*/
	std::vector<EdgeType> dijkstra_algorithm(size_t node) override;

	/**
	\brief checks whether this graph is acyclic
	*/
	bool is_acyclic() override;

	/**
	\brief checks whether this graph is tree (connected and acyclic)
	*/
	bool is_tree() override;

	/**
	\brief access to node

	\param node number of searched node
	\throw std::invalid_argument("incorrect node") if node is lesser than Nodes.size()
	\return NodeType Node with number node
	*/
	NodeType get_node(size_t node) override;

	/**
	\brief access to node

	\param begin number of begin node of searched edge
	\param begin number of end node of searched edge
	\throw std::invalid_argument("incorrect node") if begin or end is lesser than Nodes.size()
	\return EdgeType Edge with such begin and end
	*/
	EdgeType get_edge(size_t begin, size_t end) override;

	/**
	\brief removes all edges from graph
	*/
	void clear_edges() override;

	/**
	\brief removes all nodes and edges from graph
	*/
	void clear() override;
private:
	/**
	\brief finds nodes that are in one component with node

	\param node number of basic node
	\param for_check vector reference to mark checked nodes
	\param component current component (component of node)
	*/
	void find_nodes(size_t node, std::vector<bool>& for_check, std::vector<size_t>& component) override;

	/**
	\brief checks if is there cycle with begin in node

	\param node number of ckecked node
	\param checked vector reference to mark checked nodes
	\param parent number of last node
	*/
	bool check_cyclic(size_t node, std::vector<char>& checked, size_t parent) override;

	friend std::ostream& operator<<(std::ostream& os, AdjacencyStructure<NodeType, EdgeType>);
};


/**
Operator << for AdjacencyStructure in streams
*/
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
	if (first_node < structure.size() && second_node < structure.size() && first_node != second_node) {
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
			for (auto it = i.upper_bound(node); it != i.end(); it++) {
				size_t tmp = it->first;
				i[it->first - 1] = i[it->first];
				i.erase(it->first);
				it = i.upper_bound(tmp);
			}
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
