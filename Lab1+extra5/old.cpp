#include "old.h"

adjacencyMatrix::adjacencyMatrix(bool weighted, bool directed, size_t number_of_nodes, size_t number_of_edges)
{
	this->weighted = weighted;
	this->directed = directed;
	this->structure = 0;
	if ((directed && number_of_edges <= number_of_nodes * (number_of_nodes - 1)) || (!directed && number_of_edges <= (number_of_nodes * (number_of_nodes - 1)) / 2)) {
		matrix.assign(number_of_nodes, std::vector<int>(number_of_nodes, 11));
		long long copy_number_edges = number_of_edges;
		if ((!directed && number_of_edges < (number_of_nodes) * (number_of_nodes - 1) / 4) || (directed && number_of_edges < (number_of_nodes) * (number_of_nodes - 1) / 2)) {
			for (size_t i = 0; i < copy_number_edges; i++) {
				size_t first_node = mersenne() % number_of_nodes;
				size_t second_node = mersenne() % number_of_nodes;
				if ((!directed && matrix[second_node][first_node] != 11) || first_node == second_node || matrix[first_node][second_node] != 11) {
					copy_number_edges++;
					continue;
				}
				else {
					if (!weighted) {
						matrix[first_node][second_node] = 1;
					}
					else {
						matrix[first_node][second_node] = 1 + mersenne() % 10;
					}
					if (!directed) {
						matrix[second_node][first_node] = matrix[first_node][second_node];
					}
				}
			}
			for (size_t i = 0; i < number_of_nodes; i++) {
				for (size_t j = 0; j < number_of_nodes; j++) {
					if (matrix[i][j] == 11) {
						matrix[i][j] = INT_MAX;
					}
				}
			}
		}
		else {
			for (size_t i = 0; i < number_of_nodes; i++) {
				matrix[i][i] = 0;
			}
			size_t limit;
			if (directed) {
				limit = number_of_nodes * (number_of_nodes - 1);
			}
			else {
				limit = number_of_nodes * (number_of_nodes - 1) / 2;
			}
			for (size_t i = 0; i < limit - copy_number_edges; i++) {
				size_t first_node = mersenne() % number_of_nodes;
				size_t second_node = mersenne() % number_of_nodes;
				if (matrix[first_node][second_node] != 11 || (!directed && matrix[second_node][first_node] != 11) || first_node == second_node) {
					copy_number_edges--;
					continue;
				}
				else {
					matrix[first_node][second_node] = INT_MAX;
					if (!directed) {
						matrix[second_node][first_node] = INT_MAX;
					}
				}
			}
			for (size_t i = 0; i < number_of_nodes; i++) {
				for (size_t j = 0; j < number_of_nodes; j++) {
					if (matrix[i][j] == 11) {
						if (weighted) {
							matrix[i][j] = 1 + mersenne() % 10;
						}
						else {
							matrix[i][j] = 1;
						}
						if (!directed) {
							matrix[j][i] = matrix[i][j];
						}
					}
				}
			}
		}
	}
	else {
		throw GraphErr("Impossible number of edges for number of nodes");
	}
}

adjacencyMatrix::adjacencyMatrix(adjacencyStructure& another)
{
	this->directed = another.directed;
	this->weighted = another.weighted;
	this->structure = 0;
	matrix.assign(another.nodes.size(), std::vector<int>(another.nodes.size(), INT_MAX));
	for (size_t i = 0; i < matrix.size(); i++) {
		matrix[i][i] = 0;
	}
	for (size_t i = 0; i < another.nodes.size(); i++) {
		for (size_t j = 0; j < another.nodes[i].size(); j++) {
			if (weighted) {
				matrix[i][another.nodes[i][j]] = another.weights[i][j];
			}
			else {
				matrix[i][another.nodes[i][j]] = 1;
			}
		}
	}
}

void adjacencyMatrix::add_redge(size_t first_node, size_t second_node)
{
	if (first_node < matrix.size() && second_node < matrix.size()) {
		if (first_node != second_node) {
			if (!(this->weighted)) {
				if (matrix[first_node][second_node] == INT_MAX) {
					matrix[first_node][second_node] = 1;
					if (!this->directed) {
						matrix[second_node][first_node] = 1;
					}
				}
				else {
					throw GraphErr("Edge with those nodes already exists");
				}
			}
			else {
				throw GraphErr("This type of function only for unweighted graphs");
			}
		}
		else {
			throw GraphErr("Graph cannot contain self-loops");
		}
	}
	else {
		throw GraphErr("Incorrect first or second node");
	}
}

