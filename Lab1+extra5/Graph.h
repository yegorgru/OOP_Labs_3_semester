#pragma once
#include <map>
#include <vector>
#include <iostream>

template <typename T>
struct way_struct
{
	T weight;
	size_t begin;
	size_t end;
};

template <typename NodeType, typename EdgeType>
class Graph
{
	std::map<size_t, NodeType>Nodes;
	EdgeType zero_edge;
	EdgeType max_edge;
public:
	Graph(EdgeType zero_edge, EdgeType max_edge);

    virtual size_t add_node(NodeType node);
	virtual void add_redge(size_t first_node, size_t second_node, EdgeType edge);

	virtual EdgeType delete_redge(size_t first_node, size_t second_node);
	virtual void delete_node(size_t node);

	virtual bool is_connected_graph();
	virtual std::vector<std::vector<size_t>> find_components();

	virtual std::vector<EdgeType> dijkstra_algorithm(size_t node);

	virtual bool is_acyclic();

	virtual bool is_tree();

	virtual Graph<NodeType, EdgeType> minimum_spanning_tree_kruskal();
	virtual Graph<NodeType, EdgeType> minimum_spanning_reverse_delete();

private:
	virtual  bool check_cyclic(size_t node, std::vector<char>& checked, size_t parent);
	virtual void find_nodes(size_t node, std::vector<bool>& for_check, std::vector<size_t>& component);
};

template<typename NodeType, typename EdgeType>
inline Graph<NodeType, EdgeType>::Graph(EdgeType zero_edge, EdgeType max_edge)
	:zero_edge(zero_edge),max_edge(max_edge) {}
