#ifndef CIRCUITS_CIRCUIT_H
#define CIRCUITS_CIRCUIT_H

#include <sstream>
#include "matrix.h"
#include "graph.h"
#include "input_parser.h"

class circuit {
public:
    circuit(const std::string& input_string);

    void calculate_edge_current();
    std::string get_edge_current_answer();

    class ZeroResistanceException : public std::exception {
        const char* what () const throw () { return "Zero resistance not supported!"; }
    };

private:
    matrix<double> conductivity_matrix{};
    matrix<double> emf_matrix{};
    matrix<double> incidence_matrix{};
    matrix<double> edge_current_matrix{};
    graph circuit_graph;
    std::vector<double> loop_resistance;
    std::vector<double> loop_emf;
    std::vector<double> loop_current;

    void fill_conductivity_matrix(const std::vector<double> &edge_resistance, size_t edge_number);
    void fill_emf_matrix(const std::vector<double> &edge_voltage, size_t edge_number);
    void fill_incidence_matrix(size_t vertex_number, size_t edge_number);
    void fill_circuit_matrices(const std::vector<double> &edge_resistance,
                               const std::vector<double> &edge_voltage);

    std::pair<std::vector<double>, std::vector<double>> fill_circuit_graph(input_parser &parser);

    void modify_single_edge_current_answer(std::stringstream &answer, graph::edge edge_index);
    void check_resistance(double resistance) const;

    void calculate_loop_current();

    void modify_loops_current_answer(std::stringstream &answer) const;
};


#endif //CIRCUITS_CIRCUIT_H
