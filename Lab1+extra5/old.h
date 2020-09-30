#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <Windows.h>
#include <conio.h>
#include <algorithm>
#include <stack>
#include <queue>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>

extern std::mt19937 mersenne;
extern std::string help_answer;

class GraphErr :public std::logic_error
{
public:
	explicit GraphErr(const char* message) :logic_error(message) {}
};

struct node_struct {
	int weight;
	size_t node;
};

struct way_struct
{
	int weight;
	size_t begin;
	size_t end;
};

struct edge_struct {
	size_t begin;
	size_t end;
};


class adjacencyStructure;

class adjacencyMatrix {

	std::vector<std::vector<int>>matrix;

	bool weighted;
	bool directed;
	bool structure;
public:

	bool what_structure() {
		return structure;
	}

	adjacencyMatrix(bool weighted, bool directed) {
		this->weighted = weighted;
		this->directed = directed;
		this->structure = 0;
	}

	adjacencyMatrix(bool weighted, bool directed, size_t number_of_nodes, size_t number_of_edges);

	adjacencyMatrix(adjacencyStructure& another);

	void add_redge(size_t first_node, size_t second_node);
	void add_redge(size_t first_node, size_t second_node, int weight);
	void add_node();
	void show();
	void show_structure();

	int delete_redge(size_t first_node, size_t second_node);
	void delete_node(size_t node);

	size_t get_number_of_nodes() const
	{
		return matrix.size();
	}

	int get_number_of_edges() const {
		int counter = 0;
		for (size_t i = 0; i < matrix.size(); i++) {
			for (size_t j = 0; j < matrix.size(); j++) {
				if (matrix[i][j] != INT_MAX && i != j) {
					counter++;
				}
			}
		}
		if (!directed) {
			counter /= 2;
		}
		return counter;
	}

	int get_weight() {
		int answer = 0;
		for (auto& i : matrix) {
			for (auto j : i) {
				if (j != INT_MAX) {
					answer += j;
				}
			}
		}
		if (!directed) {
			answer /= 2;
		}
		return answer;
	}

	bool is_weighted() const
	{
		return this->weighted;
	}

	bool is_directed() const
	{
		if (this->directed) {
			return true;
		}
		else {
			return false;
		}
	}

	adjacencyStructure get_adjacencyStructure();

	bool is_acyclic();
	bool check_cyclic(size_t node, std::vector<char>& checked, size_t parent);

	std::string is_connected_graph();

	bool is_tree() {
		std::string connected_check = this->is_connected_graph();
		if (this->is_acyclic() && (connected_check == "strongly connected graph" || connected_check == "connected graph")) {
			return true;
		}
		else {
			return false;
		}
	}

	void find_nodes(size_t node, std::vector<bool>& for_check, std::vector<size_t>& component);
	void find_components(std::vector<std::vector<size_t>>& for_components);
	void find_strong_components(std::vector<std::vector<size_t>>& for_components);

	std::vector<int> dijkstra_algorithm(size_t node);

	adjacencyMatrix build_reverse() const;

	std::vector<std::vector<int>>floyd_algorithm();

	std::vector<int>bellman_ford_algorithm(size_t node);

	std::vector<size_t> depth_first(bool important_weight);
	void depth_first(size_t node, std::vector<bool>& checked, bool important_weight, std::vector<size_t>& order);

	adjacencyMatrix depth_first_spanning_tree(bool important_weight);
	adjacencyMatrix minimum_spanning_tree_kruskal();
	adjacencyMatrix minimum_spanning_reverse_delete();

	std::vector<size_t> topological_sorting_kahn();
	std::vector<size_t> topological_sorting_depth_first() {
		if (directed && is_acyclic()) {
			return depth_first(0);
		}
		else {
			throw GraphErr("Topological sorting is possible only for directed and acyclic graphs");
		}
	}

	friend adjacencyStructure;
	friend void dfs1(const adjacencyMatrix& graph, size_t v, std::vector<bool>& used, std::vector<size_t>& order);
	friend void dfs2(const adjacencyMatrix& graph, size_t v, std::vector<bool>& used, std::vector<size_t>& order, std::vector<size_t>& component);
};

class adjacencyStructure {
	std::vector<std::vector<size_t>>nodes;

	std::vector<std::vector<int>>weights;

	bool weighted;
	bool directed;
	bool structure;
public:
	bool what_structure() {
		return structure;
	}

	adjacencyStructure(bool weighted, bool directed) {
		this->weighted = weighted;
		this->directed = directed;
		this->structure = 1;
	}

