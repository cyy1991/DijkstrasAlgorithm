//
// ShortestPathAlgo.cpp - Contains class used compute shortest path between vertices
//

#include <functional>
#include <algorithm>

#include "ShortestPathAlgo.hpp"


//
// Path struct used to contain the distance and path to a vertex from the source vertex
//
struct Path
{
	int vertex;
	int distance;
	std::string path;

	bool operator<(const Path& val) const
	{
		return vertex < val.vertex;
	}
};

//
// GreaterDistComp contains a comparator used in creating a min heap
//
struct GreaterDistComp
{
	bool operator()(const Path& lhs,const Path& rhs) const
	{
		return lhs.distance > rhs.distance;
	}
};

void ShortestPathAlgo::PrintShortestPath(
	const Graph& graph,
	int sourceVertex,
	int endVertex)
{
	//
	// Closed set contains all explored vertices
	// Open set contains all unexplored vertics
	//
	std::vector<Path> closedSet;
	std::vector<Path> openSet;

	//
	// Start at source vertex with distance zero
	//
	Path source = {sourceVertex, 0};

	//
	// Vertex id starts at 1
	//
	source.path = std::to_string(sourceVertex + 1);

	openSet.push_back(source);

	//
	// Set all other vertex distances to infinity
	//
	for (int i = 1; i < graph.GetNumberOfVertices(); ++i)
	{
		openSet.push_back({i, INFINITY_INT});
	}

	std::vector<int> neighbours;

	while (false == openSet.empty())
	{
		//
		// Use std::make_heap to create a min heap
		//
		std::make_heap(openSet.begin(), openSet.end(), GreaterDistComp());

		//
		// Pop min value from min heap
		//
		std::pop_heap(openSet.begin(), openSet.end(), GreaterDistComp());

		Path minPath = openSet.back();

		openSet.pop_back();

		closedSet.push_back(minPath);

		graph.GetNeighbours(minPath.vertex, neighbours);

		//
		// Iterate through neighbours and update the distances in the open set
		//
		for (int i = 0; i < neighbours.size(); ++i)
		{
			if (graph.IsAdjacent(minPath.vertex, neighbours[i]))
			{
				Path neighbour = {neighbours[i], graph.GetEdgeValue(minPath.vertex, neighbours[i])};

				for (auto& it : openSet)
				{
					if (neighbour.vertex == it.vertex &&
						(minPath.distance + neighbour.distance) < it.distance)
					{
						it.path = minPath.path + " -> " + std::to_string(it.vertex + 1);

						it.distance = minPath.distance + neighbour.distance;
					}
				}
			}
		}
	}

	//
	// Sort vertices based on vertex number
	//
	std::sort(closedSet.begin(), closedSet.end());

	//
	// Erase source vertex because we don't need to print that
	//
	closedSet.erase(closedSet.begin());

	//
	// Print all shortest paths from the source vertex and the total distances.
	// Only print path if the end vertex has been explored.
	// Print all paths if the end vertex is -1 (by default)
	//
	for (auto& explored : closedSet)
	{
		if (INFINITY_INT != explored.distance &&
			(endVertex == explored.vertex ||
			 endVertex == -1))
		{
			std::cout << "Total Distance from vertex 1 to " << explored.vertex + 1 << " is " << explored.distance << " (Shortest Path: " << explored.path << ")" << std::endl;
		}
	}

	std::cout << std::endl;

}
