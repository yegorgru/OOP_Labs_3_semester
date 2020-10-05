/**
\file
\brief h file of class Graph (basic class for graph)

definitions and implementations of Graph methods are here
*/


#pragma once
#include <map>
#include <vector>
#include <iostream>

/**
\brief Basic class of graph imlementations.

For documentation of methods check AdjacencyMatrix or AdjacencyStructure
*/

template <typename NodeType, typename EdgeType>
class Graph
{
public:
	/**
	\brief Graph constructor

	\param zero_edge All edges in graph must be bigger than zero_edge
	\param max_edge All edges in graph must be lesser than max_edge
	*/
	Graph(EdgeType zero_edge, EdgeType max_edge);

    virtual size_t add_node(NodeType node);
	virtual void add_edge(size_t first_node, size_t second_node, EdgeType edge);

	virtual EdgeType delete_edge(size_t first_node, size_t second_node);
	virtual NodeType delete_node(size_t node);

	virtual bool is_connected_graph();
	virtual std::vector<std::vector<size_t>> find_components();

	virtual std::vector<EdgeType> dijkstra_algorithm(size_t node);

	virtual bool is_acyclic();

	virtual bool is_tree();

	virtual NodeType get_node(size_t node);

	virtual EdgeType get_edge(size_t begin, size_t end);

	EdgeType get_max_edge();
	EdgeType get_zero_edge();
	virtual void clear_edges();

	virtual void clear();

protected:
	virtual  bool check_cyclic(size_t node, std::vector<char>& checked, size_t parent);
	virtual void find_nodes(size_t node, std::vector<bool>& for_check, std::vector<size_t>& component);

	/**
	\brief structure which consists of Nodes (NodeType).

	Nodes[pos] means node with number pos
	*/
	std::map<size_t, NodeType>Nodes;
	EdgeType zero_edge;	///<All edges in graph must be bigger than zero_edge
	EdgeType max_edge;	///<All edges in graph must be lesser than max_edge
};

template<typename NodeType, typename EdgeType>
Graph<NodeType, EdgeType>::Graph(EdgeType zero_edge, EdgeType max_edge)
	:zero_edge(zero_edge),max_edge(max_edge) {}

template<typename NodeType, typename EdgeType>
size_t Graph<NodeType, EdgeType>::add_node(NodeType node)
{
	return 0;
}

template<typename NodeType, typename EdgeType>
inline void Graph<NodeType, EdgeType>::add_edge(size_t first_node, size_t second_node, EdgeType edge)
{
}

template<typename NodeType, typename EdgeType>
inline EdgeType Graph<NodeType, EdgeType>::delete_edge(size_t first_node, size_t second_node)
{
	return EdgeType();
}

template<typename NodeType, typename EdgeType>
inline NodeType Graph<NodeType, EdgeType>::delete_node(size_t node)
{
	return NodeType();
}

template<typename NodeType, typename EdgeType>
inline bool Graph<NodeType, EdgeType>::is_connected_graph()
{
	return false;
}

template<typename NodeType, typename EdgeType>
inline std::vector<std::vector<size_t>> Graph<NodeType, EdgeType>::find_components()
{
	return std::vector<std::vector<size_t>>();
}

template<typename NodeType, typename EdgeType>
inline std::vector<EdgeType> Graph<NodeType, EdgeType>::dijkstra_algorithm(size_t node)
{
	return std::vector<EdgeType>();
}

template<typename NodeType, typename EdgeType>
inline bool Graph<NodeType, EdgeType>::is_acyclic()
{
	return false;
}

template<typename NodeType, typename EdgeType>
inline bool Graph<NodeType, EdgeType>::is_tree()
{
	return false;
}

template<typename NodeType, typename EdgeType>
inline NodeType Graph<NodeType, EdgeType>::get_node(size_t node)
{
	return NodeType();
}

template<typename NodeType, typename EdgeType>
inline EdgeType Graph<NodeType, EdgeType>::get_edge(size_t begin, size_t end)
{
	return EdgeType();
}

template<typename NodeType, typename EdgeType>
EdgeType Graph<NodeType, EdgeType>::get_max_edge()
{
	return this->max_edge;
}

template<typename NodeType, typename EdgeType>
EdgeType Graph<NodeType, EdgeType>::get_zero_edge()
{
	return this->zero_edge;
}

template<typename NodeType, typename EdgeType>
void Graph<NodeType, EdgeType>::clear_edges()
{
}

template<typename NodeType, typename EdgeType>
inline void Graph<NodeType, EdgeType>::clear()
{
}

template<typename NodeType, typename EdgeType>
inline bool Graph<NodeType, EdgeType>::check_cyclic(size_t node, std::vector<char>& checked, size_t parent)
{
	return false;
}

template<typename NodeType, typename EdgeType>
inline void Graph<NodeType, EdgeType>::find_nodes(size_t node, std::vector<bool>& for_check, std::vector<size_t>& component)
{
}
