#ifndef CIRCUITS_CIRCUIT_H
#define CIRCUITS_CIRCUIT_H

#include <sstream>
#include "matrix.h"
#include "graph.h"
#include "input_parser.h"
#include "input_preworker.h"

template <typename T>
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
    std::vector<matrix<T>> conductivity_matrix;
    std::vector<matrix<T>> emf_matrix;
    std::vector<matrix<T>> incidence_matrix;
    std::vector<matrix<T>> edge_current_matrix;
    graph circuit_graph;
    std::vector<graph> circuit_subgraphs;
    std::vector<T> loop_resistance;
    std::vector<T> loop_emf;
    std::vector<T> loop_current;

    void fill_conductivity_matrix(const std::vector<T> &edge_resistance, size_t edge_number,
                                  size_t subgraph_index);
    void fill_emf_matrix(const std::vector<T> &edge_voltage, size_t edge_number, size_t subgraph_index);
    void fill_incidence_matrix(size_t vertex_number, size_t edge_number, size_t subgraph_index);
    void fill_circuit_matrices(const std::vector<T> &edge_resistance,
                               const std::vector<T> &edge_voltage);

    std::pair<std::vector<T>, std::vector<T>> fill_circuit_graph(input_parser &parser);

    void modify_single_edge_current_answer(std::stringstream &answer, graph::edge edge_index, size_t subgraph_index);
    void modify_answer_edge(std::stringstream &answer, graph::edge edge, size_t subgraph_index);
    bool check_resistance(T& resistance) const;

    void calculate_loop_current();

    void modify_loops_current_answer(std::stringstream &answer) const;
    void modify_answer_loop(std::stringstream &answer, size_t vertex_index) const;

    void check_empty_input(const std::vector<T> &edge_resistance) const;

    void check_vertices(graph::vertex incoming, graph::vertex outcoming) const;

    std::pair<std::vector<T>, std::vector<T>>
    fill_subgraph_edges_parameters(size_t subgraph_index, const std::vector<T> &edge_resistance,
                                   const std::vector<T> &edge_voltage) const;

    void modify_subgraph_edge_current_answer(std::stringstream &answer, size_t subgraph_index);

    void fill_subgraph_matrices(const std::vector<T> &edge_resistance,
                                const std::vector<T> &edge_voltage,
                                size_t subgraph_index);

    void resize_subgraph_number();

    const double EPS = 1e-9;//-9

    matrix<T> CalculateFlowMatrix(size_t subgraph_index);

    void fill_parameters(T &resistance, T &emf, const input_parser::state &state);
};

template <typename T>
circuit<T>::circuit(const std::string &input_string) {
    input_parser parser(input_string);
    is_ac = parser.is_alternating_current();
    auto [edge_resistance, edge_emf] = fill_circuit_graph(parser);
    resize_subgraph_number();
    fill_circuit_matrices(edge_resistance, edge_emf);
}

template <typename T>
void circuit<T>::resize_subgraph_number() {
    conductivity_matrix.resize(circuit_subgraphs.size());
    emf_matrix.resize(circuit_subgraphs.size());
    incidence_matrix.resize(circuit_subgraphs.size());
    edge_current_matrix.resize(circuit_subgraphs.size());
}

template <typename T>
std::pair<std::vector<T>, std::vector<T>>
circuit<T>::fill_circuit_graph(input_parser &parser) {
    std::vector<T> edge_resistance, edge_emf;
    bool is_zero_resistance_exist = false;
    while (!parser.is_eof()) {
        parser.make_iteration();
        graph::vertex incoming, outcoming;
        T resistance, emf;
        auto state = parser.get_current_edge_info();
        incoming = state.incoming_vertex;
        outcoming = state.outcoming_vertex;
        fill_parameters(resistance, emf, state);
        check_vertices(incoming, outcoming);
        is_zero_resistance_exist |= check_resistance(resistance);
        if (circuit_graph.add_edge(outcoming - 1, incoming - 1)) {
            edge_emf.emplace_back(parser.is_emf_included() ? emf : 0.);
            edge_resistance.emplace_back(resistance);
        } else {
            loop_resistance.emplace_back(resistance);
            loop_emf.emplace_back(parser.is_emf_included() ? emf : 0.);
        }
    }
    check_empty_input(edge_resistance);
    circuit_subgraphs = circuit_graph.get_coherent_subgraphs();
    if (is_zero_resistance_exist) {
        std::cout << "Zero resistances were replaced with " << EPS << " Ohm" << std::endl;
    }

    return {edge_resistance, edge_emf};
}

template<typename T>
void circuit<T>::fill_parameters(T &resistance, T &emf, const input_parser::state &state) {
    resistance = state.resistance;
    emf = state.emf;
}

template<>
void circuit<double>::fill_parameters(double &resistance, double &emf, const input_parser::state &state);