void adjacencyMatrix::add_redge(size_t first_node, size_t second_node, int weight)
{
	if (first_node < matrix.size() && second_node < matrix.size()) {
		if (first_node != second_node) {
			if (this->weighted) {
				if (matrix[first_node][second_node] == INT_MAX) {
					matrix[first_node][second_node] = weight;
					if (!this->directed) {
						matrix[second_node][first_node] = weight;
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
	}
}

void adjacencyMatrix::add_node()
{
	for (auto& i : matrix) {
		i.resize(matrix.size() + 1, INT_MAX);
	}
	std::vector<int>to_add(matrix.size(), INT_MAX);
	to_add.push_back(0);
	matrix.push_back(to_add);
}

int adjacencyMatrix::delete_redge(size_t first_node, size_t second_node)
{
	if (first_node < matrix.size() && second_node < matrix.size()) {
		if (first_node != second_node) {
			if (matrix[first_node][second_node] != INT_MAX) {
				int weight = matrix[first_node][second_node];
				matrix[first_node][second_node] = INT_MAX;
				if (!directed) {
					matrix[second_node][first_node] = INT_MAX;
				}
				return weight;
			}
			else {
				throw GraphErr("This edge doesn't exist");
			}
		}
		else {
			throw GraphErr("Graph cannot contain self-loops");
		}
	}
	else {
		throw GraphErr("Incorrect first or second node");
	}
}

void adjacencyMatrix::delete_node(size_t node)
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
		throw GraphErr("Incorrect node");
	}
}

void adjacencyMatrix::show()
{
	bool one_edge;
	bool many_edges;
	for (size_t i = 0; i < matrix.size(); i++) {
		one_edge = false;
		many_edges = false;
		std::cout << "Node " << i;
		for (size_t j = 0; j < matrix.size(); j++) {
			if (!one_edge && matrix[i][j] != INT_MAX && i != j) {
				std::cout << " linked with " << j;
				if (weighted) {
					std::cout << '(' << matrix[i][j] << ')';
				}
				one_edge = true;
			}
			else if (many_edges && matrix[i][j] != INT_MAX && i != j) {
				std::cout << " " << j;
				if (weighted) {
					std::cout << '(' << matrix[i][j] << ')';
				}
			}
			else if (one_edge && matrix[i][j] != INT_MAX && i != j) {
				std::cout << " " << j;
				if (weighted) {
					std::cout << '(' << matrix[i][j] << ')';
				}
				many_edges = true;
			}
		}
		if (many_edges) {
			std::cout << " nodes";
		}
		else if (one_edge) {
			std::cout << " node";
		}
		else {
			std::cout << " is not linked with any other";
		}
		std::cout << std::endl;
	}
}

void adjacencyMatrix::show_structure()
{
	if (matrix.size() > 0) {
		std::cout << "Nodes";
		for (size_t i = 0; i < matrix.size(); i++) {
			std::cout << '\t' << i;
		}
		std::cout << std::endl;
		for (size_t i = 0; i < matrix.size(); i++) {
			std::cout << i;
			for (size_t j = 0; j < matrix.size(); j++) {
				if (matrix[i][j] != INT_MAX) {
					std::cout << '\t' << matrix[i][j];
				}
				else {
					std::cout << "\t-";
				}
			}
			std::cout << std::endl;
		}
	}
	else {
		std::cout << "Nothing" << std::endl;
	}
}

std::string adjacencyMatrix::is_connected_graph()
{
	std::vector<std::vector<size_t>> for_components;
	if (directed) {
		find_strong_components(for_components);
	}
	else {
		find_components(for_components);
	}
	if (for_components.size() == 1) {
		if (directed) {
			return "strongly connected graph";
		}
		else {
			return "connected graph";
		}
	}
	else if (directed) {
		adjacencyMatrix new_graph(0, 0, matrix.size(), 0);
		for (size_t i = 0; i < matrix.size(); i++) {
			for (size_t j = i + 1; j < matrix.size(); j++) {
				if (new_graph.matrix[i][j] != INT_MAX) {
					new_graph.add_redge(i, j);
				}
			}
		}
		for_components.clear();
		new_graph.find_components(for_components);
		if (for_components.size() == 1) {
			return "weakly connected graph";
		}
	}
	return "disconnected graph";
}

void adjacencyMatrix::find_nodes(size_t node, std::vector<bool>& for_check, std::vector<size_t>& component)
{
	for_check[node] = 1;
	component.push_back(node);
	for (size_t i = 0; i < matrix[node].size(); i++) {
		if (matrix[node][i] != INT_MAX && node != i && for_check[i] != 1) {
			find_nodes(i, for_check, component);
		}
	}
}

void adjacencyMatrix::find_components(std::vector<std::vector<size_t>>& for_components)
{
	if (!directed) {
		std::vector<bool> for_check(matrix.size(), 0);
		for_components.clear();
		for (size_t i = 0; i < for_check.size(); i++) {
			if (for_check[i] == false) {
				std::vector<size_t>component;
				find_nodes(i, for_check, component);
				for_components.push_back(component);
			}
		}
	}
	else {
		throw GraphErr("This function only for undirected graphs");
	}
}

adjacencyMatrix adjacencyMatrix::build_reverse()const
{
	if (directed) {
		adjacencyMatrix reverse(weighted, directed);
		for (size_t i = 0; i < matrix.size(); i++) {
			reverse.add_node();
		}
		for (size_t i = 0; i < matrix.size(); i++) {
			for (size_t j = 0; j < matrix.size(); j++) {
				if (matrix[i][j] != INT_MAX && i != j) {
					if (weighted) {
						reverse.add_redge(j, i, matrix[i][j]);
					}
					else {
						reverse.add_redge(j, i);
					}
				}
			}
		}
		return reverse;
	}
	else {
		throw GraphErr("Reverse graph for undirected graph is this graph");
	}
}

void dfs1(const adjacencyMatrix& graph, size_t v, std::vector<bool>& used, std::vector<size_t>& order) {
	used[v] = true;
	for (size_t i = 0; i < graph.matrix.size(); i++) {
		if (used[i] == false && graph.matrix[v][i] != INT_MAX && i != v) {
			dfs1(graph, i, used, order);
		}
	}
	order.push_back(v);
}

void dfs2(const adjacencyMatrix& graph, size_t v, std::vector<bool>& used, std::vector<size_t>& order, std::vector<size_t>& component) {
	used[v] = true;
	component.push_back(v);
	for (size_t i = 0; i < graph.matrix.size(); i++) {
		if (used[i] == false && graph.matrix[v][i] != INT_MAX && v != i) {
			dfs2(graph, i, used, order, component);
		}
	}
}

void dfs1(const adjacencyStructure& graph, size_t v, std::vector<bool>& used, std::vector<size_t>& order) {
	used[v] = true;
	for (size_t i = 0; i < graph.nodes[v].size(); i++) {
		if (used[graph.nodes[v][i]] == false) {
			dfs1(graph, graph.nodes[v][i], used, order);
		}
	}
	order.push_back(v);
}

void dfs2(const adjacencyStructure& graph, size_t v, std::vector<bool>& used, std::vector<size_t>& order, std::vector<size_t>& component) {
	used[v] = true;
	component.push_back(v);
	for (size_t i = 0; i < graph.nodes[v].size(); i++) {
		if (used[graph.nodes[v][i]] == false) {
			dfs2(graph, graph.nodes[v][i], used, order, component);
		}
	}
}

void adjacencyMatrix::find_strong_components(std::vector<std::vector<size_t>>& for_components) {
	if (directed) {
		adjacencyMatrix reverse = build_reverse();
		std::vector<bool>checked(matrix.size(), false);
		std::vector<size_t> order;
		std::vector<size_t> component;
		for (size_t i = 0; i < matrix.size(); i++) {
			if (checked[i] == false) {
				dfs1(*this, i, checked, order);
			}
		}
		checked.assign(matrix.size(), false);
		for (size_t i = 0; i < matrix.size(); i++) {
			size_t v = order[matrix.size() - 1 - i];
			if (checked[v] == false) {
				dfs2(reverse, v, checked, order, component);
				for_components.push_back(component);
				component.clear();
			}
		}
	}
	else {
		throw GraphErr("This function only for directed graphs");
	}
}

std::vector<int> adjacencyMatrix::dijkstra_algorithm(size_t node)
{
	std::vector <int> answer(matrix.size(), INT_MAX);
	answer[node] = 0;
	std::vector <bool> checked(matrix.size(), 0);
	int min_distance = 0;
	size_t min_node = node;
	while (min_distance < INT_MAX)
	{
		size_t current = min_node;
		checked[current] = true;
		for (size_t i = 0; i < matrix.size(); i++) {
			if (current != i && answer[current] != INT_MAX && matrix[current][i] != INT_MAX) {
				if (answer[current] + matrix[current][i] < answer[i]) {
					answer[i] = answer[current] + matrix[current][i];
				}
			}
		}
		min_distance = INT_MAX;
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

std::vector<std::vector<int>> adjacencyMatrix::floyd_algorithm()
{
	if (matrix.size() > 0) {
		std::vector<std::vector<std::vector<int>>>ways(matrix.size() + 1, std::vector<std::vector<int>>(matrix.size(), std::vector<int>(matrix.size())));

		for (size_t i = 0; i < matrix.size(); i++) {
			for (size_t j = 0; j < matrix.size(); j++) {
				if (i == j) {
					ways[0][i][j] = 0;
				}
				else {
					ways[0][i][j] = matrix[i][j];
				}
			}
		}
		for (size_t k = 1; k < matrix.size() + 1; k++) {
			for (size_t i = 0; i < matrix.size(); i++) {
				for (size_t j = 0; j < matrix.size(); j++) {
					if (ways[k - 1][i][k - 1] != INT_MAX && ways[k - 1][k - 1][j] != INT_MAX) {
						ways[k][i][j] = min(ways[k - 1][i][j], ways[k - 1][i][k - 1] + ways[k - 1][k - 1][j]);
					}
					else {
						ways[k][i][j] = ways[k - 1][i][j];
					}
				}
			}
		}
		return ways[matrix.size()];
	}
	else {
		throw GraphErr("This graph has no nodes");
	}
}

std::vector<int> adjacencyMatrix::bellman_ford_algorithm(size_t node)
{
	if (node < matrix.size()) {
		std::vector <int> answer(matrix.size(), INT_MAX);
		answer[node] = 0;
		for (size_t k = 1; k < matrix.size(); k++) {
			for (size_t i = 0; i < matrix.size(); i++) {
				for (size_t j = 0; j < matrix.size(); j++) {
					if (answer[j] != INT_MAX && matrix[j][i] != INT_MAX) {
						if (answer[j] + matrix[j][i] < answer[i]) {
							answer[i] = answer[j] + matrix[j][i];
						}
					}
				}
			}
		}
		return answer;
	}
	else {
		throw GraphErr("Incorrect node");
	}
}

std::vector<size_t> adjacencyMatrix::depth_first(bool important_weight)
{
	if (important_weight == 1 && weighted == 0) {
		throw GraphErr("Weight can't be important in unweighted graph");
	}
	else {
		std::vector<size_t> order;
		std::vector<bool>checked(matrix.size(), false);
		for (size_t i = 0; i < checked.size(); i++) {
			if (checked[i] == false) {
				checked[i] = true;
				order.push_back(i);
				depth_first(i, checked, important_weight, order);
			}
		}
		return order;
	}
}

void adjacencyMatrix::depth_first(size_t node, std::vector<bool>& checked, bool important_weight, std::vector<size_t>& order) {
	if (!important_weight) {
		for (size_t i = 0; i < matrix.size(); i++)
		{
			if (matrix[node][i] != INT_MAX && i != node && checked[i] == false) {
				std::stack<size_t>st;
				st.emplace(i);
				while (!st.empty())
				{
					size_t p = st.top();
					if (checked[p] == false) {
						checked[p] = true;
						order.push_back(p);
					}
					bool found = false;
					size_t what_found;
					for (size_t j = 0; j < matrix.size(); j++)
					{
						if (matrix[p][j] != INT_MAX && p != j && checked[j] == false) {
							found = true;
							what_found = j;
							break;
						}
					}
					if (found) {
						st.emplace(what_found);
					}
					else {
						st.pop();
					}
				}
			}
		}
	}
	else {
		std::vector<node_struct>sorted;
		for (size_t i = 0; i < matrix.size(); i++) {
			if (matrix[node][i] != INT_MAX && node != i && checked[i] != 1) {
				sorted.push_back({ matrix[node][i], i });
			}
		}
		if (sorted.size() > 1) {
			quicksort(sorted, 0, sorted.size() - 1);
		}
		for (auto& i : sorted)
		{
			if (matrix[node][i.node] != 0 && checked[i.node] == false) {
				std::stack<size_t>st;
				st.emplace(i.node);
				while (!st.empty())
				{
					size_t p = st.top();
					if (checked[p] == false) {
						checked[p] = true;
						order.push_back(p);
					}
					std::vector<node_struct>sorted2;
					for (size_t j = 0; j < matrix.size(); j++) {
						if (matrix[p][j] != INT_MAX && p != j && checked[j] == 0) {
							sorted2.push_back({ matrix[p][j], j });
						}
					}
					if (sorted2.size() == 0) {
						st.pop();
					}
					else {
						quicksort(sorted2, 0, sorted2.size() - 1);
						st.emplace(sorted2[0].node);
					}
				}
			}
		}
	}
}

adjacencyMatrix adjacencyMatrix::depth_first_spanning_tree(bool important_weight)
{
	if (important_weight == 1 && weighted == 0) {
		throw GraphErr("Weight can't be important in unweighted graph");
	}
	else {
		std::string check_correctness = this->is_connected_graph();
		if (check_correctness == "strongly connected graph" || check_correctness == "connected graph") {
			adjacencyMatrix to_return(weighted, 0, this->matrix.size(), 0);
			std::vector<bool>checked(matrix.size(), false);
			checked[0] = true;
			if (!important_weight) {
				for (size_t i = 0; i < matrix.size(); i++)
				{
					if (matrix[0][i] != INT_MAX && i != 0 && checked[i] == false) {
						std::stack<size_t>st;
						st.emplace(i);
						size_t parent_node = 0;
						int distance = matrix[0][i];
						while (!st.empty())
						{
							size_t p = st.top();
							if (checked[p] == false) {
								checked[p] = true;
								if (weighted) {
									to_return.add_redge(parent_node, p, distance);
								}
								else {
									to_return.add_redge(parent_node, p);
								}
							}
							bool found = false;
							size_t what_found;
							for (size_t j = 0; j < matrix.size(); j++)
							{
								if (matrix[p][j] != INT_MAX && p != j && checked[j] == false) {
									found = true;
									what_found = j;
									break;
								}
							}
							if (found) {
								parent_node = p;
								distance = matrix[p][what_found];
								st.emplace(what_found);
							}
							else {
								st.pop();
							}
						}
					}
				}
			}
			else {
				std::vector<node_struct>sorted;
				for (size_t i = 0; i < matrix.size(); i++) {
					if (matrix[0][i] != INT_MAX && i != 0 && checked[i] != 1) {
						sorted.push_back({ matrix[0][i], i });
					}
				}
				quicksort(sorted, 0, sorted.size() - 1);
				for (auto& i : sorted)
				{
					if (matrix[0][i.node] != INT_MAX && 0 != i.node && checked[i.node] == false) {
						std::stack<size_t>st;
						st.emplace(i.node);
						size_t parent_node = 0;
						int distance = matrix[0][i.node];
						while (!st.empty())
						{
							size_t p = st.top();
							if (checked[p] == false) {
								checked[p] = true;
								if (weighted) {
									to_return.add_redge(parent_node, p, distance);
								}
								else {
									to_return.add_redge(parent_node, p);
								}
							}
							std::vector<node_struct>sorted2;
							for (size_t j = 0; j < matrix.size(); j++) {
								if (matrix[p][j] != INT_MAX && p != j && checked[j] == 0) {
									sorted2.push_back({ matrix[p][j], j });
								}
							}
							if (sorted2.size() == 0) {
								st.pop();
							}
							else {
								quicksort(sorted2, 0, sorted2.size() - 1);
								parent_node = p;
								distance = matrix[p][sorted2[0].node];
								st.emplace(sorted2[0].node);
							}
						}
					}
				}
			}
			return to_return;
		}
		else {
			throw GraphErr("This function only for connected graphs");
		}
	}
}

adjacencyMatrix adjacencyMatrix::minimum_spanning_tree_kruskal()
{
	if (weighted && !directed && this->is_connected_graph() == "connected graph") {
		std::vector<way_struct>ways;
		adjacencyMatrix answer(weighted, directed, matrix.size(), 0);
		std::vector<size_t>components(matrix.size());
		for (size_t i = 0; i < matrix.size(); i++) {
			components[i] = i;
		}
		for (size_t i = 0; i < matrix.size(); i++) {
			for (size_t j = i + 1; j < matrix.size(); j++) {
				if (matrix[i][j] != INT_MAX) {
					ways.push_back({ matrix[i][j] ,i,j });
				}
			}
		}
		quicksort(ways, 0, ways.size() - 1);
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
		throw GraphErr("This function only for weighted, undirected and connected graphs");
	}
}

adjacencyMatrix adjacencyMatrix::minimum_spanning_reverse_delete()
{
	if (weighted && !directed && this->is_connected_graph() == "connected graph") {
		adjacencyMatrix answer(*this);
		std::vector<way_struct>edges;
		for (size_t i = 0; i < matrix.size(); i++) {
			for (size_t j = i + 1; j < matrix.size(); j++) {
				if (matrix[i][j] != INT_MAX && i != j) {
					edges.push_back({ matrix[i][j] ,i,j });
				}
			}
		}
		quicksort(edges, 0, edges.size() - 1);
		for (size_t i = edges.size() - 1; i > 0; i--) {
			int weight = answer.delete_redge(edges[i].begin, edges[i].end);
			if (answer.is_connected_graph() != "connected graph") {
				answer.add_redge(edges[i].begin, edges[i].end, weight);
			}
		}
		return answer;
	}
	else {
		throw GraphErr("This function only for weighted, undirected and connected graphs");
	}
}

adjacencyStructure adjacencyMatrix::get_adjacencyStructure()
{
	return adjacencyStructure(*this);
}

bool adjacencyMatrix::is_acyclic()
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

bool adjacencyMatrix::check_cyclic(size_t node, std::vector<char>& checked, size_t parent)
{
	checked[node] = 2;
	for (size_t i = 0; i < matrix.size(); i++) {
		if (matrix[node][i] != INT_MAX && node != i && i != parent && checked[i] == 2) {
			return true;
		}
		else if (matrix[node][i] != INT_MAX && node != i && i != parent) {
			check_cyclic(i, checked, node);
		}
	}
	checked[node] = 3;
	return false;
}

std::vector<size_t> adjacencyMatrix::topological_sorting_kahn()
{
	if (directed && is_acyclic()) {
		std::vector<size_t>order;
		std::queue<size_t>start_elements;
		size_t was_there = 0;
		std::vector<size_t>in_degrees(matrix.size());
		for (size_t i = 0; i < matrix.size(); i++) {
			size_t in_degree = 0;
			for (size_t j = 0; j < matrix.size(); j++) {
				if (matrix[j][i] != INT_MAX && i != j) {
					in_degree++;
				}
			}
			in_degrees[i] = in_degree;
			if (in_degree == 0) {
				start_elements.push(i);
			}
		}
		while (!start_elements.empty()) {
			order.push_back(start_elements.front());
			was_there++;
			for (size_t i = 0; i < matrix.size(); i++) {
				if (matrix[start_elements.front()][i] != INT_MAX && start_elements.front() != i) {
					in_degrees[i]--;
					if (in_degrees[i] == 0) {
						start_elements.push(i);
					}
				}
			}
			start_elements.pop();
		}
		if (matrix.size() == was_there) {
			return order;
		}
		else {
			throw GraphErr("Topological sorting works incorrect for this graph");
		}
	}
	else {
		throw GraphErr("Topological sorting is possible only for directed and acyclic graphs");
	}
}



adjacencyStructure::adjacencyStructure(bool weighted, bool directed, size_t number_of_nodes, size_t number_of_edges)
{
	this->weighted = weighted;
	this->directed = directed;
	this->structure = 1;
	if ((directed && number_of_edges <= number_of_nodes * (number_of_nodes - 1)) || (!directed && number_of_edges <= (number_of_nodes * (number_of_nodes - 1)) / 2)) {
		nodes.assign(number_of_nodes, std::vector<size_t>());
		if (weighted) {
			weights.assign(number_of_nodes, std::vector<int>());
		}
		if ((directed && number_of_edges <= number_of_nodes * (number_of_nodes - 1) / 2) || (!directed && number_of_edges <= (number_of_nodes * (number_of_nodes - 1)) / 4)) {
			size_t copy_number_edges = number_of_edges;
			std::vector<edge_struct>all_edges;
			for (size_t i = 0; i < copy_number_edges; i++) {
				size_t first_node = mersenne() % number_of_nodes;
				size_t second_node = mersenne() % number_of_nodes;
				if (first_node == second_node || find(first_node, second_node)) {
					copy_number_edges++;
				}
				else {
					if (weighted) {
						int weight = 1 + mersenne() % 10;
						nodes[first_node].push_back(second_node);
						weights[first_node].push_back(weight);
						if (!directed) {
							nodes[second_node].push_back(first_node);
							weights[second_node].push_back(weight);
						}
					}
					else {
						nodes[first_node].push_back(second_node);
						if (!directed) {
							nodes[second_node].push_back(first_node);
						}
					}
				}
			}
		}
		else {
			for (size_t i = 0; i < number_of_nodes; i++) {
				if (directed) {
					for (size_t j = 0; j < number_of_nodes; j++) {
						if (i != j) {
							if (weighted) {
								int weight = 1 + mersenne() % 10;
								add_redge(i, j, weight);
							}
							else {
								add_redge(i, j);
							}
						}
					}
				}
				else {
					for (size_t j = i + 1; j < number_of_nodes; j++) {
						if (weighted) {
							int weight = 1 + mersenne() % 10;
							add_redge(i, j, weight);
						}
						else {
							add_redge(i, j);
						}
					}
				}
			}
			size_t number_to_delete = 0;
			if (directed) {
				number_to_delete = number_of_nodes * (number_of_nodes - 1) - number_of_edges;
			}
			else {
				number_to_delete = number_of_nodes * (number_of_nodes - 1) / 2 - number_of_edges;
			}
			for (size_t i = 0; i < number_to_delete; i++) {
				try
				{
					size_t first = mersenne() % number_of_nodes;
					size_t second = mersenne() % number_of_nodes;
					delete_redge(first, second);
				}
				catch (const std::exception&)
				{
					number_to_delete++;
				}
			}
		}
	}
	else {
		throw GraphErr("Impossible number of edges for number of nodes");
	}
}

adjacencyStructure::adjacencyStructure(const adjacencyMatrix& another)
{
	this->directed = another.directed;
	this->weighted = another.weighted;
	this->structure = 1;
	nodes.assign(another.matrix.size(), std::vector<size_t>());
	if (weighted) {
		weights.assign(another.matrix.size(), std::vector<int>());
	}
	for (size_t i = 0; i < another.matrix.size(); i++) {
		for (size_t j = 0; j < another.matrix.size(); j++) {
			if (another.matrix[i][j] != INT_MAX && i != j) {
				nodes[i].push_back(j);
				if (weighted) {
					weights[i].push_back(another.matrix[i][j]);
				}
			}
		}
	}
}

bool adjacencyStructure::find(size_t first_node, size_t second_node) const
{
	if (first_node < nodes.size() && second_node < nodes.size()) {
		for (size_t i = 0; i < nodes[first_node].size(); i++) {
			if (nodes[first_node][i] == second_node) {
				return true;
			}
		}
		return false;
	}
	else {
		throw GraphErr("Incorrect first or second node");
	}
}

void adjacencyStructure::add_redge(size_t first_node, size_t second_node)
{
	if (first_node < nodes.size() && second_node < nodes.size()) {
		if (first_node != second_node) {
			if (!(this->weighted)) {
				if (!(this->find(first_node, second_node))) {
					nodes[first_node].push_back(second_node);
					if (!directed) {
						nodes[second_node].push_back(first_node);
					}
				}
				else {
					throw GraphErr("Edge with those nodes already exist");
				}
			}
			else {
				throw GraphErr("This type of function only for unweighted graphs");
			}
		}
		else {
			throw GraphErr("Graph cannot contain self-loops");
		}
	}
	else {
		throw GraphErr("Incorrect first or second node");
	}
}

void adjacencyStructure::add_redge(size_t first_node, size_t second_node, int weight)
{
	if (first_node < nodes.size() && second_node < nodes.size()) {
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
	}
}

void adjacencyStructure::add_node()
{
	nodes.push_back(std::vector<size_t>());
	if (weighted) {
		weights.push_back(std::vector<int>());
	}
}

void adjacencyStructure::show()
{
	for (size_t i = 0; i < nodes.size(); i++) {
		std::cout << "Node " << i;
		if (nodes[i].size() == 0) {
			std::cout << " is not linked with any other";
		}
		else {
			std::cout << " linked with ";
			for (size_t j = 0; j < nodes[i].size(); j++) {
				std::cout << " " << nodes[i][j];
				if (weighted) {
					std::cout << " (" << weights[i][j] << ')';
				}
			}
			std::cout << " node";
			if (nodes[i].size() > 1) {
				std::cout << 's';
			}
		}
		std::cout << std::endl;
	}
}

void adjacencyStructure::show_structure()
{
	if (nodes.size() > 0) {
		std::cout << "Nodes: " << std::endl;
		for (size_t i = 0; i < nodes.size(); i++) {
			std::cout << "Node " << i << " :";
			if (nodes[i].size() > 0) {
				for (size_t j = 0; j < nodes[i].size(); j++) {
					if (nodes[i][j] == INT_MAX) {
						std::cout << "\t-";
					}
					else {
						std::cout << '\t' << nodes[i][j];
					}
				}
			}
			else {
				std::cout << "---";
			}
			std::cout << std::endl;
		}
		if (weighted) {
			std::cout << "Weights: " << std::endl;
			for (size_t i = 0; i < nodes.size(); i++) {
				std::cout << "Node " << i << " :";
				if (nodes[i].size() > 0) {
					for (size_t j = 0; j < nodes[i].size(); j++) {
						std::cout << '\t' << weights[i][j];
					}
				}
				else {
					std::cout << "---";
				}
				std::cout << std::endl;
			}
		}
	}
	else {
		std::cout << "Nothing" << std::endl;
	}
}

int adjacencyStructure::delete_redge(size_t first_node, size_t second_node)
{
	int weight = 1;
	if (first_node < nodes.size() && second_node < nodes.size()) {
		if (first_node != second_node) {
			bool found = false;
			for (size_t i = 0; i < nodes[first_node].size(); i++) {
				if (nodes[first_node][i] == second_node) {
					found = true;
					nodes[first_node].erase(nodes[first_node].begin() + i);
					if (weighted) {
						weight = weights[first_node][i];
						weights[first_node].erase(weights[first_node].begin() + i);
					}
					if (!directed) {
						for (size_t j = 0; j < nodes[second_node].size(); j++) {
							if (nodes[second_node][j] == first_node) {
								nodes[second_node].erase(nodes[second_node].begin() + j);
								if (weighted) {
									weights[second_node].erase(weights[second_node].begin() + j);
								}
								break;
							}
						}
					}
					break;
				}
			}
			if (!found) {
				throw GraphErr("This edge doesn't exist");
			}
			else {
				return weight;
			}
		}
		else {
			throw GraphErr("Graph cannot contain self-loops");
		}
	}
	else {
		throw GraphErr("Incorrect first or second node");
	}
}

void adjacencyStructure::delete_node(size_t node)
{
	if (node < nodes.size()) {
		nodes.erase(nodes.begin() + node);
		if (weighted) {
			weights.erase(weights.begin() + node);
		}
		for (size_t i = 0; i < nodes.size(); i++) {
			for (size_t j = 0; j < nodes[i].size(); j++) {
				if (nodes[i][j] == node) {
					nodes[i].erase(nodes[i].begin() + j);
					if (weighted) {
						weights[i].erase(weights[i].begin() + j);
					}
				}
			}
		}
		for (size_t i = 0; i < nodes.size(); i++) {
			for (size_t j = 0; j < nodes[i].size(); j++) {
				if (nodes[i][j] > node) {
					nodes[i][j]--;
				}
			}
		}
	}
	else {
		throw GraphErr("Incorrect node");
	}
}

bool adjacencyStructure::is_acyclic()
{
	std::vector<char>checked;
	for (size_t i = 0; i < nodes.size(); i++) {
		checked.assign(nodes.size(), 1);
		if (check_cyclic(i, checked, (size_t)LLONG_MAX)) {
			return false;
		}
	}
	return true;
}

bool adjacencyStructure::check_cyclic(size_t node, std::vector<char>& checked, size_t parent)
{
	checked[node] = 2;
	for (size_t i = 0; i < nodes[node].size(); i++) {
		if (nodes[node][i] != parent && checked[nodes[node][i]] == 2) {
			return true;
		}
		else if (nodes[node][i] != parent) {
			check_cyclic(nodes[node][i], checked, node);
		}
	}
	checked[node] = 3;
	return false;
}

std::string adjacencyStructure::is_connected_graph()
{
	std::vector<std::vector<size_t>> for_components;
	if (directed) {
		find_strong_components(for_components);
	}
	else {
		find_components(for_components);
	}
	if (for_components.size() == 1) {
		if (directed) {
			return "strongly connected graph";
		}
		else {
			return "connected graph";
		}
	}
	else if (directed) {
		adjacencyStructure new_graph(0, 0, nodes.size(), 0);
		for (size_t i = 0; i < nodes.size(); i++) {
			for (size_t j = 0; j < nodes[i].size(); j++) {
				if (nodes[i][j] > i) {
					new_graph.add_redge(nodes[i][j], i);
				}
			}
		}
		for_components.clear();
		new_graph.find_components(for_components);
		if (for_components.size() == 1) {
			return "weakly connected graph";
		}
	}
	return "disconnected graph";
}

void adjacencyStructure::find_nodes(size_t node, std::vector<bool>& for_check, std::vector<size_t>& component)
{
	for_check[node] = 1;
	component.push_back(node);
	for (auto& i : nodes[node]) {
		if (for_check[i] != 1) {
			find_nodes(i, for_check, component);
		}
	}
}

void adjacencyStructure::find_components(std::vector<std::vector<size_t>>& for_components)
{
	if (!directed) {
		std::vector<bool> for_check(nodes.size(), 0);
		for_components.clear();
		for (size_t i = 0; i < for_check.size(); i++) {
			if (for_check[i] == false) {
				std::vector<size_t>component;
				find_nodes(i, for_check, component);
				for_components.push_back(component);
			}
		}
	}
	else {
		throw GraphErr("This function only for undirected graphs");
	}
}

adjacencyStructure adjacencyStructure::build_reverse() const
{
	if (directed) {
		adjacencyStructure reverse(weighted, directed);
		for (size_t i = 0; i < nodes.size(); i++) {
			reverse.add_node();
		}
		for (size_t i = 0; i < nodes.size(); i++) {
			for (size_t j = 0; j < nodes[i].size(); j++) {
				if (weighted) {
					reverse.add_redge(nodes[i][j], i, weights[i][j]);
				}
				else {
					reverse.add_redge(nodes[i][j], i);
				}
			}
		}
		return reverse;
	}
	else {
		throw GraphErr("Reverse graph for undirected graph is this graph");
	}
}

void adjacencyStructure::find_strong_components(std::vector<std::vector<size_t>>& for_components)
{
	if (directed) {
		adjacencyStructure reverse = build_reverse();
		std::vector<bool>checked(nodes.size(), false);
		std::vector<size_t> order;
		std::vector<size_t> component;
		for (size_t i = 0; i < nodes.size(); i++) {
			if (checked[i] == false) {
				dfs1(*this, i, checked, order);
			}
		}
		checked.assign(nodes.size(), false);
		for (size_t i = 0; i < nodes.size(); i++) {
			size_t v = order[nodes.size() - 1 - i];
			if (checked[v] == false) {
				dfs2(reverse, v, checked, order, component);
				for_components.push_back(component);
				component.clear();
			}
		}
	}
	else {
		throw GraphErr("This function only for directed graphs");
	}
}

std::vector<int> adjacencyStructure::dijkstra_algorithm(size_t node)
{
	std::vector <int> answer(nodes.size(), INT_MAX);
	answer[node] = 0;
	std::vector <bool> checked(nodes.size(), 0);
	int min_distance = 0;
	size_t min_node = node;
	while (min_distance < INT_MAX)
	{
		size_t current = min_node;
		checked[current] = true;
		for (size_t i = 0; i < nodes[current].size(); i++) {
			if (weighted) {
				if (answer[current] != INT_MAX && weights[current][i] != INT_MAX) {
					if (answer[current] + weights[current][i] < answer[nodes[current][i]]) {
						answer[nodes[current][i]] = answer[current] + weights[current][i];
					}
				}
			}
			else {
				if (answer[current] != INT_MAX) {
					if (answer[current] + 1 < answer[nodes[current][i]]) {
						answer[nodes[current][i]] = answer[current] + 1;
					}
				}
			}
		}
		min_distance = INT_MAX;
		for (size_t i = 0; i < nodes.size(); i++) {
			if (checked[i] == false && answer[i] < min_distance)
			{
				min_distance = answer[i];
				min_node = i;
			}
		}
	}
	return answer;
}

std::vector<std::vector<int>> adjacencyStructure::floyd_algorithm()
{
	try
	{
		adjacencyMatrix matrix(*this);
		std::vector<std::vector<int>>ways = matrix.floyd_algorithm();
		return ways;
	}
	catch (const std::exception& ex)
	{
		throw ex;
	}
}

std::vector<int> adjacencyStructure::bellman_ford_algorithm(size_t node)
{
	try
	{
		adjacencyMatrix matrix(*this);
		std::vector<int> answer = matrix.bellman_ford_algorithm(node);
		return answer;
	}
	catch (const std::exception& ex)
	{
		throw ex;
	}
}

std::vector<size_t> adjacencyStructure::depth_first(bool important_weight)
{
	if (important_weight == 1 && weighted == 0) {
		throw GraphErr("Weight can't be important in unweighted graph");
	}
	else {
		std::vector<size_t> order;
		std::vector<bool>checked(nodes.size(), false);
		for (size_t i = 0; i < checked.size(); i++) {
			if (checked[i] == false) {
				checked[i] = true;
				order.push_back(i);
				depth_first(i, checked, important_weight, order);
			}
		}
		return order;
	}
}

void adjacencyStructure::depth_first(size_t node, std::vector<bool>& checked, bool important_weight, std::vector<size_t>& order)
{
	if (!important_weight) {
		for (size_t i = 0; i < nodes[node].size(); i++)
		{
			if (checked[nodes[node][i]] == false) {
				std::stack<size_t>st;
				st.emplace(nodes[node][i]);
				while (!st.empty())
				{
					size_t p = st.top();
					if (checked[p] == false) {
						checked[p] = true;
						order.push_back(p);
					}
					bool found = false;
					size_t what_found;
					for (size_t j = 0; j < nodes[p].size(); j++)
					{
						if (checked[nodes[p][j]] == false) {
							found = true;
							what_found = nodes[p][j];
							break;
						}
					}
					if (found) {
						st.emplace(what_found);
					}
					else {
						st.pop();
					}
				}
			}
		}
	}
	else {
		std::vector<node_struct>sorted;
		for (size_t i = 0; i < nodes[node].size(); i++) {
			if (checked[nodes[node][i]] != 1) {
				if (weighted) {
					sorted.push_back({ weights[node][i], nodes[node][i] });
				}
				sorted.push_back({ 1, nodes[node][i] });
			}
		}
		if (weighted && sorted.size() > 1) {
			quicksort(sorted, 0, sorted.size() - 1);
		}
		for (auto& i : sorted)
		{
			if (checked[i.node] == false) {
				std::stack<size_t>st;
				st.emplace(i.node);
				while (!st.empty())
				{
					size_t p = st.top();
					if (checked[p] == false) {
						checked[p] = true;
						order.push_back(p);
					}
					std::vector<node_struct>sorted2;
					for (size_t j = 0; j < nodes[p].size(); j++) {
						if (checked[nodes[p][j]] == 0) {
							if (weighted) {
								sorted2.push_back({ weights[p][j], nodes[p][j] });
							}
							else {
								sorted2.push_back({ 1, nodes[p][j] });
							}
						}
					}
					if (sorted2.size() == 0) {
						st.pop();
					}
					else {
						quicksort(sorted2, 0, sorted2.size() - 1);
						st.emplace(sorted2[0].node);
					}
				}
			}
		}
	}
}

adjacencyStructure adjacencyStructure::depth_first_spanning_tree(bool important_weight)
{
	if (important_weight == 1 && weighted == 0) {
		throw GraphErr("Weight can't be important in unweighted graph");
	}
	else {
		std::string check_correctness = this->is_connected_graph();
		if (check_correctness == "strongly connected graph" || check_correctness == "connected graph") {
			adjacencyStructure to_return(weighted, 0, this->nodes.size(), 0);
			std::vector<bool>checked(nodes.size(), false);
			checked[0] = true;
			if (!important_weight) {
				for (size_t i = 0; i < nodes[0].size(); i++)
				{
					if (checked[nodes[0][i]] == false) {
						std::stack<size_t>st;
						st.emplace(nodes[0][i]);
						size_t parent_node = 0;
						int distance;
						if (weighted) {
							distance = weights[0][i];
						}
						else {
							distance = 1;
						}
						while (!st.empty())
						{
							size_t p = st.top();
							if (checked[p] == false) {
								checked[p] = true;
								if (weighted) {
									to_return.add_redge(parent_node, p, distance);
								}
								else {
									to_return.add_redge(parent_node, p);
								}
							}
							bool found = false;
							size_t what_found;
							for (size_t j = 0; j < nodes[p].size(); j++)
							{
								if (checked[nodes[p][j]] == false) {
									found = true;
									what_found = nodes[p][j];
									if (weighted) {
										distance = weights[p][j];
									}
									else {
										distance = 1;
									}
									break;
								}
							}
							if (found) {
								parent_node = p;
								st.emplace(what_found);
							}
							else {
								st.pop();
							}
						}
					}
				}
			}
			else {
				std::vector<node_struct>sorted;
				for (size_t i = 0; i < nodes[0].size(); i++) {
					if (checked[nodes[0][i]] != 1) {
						if (weighted) {
							sorted.push_back({ weights[0][i], nodes[0][i] });
						}
						else {
							sorted.push_back({ 1, nodes[0][i] });
						}
					}
				}
				if (weighted) {
					quicksort(sorted, 0, sorted.size() - 1);
				}
				for (auto& i : sorted)
				{
					if (checked[i.node] == false) {
						std::stack<size_t>st;
						checked[i.node] = true;
						if (weighted) {
							to_return.add_redge(0, i.node, i.weight);
						}
						else {
							to_return.add_redge(0, i.node);
						}
						st.emplace(i.node);
						size_t parent_node = 0;
						int distance = 0;
						if (weighted) {
							for (size_t m = 0; m < nodes[0].size(); m++) {
								if (nodes[0][m] == i.node) {
									distance = weights[0][m];
									break;
								}
							}
						}
						else {
							distance = 1;
						}
						while (!st.empty())
						{
							size_t p = st.top();
							if (checked[p] == false) {
								checked[p] = true;
								if (weighted) {
									to_return.add_redge(parent_node, p, distance);
								}
								else {
									to_return.add_redge(parent_node, p);
								}
							}
							std::vector<node_struct>sorted2;
							for (size_t j = 0; j < nodes[p].size(); j++) {
								if (checked[nodes[p][j]] == 0) {
									if (weighted) {
										sorted2.push_back({ weights[p][j], nodes[p][j] });
									}
									else {
										sorted2.push_back({ 1, nodes[p][j] });
									}
								}
							}
							if (sorted2.size() == 0) {
								st.pop();
							}
							else {
								quicksort(sorted2, 0, sorted2.size() - 1);
								parent_node = p;
								if (weighted) {
									distance = sorted2[0].weight;
								}
								else {
									distance = 1;
								}
								st.emplace(sorted2[0].node);
							}
						}
					}
				}
			}
			return to_return;
		}
		else {
			throw GraphErr("This function only for connected graphs");
		}
	}
}

adjacencyStructure adjacencyStructure::minimum_spanning_tree_kruskal()
{
	if (weighted && !directed && this->is_connected_graph() == "connected graph") {
		std::vector<way_struct>ways;
		adjacencyStructure answer(weighted, directed, nodes.size(), 0);
		std::vector<size_t>components(nodes.size());
		for (size_t i = 0; i < nodes.size(); i++) {
			components[i] = i;
		}
		for (size_t i = 0; i < nodes.size(); i++) {
			for (size_t j = 0; j < nodes[i].size(); j++) {
				if (i < nodes[i][j]) {
					ways.push_back({ weights[i][j] ,i,nodes[i][j] });
				}
			}
		}
		quicksort(ways, 0, ways.size() - 1);
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
		throw GraphErr("This function only for weighted, undirected and connected graphs");
	}
}

adjacencyStructure adjacencyStructure::minimum_spanning_reverse_delete()
{
	if (weighted && !directed && this->is_connected_graph() == "connected graph") {
		adjacencyStructure answer(*this);
		std::vector<way_struct>edges;
		for (size_t i = 0; i < nodes.size(); i++) {
			for (size_t j = 0; j < nodes[i].size(); j++) {
				if (i < nodes[i][j]) {
					edges.push_back({ weights[i][j] ,i, nodes[i][j] });
				}
			}
		}
		quicksort(edges, 0, edges.size() - 1);
		for (size_t i = edges.size() - 1; i > 0; i--) {
			int weight = answer.delete_redge(edges[i].begin, edges[i].end);
			if (answer.is_connected_graph() != "connected graph") {
				answer.add_redge(edges[i].begin, edges[i].end, weight);
			}
		}
		return answer;
	}
	else {
		throw GraphErr("This function only for weighted, undirected and connected graphs");
	}
}

std::vector<size_t> adjacencyStructure::topological_sorting_kahn()
{
	if (directed && is_acyclic()) {
		std::vector<size_t>order;
		std::queue<size_t>start_elements;
		size_t was_there = 0;
		std::vector<size_t>in_degrees(nodes.size());
		for (size_t i = 0; i < nodes.size(); i++) {
			size_t in_degree = 0;
			for (size_t j = 0; j < nodes.size(); j++) {
				for (size_t k = 0; k < nodes[j].size(); k++) {
					if (nodes[j][k] == i) {
						in_degree++;
					}
				}
			}
			in_degrees[i] = in_degree;
			if (in_degree == 0) {
				start_elements.push(i);
			}
		}
		while (!start_elements.empty()) {
			order.push_back(start_elements.front());
			was_there++;
			for (size_t i = 0; i < nodes[start_elements.front()].size(); i++) {
				in_degrees[nodes[start_elements.front()][i]]--;
				if (in_degrees[nodes[start_elements.front()][i]] == 0) {
					start_elements.push(nodes[start_elements.front()][i]);
				}
			}
			start_elements.pop();
		}
		if (nodes.size() == was_there) {
			return order;
		}
		else {
			throw GraphErr("Topological sorting works incorrect for this graph");
		}
	}
	else {
		throw GraphErr("Topological sorting is possible only for directed and acyclic graphs");
	}
}


bool is_int(const std::string& str)
{
	for (auto i : str) {
		if (i < 48 && i>57) {
			return false;
		}
	}
	return true;
}