#ifndef EDGE_HPP
#define EDGE_HPP

struct Edge {
    int u, v, year, length, cost;
    Edge(int _u, int _v, int _year, int _length, int _cost) 
        : u(_u), v(_v), year(_year), length(_length), cost(_cost) {}
};

#endif // EDGE_HPP
