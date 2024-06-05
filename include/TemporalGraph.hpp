#ifndef TEMPORALGRAPH_HPP
#define TEMPORALGRAPH_HPP

#include <vector>
#include <queue>
#include "Edge.hpp"

class TemporalGraph {
private:
    int numVertices;
    std::vector<std::vector<Edge>> adjList;

public:
    TemporalGraph(int n);
    void addEdge(int u, int v, int year, int length, int cost);
    std::vector<int> dijkstra(int start);
    int findMinYear();
    int findMinCost();
};

#endif // TEMPORALGRAPH_HPP