template <typename T>
void circuit<T>::check_vertices(graph::vertex incoming, graph::vertex outcoming) const {
    if (!incoming || !outcoming)
        throw ZeroVerticesException();
}

template <typename T>
void circuit<T>::check_empty_input(const std::vector<T> &edge_resistance) const {
    if (edge_resistance.empty() && loop_resistance.empty())
        throw EmptyInputException();
}

template <typename T>
bool circuit<T>::check_resistance(T& resistance) const {
    if (std::abs(resistance) < EPS) {
        resistance += EPS;
        return true;
    }
    return false;
}

template <typename T>
void circuit<T>::fill_circuit_matrices(const std::vector<T> &edge_resistance,
                                    const std::vector<T> &edge_voltage) {
    for (size_t subgraph_index = 0; subgraph_index < circuit_subgraphs.size(); subgraph_index++) {
        fill_subgraph_matrices(edge_resistance, edge_voltage, subgraph_index);
    }
}

template <typename T>
void circuit<T>::fill_subgraph_matrices(const std::vector<T> &edge_resistance,
                                const std::vector<T> &edge_voltage,
                                size_t subgraph_index) {
    size_t vertex_number = circuit_subgraphs[subgraph_index].get_vertex_number();
    size_t edge_number = circuit_subgraphs[subgraph_index].get_edge_number();
    auto [subgraph_edge_resistance, subgraph_edge_voltage] =
    fill_subgraph_edges_parameters(subgraph_index, edge_resistance, edge_voltage);

    if (!edge_number)
        return;

    fill_conductivity_matrix(subgraph_edge_resistance, edge_number, subgraph_index);
    fill_emf_matrix(subgraph_edge_voltage, edge_number, subgraph_index);
    fill_incidence_matrix(vertex_number, edge_number, subgraph_index);
}

template <typename T>
std::pair<std::vector<T>, std::vector<T>>
circuit<T>::fill_subgraph_edges_parameters(size_t subgraph_index, const std::vector<T>& edge_resistance,
                                        const std::vector<T>& edge_voltage) const {
    std::vector<T> subgraph_edge_resistance(circuit_subgraphs[subgraph_index].get_edge_number()),
            subgraph_edge_voltage(circuit_subgraphs[subgraph_index].get_edge_number());

    for (size_t edge_index = 0; edge_index < circuit_subgraphs[subgraph_index].get_edge_number(); edge_index++) {
        graph::edge circuit_graph_edge = circuit_graph.get_subgraph_mapping_edge({subgraph_index, edge_index});
        subgraph_edge_resistance[edge_index] = edge_resistance[circuit_graph_edge];
        subgraph_edge_voltage[edge_index] = edge_voltage[circuit_graph_edge];
    }

    return {subgraph_edge_resistance, subgraph_edge_voltage};
}

template <typename T>
void circuit<T>::fill_incidence_matrix(size_t vertex_number, size_t edge_number, size_t subgraph_index) {
    incidence_matrix[subgraph_index] = matrix<T>(vertex_number - 1, edge_number);
    for (graph::vertex vertex = 0; vertex < vertex_number - 1; vertex++) {
        auto current_incoming_edges = circuit_subgraphs[subgraph_index].get_incoming_edges(vertex);
        for (auto edge : current_incoming_edges) {
            incidence_matrix[subgraph_index][vertex][edge] = 1.;
        }
        auto current_outcoming_edges = circuit_subgraphs[subgraph_index].get_outcoming_edges(vertex);
        for (auto edge : current_outcoming_edges) {
            incidence_matrix[subgraph_index][vertex][edge] = -1.;
        }
    }
}

template <typename T>
void circuit<T>::fill_emf_matrix(const std::vector<T> &edge_voltage, size_t edge_number,
                              size_t subgraph_index) {
    emf_matrix[subgraph_index] = matrix<T>(edge_number, 1);
    for (graph::edge edge = 0; edge < edge_voltage.size(); edge++) {
        emf_matrix[subgraph_index][edge][0] = edge_voltage[edge];
    }
}

template <typename T>
void circuit<T>::fill_conductivity_matrix(const std::vector<T> &edge_resistance, size_t edge_number,
                                       size_t subgraph_index) {
    conductivity_matrix[subgraph_index] = matrix<T>(edge_number, edge_number);
    for (graph::edge edge = 0; edge < edge_resistance.size(); edge++) {
        conductivity_matrix[subgraph_index][edge][edge] = T(1) / edge_resistance[edge];
    }
}

template <typename T>
void circuit<T>::calculate_edge_current() {
    for (size_t subgraph_index = 0; subgraph_index < circuit_subgraphs.size(); subgraph_index++) {
        auto flow_matrix = CalculateFlowMatrix(subgraph_index);
        auto voltage_matrix = incidence_matrix[subgraph_index].transpose() * flow_matrix;
        edge_current_matrix[subgraph_index] = conductivity_matrix[subgraph_index] *
                                              (emf_matrix[subgraph_index] - voltage_matrix);
    }
    calculate_loop_current();
}

