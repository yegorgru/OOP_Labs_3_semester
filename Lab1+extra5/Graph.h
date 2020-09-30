#pragma once
#include <map>
#include <vector>
#include <iostream>

template <typename NodeType, typename EdgeType>
class Graph
{
	std::map<size_t, NodeType>Nodes;
	EdgeType zero_edge;
	EdgeType max_edge;
public:
	Graph(EdgeType zero_edge, EdgeType max_edge);

    virtual size_t add_node(NodeType node);
	virtual void add_redge(size_t first_node, size_t second_node);

	virtual void delete_redge(size_t first_node, size_t second_node);
	virtual void delete_node(size_t node);

	virtual bool is_connected_graph();
	virtual std::vector<std::vector<size_t>> find_components();
private:
	virtual void find_nodes(size_t node, std::vector<bool>& for_check, std::vector<size_t>& component);
};

template<typename NodeType, typename EdgeType>
inline Graph<NodeType, EdgeType>::Graph(EdgeType zero_edge, EdgeType max_edge)
	:zero_edge(zero_edge),max_edge(max_edge) {}
