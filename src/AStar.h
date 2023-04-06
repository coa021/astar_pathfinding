#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <list>
#include <random>
#include <functional>
#include <memory>

class AStar
{
public:

	bool initialize();
	bool solve_astar();
	void connect_path();
	void print_graph();

	void debug_print_graph();
private:
	bool randomBool();

	struct Node;
	using NodePtr = std::shared_ptr<Node>;

	NodePtr create_node();

	//to acces nodes[x][y] use nodes.at(y).at(x)
	std::vector< std::vector< NodePtr > > nodes;
	NodePtr startNode{ nullptr };
	NodePtr endNode{ nullptr };

	int map_width{ 16 };
	int map_height{ 16 };
};

