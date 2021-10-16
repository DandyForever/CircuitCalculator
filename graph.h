#ifndef CIRCUITS_GRAPH_H
#define CIRCUITS_GRAPH_H

#include <vector>

class graph {
public:
    typedef size_t edge;
    typedef size_t vertex;
    typedef std::vector<edge> edges;

    void add_edge(vertex first, vertex second);
    size_t get_edge_number() const { return edges_vertices.size(); }
    size_t get_vertex_number() const { return incoming_edges.size(); }
    const edges& get_incoming_edges(vertex vertex_) const { return incoming_edges[vertex_]; }
    const edges& get_outcoming_edges(vertex vertex_) const { return outcoming_edges[vertex_]; }
private:
    typedef std::pair<vertex, vertex> tied_vertices;

    std::vector<edges> incoming_edges;
    std::vector<edges> outcoming_edges;
    std::vector<tied_vertices> edges_vertices;
};


#endif //CIRCUITS_GRAPH_H
