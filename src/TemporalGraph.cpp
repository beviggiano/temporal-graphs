#include "TemporalGraph.hpp"
#include "UnionFind.hpp"
#include <algorithm>
#include <climits>

TemporalGraph::TemporalGraph(int n) : numVertices(n), adjList(n) {}

void TemporalGraph::addEdge(int u, int v, int year, int length, int cost) {
    adjList[u].push_back(Edge(u, v, year, length, cost));
    adjList[v].push_back(Edge(v, u, year, length, cost));
}

std::vector<int> TemporalGraph::dijkstra(int start) {
    std::vector<int> dist(numVertices, INT_MAX);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (const auto& edge : adjList[u]) {
            int v = edge.v;
            int length = edge.length;

            if (dist[u] + length < dist[v]) {
                dist[v] = dist[u] + length;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}

int TemporalGraph::findMinYear() {
    std::vector<int> minYear(numVertices, INT_MAX);
    std::vector<int> dist(numVertices, INT_MAX);
    dist[0] = 0;
    minYear[0] = 0;

    std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, std::greater<std::tuple<int, int, int>>> pq;
    pq.push({0, 0, 0}); // {current year, current vertex, current distance}

    while (!pq.empty()) {
        auto [currentYear, u, currentDistance] = pq.top();
        pq.pop();

        if (currentYear > minYear[u]) continue;

        for (const auto& edge : adjList[u]) {
            int v = edge.v;
            int year = edge.year;
            int length = edge.length;

            if (year < minYear[v] || (year == minYear[v] && currentDistance + length < dist[v])) {
                minYear[v] = year;
                dist[v] = currentDistance + length;
                pq.push({year, v, dist[v]});
            }
        }
    }

    int latestYear = *std::max_element(minYear.begin(), minYear.end());
    return latestYear;
}


int TemporalGraph::findMinYearAllReachable() {
    std::vector<Edge> edges;
    for (int u = 0; u < numVertices; ++u) {
        for (const auto& edge : adjList[u]) {
            edges.push_back(edge);
        }
    }

    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.year < b.year;
    });

    UnionFind uf(numVertices);
    int yearAllReachable = 0;

    for (const auto& edge : edges) {
        uf.unionSets(edge.u, edge.v);
        if (uf.find(0) == uf.find(edge.v)) {
            yearAllReachable = edge.year;
        }

        bool allConnected = true;
        for (int i = 1; i < numVertices; ++i) {
            if (uf.find(0) != uf.find(i)) {
                allConnected = false;
                break;
            }
        }
        if (allConnected) {
            yearAllReachable = edge.year;
            break;
        }
    }

    return yearAllReachable;
}

int TemporalGraph::findMinCost() {
    std::vector<Edge> edges;
    for (int u = 0; u < numVertices; ++u) {
        for (const auto& edge : adjList[u]) {
            if (u < edge.v) {
                edges.push_back(edge);
            }
        }
    }

    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.cost < b.cost;
    });

    UnionFind uf(numVertices);
    int minCost = 0;
    int edgesUsed = 0;

    for (const auto& edge : edges) {
        if (uf.find(edge.u) != uf.find(edge.v)) {
            uf.unionSets(edge.u, edge.v);
            minCost += edge.cost;
            edgesUsed++;
            if (edgesUsed == numVertices - 1) break;
        }
    }

    return minCost;
}