template <typename T>
matrix<T> circuit<T>::CalculateFlowMatrix(size_t subgraph_index) {
    auto flow_matrix = incidence_matrix[subgraph_index] * conductivity_matrix[subgraph_index] *
                       incidence_matrix[subgraph_index].transpose();
    try {
        flow_matrix = flow_matrix.inverse() * incidence_matrix[subgraph_index] * conductivity_matrix[subgraph_index] *
                      emf_matrix[subgraph_index];
    } catch (typename matrix<T>::ZeroDivisionException&) {
        std::cout << "Resonant circuit was updated via adding " << EPS << " Ohm resistance\n";
        conductivity_matrix[subgraph_index][0][0] = 1. / (1. / conductivity_matrix[subgraph_index][0][0] + EPS);
        return CalculateFlowMatrix(subgraph_index);
    }
    return flow_matrix;
}

template <typename T>
void circuit<T>::calculate_loop_current() {
    for (size_t loop_index = 0; loop_index < loop_resistance.size(); loop_index++) {
        loop_current.push_back(loop_emf[loop_index] / loop_resistance[loop_index]);
    }
}

template <typename T>
std::string circuit<T>::get_edge_current_answer() {
    std::stringstream answer;
    modify_loops_current_answer(answer);
    for (size_t subgraph_index = 0; subgraph_index < circuit_subgraphs.size(); subgraph_index++) {
        modify_subgraph_edge_current_answer(answer, subgraph_index);
    }
    std::string answer_string = answer.str();
    answer_string.pop_back();
    return answer_string;
}

template <typename T>
void circuit<T>::modify_subgraph_edge_current_answer(std::stringstream &answer, size_t subgraph_index) {
    if (!conductivity_matrix[subgraph_index].get_row_number())
        return;
    for (graph::edge edge_index = 0; edge_index < edge_current_matrix[subgraph_index].get_row_number(); edge_index++) {
        modify_single_edge_current_answer(answer, edge_index, subgraph_index);
        answer << '\n';
    }
}

template <typename T>
void circuit<T>::modify_loops_current_answer(std::stringstream &answer) const {
    auto& loops = circuit_graph.get_loops();
    for (size_t vertex_index = 0; vertex_index < loops.size(); vertex_index++) {
        answer << loops[vertex_index] + 1 << " -- " << loops[vertex_index] + 1 << ": ";
        modify_answer_loop(answer, vertex_index);
        answer << ";\n";
    }
}

template <typename T>
void circuit<T>::modify_answer_loop(std::stringstream &answer, size_t vertex_index) const {
    answer << std::abs(loop_current[vertex_index]) << " A, " <<
           std::arg(loop_current[vertex_index]) / M_PI * 180;
}

template <>
void circuit<double>::modify_answer_loop(std::stringstream &answer, size_t vertex_index) const;

template <typename T>
void circuit<T>::modify_single_edge_current_answer(std::stringstream &answer, graph::edge edge, size_t subgraph_index) {
    auto [incoming_vertex, outcoming_vertex] =
    circuit_graph.get_tied_vertices(circuit_graph.get_subgraph_mapping_edge({subgraph_index, edge}));
    answer << incoming_vertex + 1 << " -- " << outcoming_vertex + 1 << ": ";
    modify_answer_edge(answer, edge, subgraph_index);
    answer << ';';
}

template <typename T>
void circuit<T>::modify_answer_edge(std::stringstream &answer, graph::edge edge, size_t subgraph_index) {
    answer << std::abs(edge_current_matrix[subgraph_index][edge][0]) << " A, " <<
           std::arg(edge_current_matrix[subgraph_index][edge][0]) / M_PI * 180;
}

template <>
void circuit<double>::modify_answer_edge(std::stringstream &answer, graph::edge edge, size_t subgraph_index);

template <typename T>
void calculate_circuit(std::istream& input = std::cin, std::ostream& output = std::cout) {
    std::string inp;
    std::string input_string;
    while (std::getline(input, inp)) {
        inp.push_back(' ');
        input_string.insert(input_string.length(), inp);
    }
    circuit<T> cir(input_string);
    cir.calculate_edge_current();
    output << cir.get_edge_current_answer();
}

template <typename T>
void calculate_complex_circuit(std::istream& input = std::cin, const std::string& file_name = "input", std::ostream& output = std::cout) {
    input_preworker preworker(input, file_name, 0);
    preworker.perform_prework();
    circuit<T> cir(preworker.get_output());
    cir.calculate_edge_current();
    output << cir.get_edge_current_answer();
}

#endif //CIRCUITS_CIRCUIT_H
