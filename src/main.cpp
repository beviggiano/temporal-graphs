#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <tuple>

// Classe UnionFind implementa a estrutura de união e busca (Union-Find) para a gestão de conjuntos disjuntos
class UnionFind {
private:
    std::vector<int> parent, rank;  // Vetores para armazenar os pais e as alturas dos nós

public:
    // Construtor inicializa o vetor 'parent' com cada elemento sendo seu próprio pai e 'rank' com zero
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    // Método para encontrar o representante do conjunto de um elemento com compressão de caminho
    int find(int u) {
        if (u != parent[u]) {
            parent[u] = find(parent[u]);
        }
        return parent[u];
    }

    // Método para unir dois conjuntos
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

// Estrutura Edge representa uma aresta em um grafo temporal
struct Edge {
    int u, v, year, length, cost;  // Vértices da aresta, ano, comprimento e custo
    Edge(int _u, int _v, int _year, int _length, int _cost) 
        : u(_u), v(_v), year(_year), length(_length), cost(_cost) {}
};

// Classe TemporalGraph implementa um grafo temporal com métodos para encontrar várias propriedades do grafo
class TemporalGraph {
private:
    int numVertices;  // Número de vértices no grafo
    std::vector<std::vector<Edge>> adjList;  // Lista de adjacência para armazenar as arestas

public:
    // Construtor inicializa o grafo com n vértices
    TemporalGraph(int n) : numVertices(n), adjList(n) {}

    // Método para adicionar uma aresta ao grafo
    void addEdge(int u, int v, int year, int length, int cost) {
        adjList[u].push_back(Edge(u, v, year, length, cost));
        adjList[v].push_back(Edge(v, u, year, length, cost));
    }

    // Método que implementa o algoritmo de Dijkstra para encontrar as menores distâncias a partir de um vértice de início
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

    // Método para encontrar o ano mínimo em que todos os vértices podem ser alcançados a partir do vértice 0
    int findMinYear() {
        std::vector<int> dist(numVertices, INT_MAX);
        std::vector<int> yearReached(numVertices, INT_MAX);
        yearReached[0] = 0;

        typedef std::tuple<int, int, int> T;
        std::priority_queue<T, std::vector<T>, std::greater<T>> pq;
        pq.push(T(0, 0, 0)); // {current distance, current vertex, year}

        while (!pq.empty()) {
            T top = pq.top();
            int currentDistance = std::get<0>(top);
            int u = std::get<1>(top);
            int currentYear = std::get<2>(top);
            pq.pop();

            if (currentDistance > dist[u]) continue;

            for (const auto& edge : adjList[u]) {
                int v = edge.v;
                int year = edge.year;
                int length = edge.length;

                if (currentDistance + length < dist[v] || (currentDistance + length == dist[v] && year < yearReached[v])) {
                    dist[v] = currentDistance + length;
                    yearReached[v] = year;
                    pq.push(T(dist[v], v, year));
                }
            }
        }

        int latestYear = *std::max_element(yearReached.begin(), yearReached.end());
        return latestYear;
    }

    // Método para encontrar o ano mínimo em que todos os vértices podem ser alcançados de qualquer vértice
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

    // Método para encontrar o custo mínimo necessário para conectar todos os vértices
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
    std::cin >> N >> M;  // Lê o número de vértices (N) e arestas (M)
    
    TemporalGraph graph(N);  // Cria um grafo temporal com N vértices
    
    for (int i = 0; i < M; ++i) {
        int u, v, year, length, cost;
        std::cin >> u >> v >> year >> length >> cost;  // Lê os dados de cada aresta
        graph.addEdge(u - 1, v - 1, year, length, cost);  // Adiciona a aresta ao grafo (ajustando índices para 0-based)
    }
    
    // Executa o algoritmo de Dijkstra a partir do vértice 0
    std::vector<int> distances = graph.dijkstra(0); 
    
    // Imprime as distâncias mínimas a partir do vértice 0
    for (int dist : distances) {
        std::cout << dist << std::endl;
    }
    
    // Encontra e imprime o ano mínimo em que todos os vértices são alcançáveis a partir do vértice 0
    int minYear = graph.findMinYear();
    std::cout << minYear << std::endl;
    
    // Encontra e imprime o ano mínimo em que todos os vértices são alcançáveis a partir de qualquer vértice
    int minYearAllReachable = graph.findMinYearAllReachable();
    std::cout << minYearAllReachable << std::endl;
    
    // Encontra e imprime o custo mínimo necessário para conectar todos os vértices
    int minCost = graph.findMinCost();
    std::cout << minCost << std::endl;
    
    return 0;
}
