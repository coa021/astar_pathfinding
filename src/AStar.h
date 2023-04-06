#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <list>
#include <random>
#include <functional>


class AStar
{
public:
	bool randomBool() {
		return ((double)rand() / (double)RAND_MAX) < 0.30;

	}

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
				nodes[y * map_width + x].value = ' ';



				if (randomBool())
				{
					nodes[y * map_width + x].value = 'x';
					nodes[y * map_width + x].b_Obstacle = true;
				}
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
					nodes[y * map_width + x].neighbours.push_back(&nodes[(y - 1) * map_width + x]);
				if (y < map_height - 1)
					nodes[y * map_width + x].neighbours.push_back(&nodes[(y + 1) * map_width + x]);
				if (x > 0)
					nodes[y * map_width + x].neighbours.push_back(&nodes[(y)*map_width + (x - 1)]);
				if (x < map_width - 1)
					nodes[y * map_width + x].neighbours.push_back(&nodes[(y)*map_width + (x + 1)]);
			}

		//manual position of start/end
		startNode = &nodes[16];
		startNode->value = 'S';

		endNode = &nodes[map_width*map_height-1];
		endNode->value = 'E';
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
				nodes[y * map_width + x].local_goal = INFINITY;
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
		startNode->global_goal = heuristic(startNode, endNode);

		// Following refers to open list from writeup
		// Open list has reachable/walkable squares adjacent to the starting point
		std::list<Node*> open_list;
		open_list.push_back(startNode);

		while (!open_list.empty() && currentNode != endNode)
		{
			//sort so the lowest score is first
			open_list.sort([](const Node* lhs, const Node* rhs) { return lhs->global_goal < rhs->global_goal; });

			//get rid of nodes that are already visited
			while (!open_list.empty() && open_list.front()->b_Visited)
				open_list.pop_front();

			//if there are no valid nodes, abort
			if (open_list.empty())
				break;

			currentNode = open_list.front();
			currentNode->b_Visited = true;

			//checking neighbours
			for (auto neigbour : currentNode->neighbours)
			{
				//if not visited and not an obstacle, add to open_list
				if (!neigbour->b_Visited && !neigbour->b_Obstacle)
					open_list.push_back(neigbour);

				//calculate potentional lowest parent distance
				double possible_lower_goal = currentNode->local_goal + distance(currentNode, neigbour);

				//if this is shorter path, update neigbour to use this node
				if (possible_lower_goal < neigbour->local_goal)
				{
					neigbour->parent = currentNode;
					neigbour->local_goal = possible_lower_goal;

					//update the score
					neigbour->global_goal = neigbour->local_goal + heuristic(neigbour, endNode);
				}
			}

		}
		return true;
	}


	void connect_path()
	{
		if (endNode != nullptr)
		{
			Node* p = endNode;
			while (p->parent != nullptr)
			{
				if(p!=endNode)	// pointless quick hax not to delete endNode's value xd
					p->value = 'o';
				p = p->parent;
			}
		}
	}


	void print_graph()
	{
		for (int x = 0; x < map_width; x++)
		{
			for (int y = 0; y < map_height; y++)
			{
				std::cout << nodes[y * map_width + x].value << " ";

			}
			std::cout << "\n";
		}
	}

	void debug_print_graph()
	{
		for (int x = 0; x < map_width; x++)
		{
			for (int y = 0; y < map_height; y++)
			{
				//for (auto n : nodes[y * map_width + x].neighbours)
				std::cout << y * map_width + x << " ";

			}
			std::cout << "\n";
		}
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
		char value;

		std::vector<Node*> neighbours;
		Node* parent;
	};

	Node* nodes = nullptr;
	Node* startNode = nullptr;
	Node* endNode = nullptr;

	int map_width = 16;
	int map_height = 16;



};

