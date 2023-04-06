#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


class AStar
{
public:
	bool initialize()
	{
		nodes = new Node[map_width * map_height];

		for (int x = 0; x < map_width; x++)
			for (int y = 0; y < map_height; y++)
			{
				nodes[y * map_width + x].x = x;
				nodes[y * map_width + x].y = y;
				nodes[y * map_width + x].b_Obstacle = false;
				nodes[y * map_width + x].b_Visited = false;
				nodes[y * map_width + x].parent = nullptr;
			}

		/*  creating connections
			there will be no possibility for diagonal connections
			only hor/ver */
		for (int x = 0; x < map_width; x++)
			for (int y = 0; y < map_height; y++)
			{
				// checking for bounds
				// if node is on the edges
				// theres no possibility to go in some directions
				if (y > 0)
					nodes[y * map_width + x].neighbours.push_back(&nodes[(y - 1) * map_width + x);
				if (y < map_height - 1)
					nodes[y * map_width + x].neighbours.push_back(&nodes[(y + 1) * map_width + x);
				if (x > 0)
					nodes[y * map_width + x].neighbours.push_back(&nodes[(y    ) * map_width + (x - 1));
				if (x < map_width - 1)
					nodes[y * map_width + x].neighbours.push_back(&nodes[(y    ) * map_width + (x + 1));
			}

		//manual position of start/end
		startNode = &nodes[0];
		endNode = &nodes[(map_height / 2) * map_width + map_width / 2];

		return true;
	}

	bool solve_astar()
	{
		//reseting graph
		for (int x = 0; x < map_width; x++)
			for (int y = 0; y < map_height; y++)
			{
				nodes[y * map_width + x].b_Visited = false;
				nodes[y * map_width + x].global_goal = INFINITY;
				nodes[y * map_width + x].local_goal= INFINITY;
				nodes[y * map_width + x].parent = nullptr;
			}
		
		auto distance = [](Node* a, Node* b)
		{
			return sqrtl((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
		};

		auto heuristic = [distance](Node* a, Node* b) { return distance(a, b); };

		//starting conditions
		Node* currentNode = startNode;
		startNode->local_goal = 0.0;
		startNode->global_goal= heuristic(startNode, endNode);

		// Following code refers to 

	}

private:
	struct Node
	{
		bool b_Obstacle = false;
		bool b_Visited = false;
		double global_goal;
		double local_goal;
		int x;
		int y;

		std::vector<Node*> neighbours;
		Node* parent;
	};

	Node* nodes = nullptr;
	Node* startNode = nullptr;
	Node* endNode = nullptr;

	int map_width = 16;
	int map_height = 16;



};

