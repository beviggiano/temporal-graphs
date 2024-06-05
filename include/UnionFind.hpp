#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

#include <vector>

class UnionFind {
private:
    std::vector<int> parent, rank;
public:
    UnionFind(int n);
    int find(int u);
    void unionSets(int u, int v);
};

#endif // UNIONFIND_HPP
