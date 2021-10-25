#include "circuit.h"

circuit::circuit(const std::string &input_string) {
    input_parser parser(input_string);
    auto [edge_resistance, edge_emf] = fill_circuit_graph(parser);
    fill_circuit_matrices(edge_resistance, edge_emf);
}

std::pair<std::vector<double>, std::vector<double>> circuit::fill_circuit_graph(input_parser &parser) {
    std::vector<double> edge_resistance, edge_emf;
    while (!parser.is_eof()) {
        parser.make_iteration();
        auto [incoming, outcoming, resistance, emf] = parser.get_current_edge_info();
        check_resistance(resistance);
        if (circuit_graph.add_edge(outcoming - 1, incoming - 1)) {
            edge_emf.push_back(parser.is_emf_included() ? emf : 0.);
            edge_resistance.push_back(resistance);
        } else {
            loop_resistance.push_back(resistance);
            loop_emf.push_back(parser.is_emf_included() ? emf : 0.);
        }
    }
    check_empty_input(edge_resistance);
    circuit_graph.check_connectivity();

    return {edge_resistance, edge_emf};
}

void circuit::check_empty_input(const std::vector<double> &edge_resistance) const {
    if (edge_resistance.empty() && loop_resistance.empty())
        throw EmptyInputException();
}

void circuit::check_resistance(double resistance) const {
    if (std::abs(resistance) < 1e-9)
        throw ZeroResistanceException();
}

void circuit::fill_circuit_matrices(const std::vector<double> &edge_resistance,
                                    const std::vector<double> &edge_voltage) {
    size_t vertex_number = circuit_graph.get_vertex_number();
    size_t edge_number = circuit_graph.get_edge_number();
    if (!edge_number)
        return;
    fill_conductivity_matrix(edge_resistance, edge_number);
    fill_emf_matrix(edge_voltage, edge_number);
    fill_incidence_matrix(vertex_number, edge_number);
}

void circuit::fill_incidence_matrix(size_t vertex_number, size_t edge_number) {
    incidence_matrix = matrix<double>(vertex_number - 1, edge_number);
    for (graph::vertex vertex = 0; vertex < vertex_number - 1; vertex++) {
        auto current_incoming_edges = circuit_graph.get_incoming_edges(vertex);
        for (auto edge : current_incoming_edges) {
            incidence_matrix[vertex][edge] = 1.;
        }
        auto current_outcoming_edges = circuit_graph.get_outcoming_edges(vertex);
        for (auto edge : current_outcoming_edges) {
            incidence_matrix[vertex][edge] = -1.;
        }
    }
}

void circuit::fill_emf_matrix(const std::vector<double> &edge_voltage, size_t edge_number) {
    emf_matrix = matrix<double>(edge_number, 1);
    for (graph::edge edge = 0; edge < edge_voltage.size(); edge++) {
        emf_matrix[edge][0] = edge_voltage[edge];
    }
}

void circuit::fill_conductivity_matrix(const std::vector<double> &edge_resistance, size_t edge_number) {
    conductivity_matrix = matrix<double>(edge_number, edge_number);
    for (graph::edge edge = 0; edge < edge_resistance.size(); edge++) {
        conductivity_matrix[edge][edge] = 1 / edge_resistance[edge];
    }
}

void circuit::calculate_edge_current() {
    auto flow_matrix = incidence_matrix * conductivity_matrix * incidence_matrix.transpose();
    flow_matrix = flow_matrix.inverse() * incidence_matrix * conductivity_matrix * emf_matrix;
    auto voltage_matrix = incidence_matrix.transpose() * flow_matrix;
    edge_current_matrix = conductivity_matrix * (emf_matrix - voltage_matrix);
    calculate_loop_current();
}

void circuit::calculate_loop_current() {
    for (size_t loop_index = 0; loop_index < loop_resistance.size(); loop_index++) {
        loop_current.push_back(loop_emf[loop_index] / loop_resistance[loop_index]);
    }
}

std::string circuit::get_edge_current_answer() {
    std::stringstream answer;
    modify_loops_current_answer(answer);\
    if (!conductivity_matrix.get_row_number())
        return answer.str();
    modify_single_edge_current_answer(answer, 0);
    for (graph::edge edge_index = 1; edge_index < edge_current_matrix.get_row_number(); edge_index++) {
        answer << '\n';
        modify_single_edge_current_answer(answer, edge_index);
    }
    return answer.str();
}

void circuit::modify_loops_current_answer(std::stringstream &answer) const {
    auto& loops = circuit_graph.get_loops();
    for (size_t vertex_index = 0; vertex_index < loops.size(); vertex_index++) {
        answer << loops[vertex_index] + 1 << " -- " << loops[vertex_index] + 1 << ": " <<
               loop_current[vertex_index] << " A;" << '\n';
    }
}

void circuit::modify_single_edge_current_answer(std::stringstream &answer, graph::edge edge) {
    auto [incoming_vertex, outcoming_vertex] = circuit_graph.get_tied_vertices(edge);
    answer << incoming_vertex + 1 << " -- " << outcoming_vertex + 1 << ": " <<
           edge_current_matrix[edge][0] << " A;";
}
