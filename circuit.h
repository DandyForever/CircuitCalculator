#ifndef CIRCUITS_CIRCUIT_H
#define CIRCUITS_CIRCUIT_H

#include <sstream>
#include "matrix.h"
#include "graph.h"
#include "input_parser.h"
#include "complex_number.h"

class circuit {
public:
    circuit(const std::string& input_string);

    void calculate_edge_current();
    std::string get_edge_current_answer();

    class ZeroResistanceException : public std::exception {
        const char* what () const throw () { return "Zero resistances was replaced with 1e-9 Ohm"; }
    };

    class EmptyInputException : public std::exception {
        const char* what () const throw () { return "Invalid input: Empty input was parsed!"; }
    };
    class ZeroVerticesException : public std::exception {
        const char* what () const throw () { return "Invalid input: Unsupported zero vertex was parsed!"; }
    };

private:
    bool is_ac = false;
    std::vector<matrix<complex_number>> conductivity_matrix;
    std::vector<matrix<complex_number>> emf_matrix;
    std::vector<matrix<complex_number>> incidence_matrix;
    std::vector<matrix<complex_number>> edge_current_matrix;
    graph circuit_graph;
    std::vector<graph> circuit_subgraphs;
    std::vector<complex_number> loop_resistance;
    std::vector<complex_number> loop_emf;
    std::vector<complex_number> loop_current;

    void fill_conductivity_matrix(const std::vector<complex_number> &edge_resistance, size_t edge_number,
                                  size_t subgraph_index);
    void fill_emf_matrix(const std::vector<complex_number> &edge_voltage, size_t edge_number, size_t subgraph_index);
    void fill_incidence_matrix(size_t vertex_number, size_t edge_number, size_t subgraph_index);
    void fill_circuit_matrices(const std::vector<complex_number> &edge_resistance,
                               const std::vector<complex_number> &edge_voltage);

    std::pair<std::vector<complex_number>, std::vector<complex_number>> fill_circuit_graph(input_parser &parser);

    void modify_single_edge_current_answer(std::stringstream &answer, graph::edge edge_index, size_t subgraph_index);
    bool check_resistance(complex_number& resistance) const;

    void calculate_loop_current();

    void modify_loops_current_answer(std::stringstream &answer) const;

    void check_empty_input(const std::vector<complex_number> &edge_resistance) const;

    void check_vertices(graph::vertex incoming, graph::vertex outcoming) const;

    std::pair<std::vector<complex_number>, std::vector<complex_number>>
    fill_subgraph_edges_parameters(size_t subgraph_index, const std::vector<complex_number> &edge_resistance,
                                   const std::vector<complex_number> &edge_voltage) const;

    void modify_subgraph_edge_current_answer(std::stringstream &answer, size_t subgraph_index);

    void fill_subgraph_matrices(const std::vector<complex_number> &edge_resistance,
                                const std::vector<complex_number> &edge_voltage,
                                size_t subgraph_index);

    void resize_subgraph_number();

    const double EPS = 1e-9;

    matrix<complex_number> CalculateFlowMatrix(size_t subgraph_index);
};


#endif //CIRCUITS_CIRCUIT_H
