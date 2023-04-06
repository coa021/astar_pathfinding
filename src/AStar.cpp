#include "AStar.h"


struct AStar::Node
{
	bool b_Obstacle{ false };
	bool b_Visited{ false };
	double global_goal;
	double local_goal;
	int x;
	int y;
	char value;

	std::vector<NodePtr> neighbours;	//NodePtr is alias for unique_ptr
	NodePtr parent;
};

AStar::NodePtr AStar::create_node()
{
	return std::make_shared<Node>();
}


bool AStar::randomBool() {
	return ((double)rand() / (double)RAND_MAX) < 0.30;
}

bool AStar::initialize()
{
	//reserve column worth of shared_ptr's i guess
	nodes.reserve(map_height);

	for (int x = 0; x < map_width; x++)
	{
		nodes.emplace_back();
		nodes.back().reserve(map_width);	//is this necessary? can i just use reserve?
		for (int y = 0; y < map_height; y++)
		{
			NodePtr node = create_node();
			node->x = x;
			node->y = y;
			node->b_Obstacle = false;	//already set by default but whatever
			node->b_Visited = false;
			node->parent = nullptr;
			node->value = ' ';

			if (randomBool())
			{
				node->value = 'x';
				node->b_Obstacle = true;
			}
			nodes.back().push_back(std::move(node));	//here to, do i need back()?
		}
	}

	/*  
		creating connections
		there will be no possibility for diagonal connections
		only hor/ver 
	*/
	for (int x = 0; x < map_width; x++)
		for (int y = 0; y < map_height; y++)
		{

			auto& node = nodes.at(y).at(x);
			// checking for bounds
			// if node is on the edges
			// theres no possibility to go in some directions
			
			if (y > 0)
				node->neighbours.push_back(nodes.at(y - 1).at(x));
			if (y < map_height - 1)
				node->neighbours.push_back(nodes.at(y + 1).at(x));
			if (x > 0)
				node->neighbours.push_back(nodes.at(y).at(x - 1));
			if (x < map_width - 1)
				node->neighbours.push_back(nodes.at(y).at(x + 1));
		}

	//manual position of start/end
	startNode = nodes.at(0).at(0);
	startNode->value = 'S';

	endNode = nodes.at(map_width - 1).at(map_height - 1);	//check this, should change values if they are set wrong
	endNode->value = 'E';
	return true;
}

bool AStar::solve_astar()
{
	//reseting graph
	for (int x = 0; x < map_width; x++)
		for (int y = 0; y < map_height; y++)
		{
			nodes.at(y).at(x)->b_Visited = false;
			nodes.at(y).at(x)->global_goal = INFINITY;	//should i use INFINITY here, should i change it to more standard type defined in num_limits?
			nodes.at(y).at(x)->local_goal = INFINITY;
			nodes.at(y).at(x)->parent = nullptr;
		}

	auto distance = [](NodePtr a, NodePtr b)
	{
		return sqrtl((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
	};

	auto heuristic = [distance](NodePtr a, NodePtr b) { return distance(a, b); };
	//starting conditions
	NodePtr currentNode = startNode;
	startNode->local_goal = 0.0;
	startNode->global_goal = heuristic(startNode, endNode);

	// Following refers to open list from writeup
	// Open list has reachable/walkable squares adjacent to the starting point

	//is this the problem??
	std::list<NodePtr> open_list;
	open_list.push_back(startNode);
	
	while (!open_list.empty() && currentNode != endNode)
	{
		//sort so the lowest score is first
		open_list.sort([](const NodePtr lhs, const NodePtr rhs) { return lhs->global_goal < rhs->global_goal; });

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


void AStar::connect_path()
{
	if (endNode != nullptr)
	{
		NodePtr p = endNode;
		while (p->parent != nullptr)
		{
			if (p != endNode)	// pointless quick hax not to delete endNode's value xd
				p->value = 'o';
			p = p->parent;
		}
	}
}


void AStar::print_graph()
{
	for (int x = 0; x < map_width; x++)
	{
		for (int y = 0; y < map_height; y++)
		{
			std::cout << nodes.at(y).at(x)->value << " ";
		}
		std::cout << "\n";
	}
}

void AStar::debug_print_graph()
{
	for (int x = 0; x < map_width; x++)
	{
		for (int y = 0; y < map_height; y++)
		{
			std::cout << y * map_width + x << " ";

		}
		std::cout << "\n";
	}
}