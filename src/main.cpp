#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

class UnionFind {
private:
    std::vector<int> parent, rank;
public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    int find(int u) {
        if (u != parent[u]) {
            parent[u] = find(parent[u]);
        }
        return parent[u];
    }

    void unionSets(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU != rootV) {
            if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            } else if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            } else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }
};

struct Edge {
    int u, v, year, length, cost;
    Edge(int _u, int _v, int _year, int _length, int _cost) 
        : u(_u), v(_v), year(_year), length(_length), cost(_cost) {}
};

class TemporalGraph {
private:
    int numVertices;
    std::vector<std::vector<Edge>> adjList;

public:
    TemporalGraph(int n) : numVertices(n), adjList(n) {}

    void addEdge(int u, int v, int year, int length, int cost) {
        adjList[u].push_back(Edge(u, v, year, length, cost));
        adjList[v].push_back(Edge(v, u, year, length, cost));
    }

    std::vector<int> dijkstra(int start) {
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

    int findMinYear() {
        std::vector<int> dist(numVertices, INT_MAX);
        std::vector<int> yearReached(numVertices, INT_MAX);
        yearReached[0] = 0;

        std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, std::greater<std::tuple<int, int, int>>> pq;
        pq.push({0, 0, 0}); // {current distance, current vertex, year}

        while (!pq.empty()) {
            auto [currentDistance, u, currentYear] = pq.top();
            pq.pop();

            if (currentDistance > dist[u]) continue;

            for (const auto& edge : adjList[u]) {
                int v = edge.v;
                int year = edge.year;
                int length = edge.length;

                if (currentDistance + length < dist[v] || (currentDistance + length == dist[v] && year < yearReached[v])) {
                    dist[v] = currentDistance + length;
                    yearReached[v] = year;
                    pq.push({dist[v], v, year});
                }
            }
        }

        int latestYear = *std::max_element(yearReached.begin(), yearReached.end());
        return latestYear;
    }

    int findMinYearAllReachable() {
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

    int findMinCost() {
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
};


int main() {
    int N, M;
    std::cin >> N >> M;
    
    TemporalGraph graph(N);
    
    for (int i = 0; i < M; ++i) {
        int u, v, year, length, cost;
        std::cin >> u >> v >> year >> length >> cost;
        graph.addEdge(u - 1, v - 1, year, length, cost); // Ajustando para 0-based index
    }
    
    std::vector<int> distances = graph.dijkstra(0); // Supondo que o palácio real está no vértice 0
    
    for (int dist : distances) {
        std::cout << dist << std::endl;
    }
    
    int minYear = graph.findMinYear();
    std::cout << minYear << std::endl;
    
    int minYearAllReachable = graph.findMinYearAllReachable();
    std::cout << minYearAllReachable << std::endl;
    
    int minCost = graph.findMinCost();
    std::cout << minCost << std::endl;
    
    return 0;
}