	adjacencyStructure(bool weighted, bool directed, size_t number_of_nodes, size_t number_of_edges);

	adjacencyStructure(const adjacencyMatrix& another);

	bool find(size_t first_node, size_t second_node) const;

	void add_redge(size_t first_node, size_t second_node);
	void add_redge(size_t first_node, size_t second_node, int weight);
	void add_node();
	void show();
	void show_structure();

	int delete_redge(size_t first_node, size_t second_node);
	void delete_node(size_t node);

	size_t get_number_of_nodes() const
	{
		return nodes.size();
	}

	int get_number_of_edges() const {
		int counter = 0;
		for (size_t i = 0; i < nodes.size(); i++) {
			counter += nodes[i].size();
		}
		if (!directed) {
			counter /= 2;
		}
		return counter;
	}

	int get_weight() {
		int answer = 0;
		if (!weighted) {
			return get_number_of_edges();
		}
		else {
			for (auto& i : weights) {
				for (auto j : i) {
					answer += j;
				}
			}
		}
		if (!directed) {
			answer /= 2;
		}
		return answer;
	}

	bool is_weighted() const
	{
		return this->weighted;
	}

	bool is_directed() const
	{
		if (directed) {
			return true;
		}
		else {
			return false;
		}
	}

	adjacencyMatrix get_adjacencyMatrix() {
		return adjacencyMatrix(*this);
	}

	bool is_acyclic();
	bool check_cyclic(size_t node, std::vector<char>& checked, size_t parent);

	bool is_tree() {
		std::string connected_check = this->is_connected_graph();
		if (this->is_acyclic() && (connected_check == "strongly connected graph" || connected_check == "connected graph")) {
			return true;
		}
		else {
			return false;
		}
	}

	std::string is_connected_graph();

	void find_nodes(size_t node, std::vector<bool>& for_check, std::vector<size_t>& component);
	void find_components(std::vector<std::vector<size_t>>& for_components);
	void find_strong_components(std::vector<std::vector<size_t>>& for_components);

	adjacencyStructure build_reverse() const;

	std::vector<int> dijkstra_algorithm(size_t node);

	std::vector<std::vector<int>>floyd_algorithm();
	std::vector<int>bellman_ford_algorithm(size_t node);

	std::vector<size_t> depth_first(bool important_weight);
	void depth_first(size_t node, std::vector<bool>& checked, bool important_weight, std::vector<size_t>& order);

	adjacencyStructure depth_first_spanning_tree(bool important_weight);
	adjacencyStructure minimum_spanning_tree_kruskal();
	adjacencyStructure minimum_spanning_reverse_delete();

	std::vector<size_t> topological_sorting_kahn();
	std::vector<size_t> topological_sorting_depth_first() {
		if (directed && is_acyclic()) {
			return depth_first(0);
		}
		else {
			throw GraphErr("Topological sorting is possible only for directed and acyclic graphs");
		}
	}

	friend adjacencyMatrix;
	friend void dfs1(const adjacencyStructure& graph, size_t v, std::vector<bool>& used, std::vector<size_t>& order);
	friend void dfs2(const adjacencyStructure& graph, size_t v, std::vector<bool>& used, std::vector<size_t>& order, std::vector<size_t>& component);
};

bool is_int(const std::string& str);

template<typename T>
void insertion_sort(std::vector<T>& Array, size_t begin, size_t end)
{
	for (size_t i = begin + 1; i < end + 1; i++) {
		T key = Array[i];
		size_t j = i;
		while (j > begin && Array[j - 1].weight > key.weight) {
			Array[j] = Array[j - 1];
			j--;
		}
		Array[j] = key;
	}
}

template <typename T>
size_t partition_quicksort(std::vector<T>& Array, size_t begin, size_t end) {
	begin++;
	end++;
	T pivot = Array[begin - 1 + mersenne() % (end - begin)];
	size_t i = begin - 1;
	size_t j = end + 1;
	while (true) {
		do {
			i++;
		} while (Array[i - 1].weight < pivot.weight);
		do {
			j--;
		} while (Array[j - 1].weight > pivot.weight);
		if (i >= j) {
			return j - 1;
		}
		T help = Array[i - 1];
		Array[i - 1] = Array[j - 1];
		Array[j - 1] = help;
	}
}

template <typename T>
void quicksort(std::vector<T>& Array, size_t begin, size_t end)
{
	if (begin < end) {
		if ((end - begin) < 30) {
			insertion_sort(Array, begin, end);
			return;
		}
		size_t p = partition_quicksort(Array, begin, end);
		quicksort(Array, begin, p);
		quicksort(Array, p + 1, end);
	}
}
