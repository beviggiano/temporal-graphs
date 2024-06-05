#include <iostream>
#include "TemporalGraph.hpp"

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
    
    int minYearAllReachable = graph.findMinYear();
    std::cout << minYearAllReachable << std::endl;
    
    int minCost = graph.findMinCost();
    std::cout << minCost << std::endl;
    
    return 0;
}
