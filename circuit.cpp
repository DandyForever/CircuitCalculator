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
        edge_resistance.push_back(resistance);
        circuit_graph.add_edge(outcoming - 1, incoming - 1);
        edge_emf.push_back(parser.is_emf_included() ? emf : 0.);
    }

    return {edge_resistance, edge_emf};
}

void circuit::fill_circuit_matrices(const std::vector<double> &edge_resistance,
                                    const std::vector<double> &edge_voltage) {
    size_t vertex_number = circuit_graph.get_vertex_number();
    size_t edge_number = circuit_graph.get_edge_number();
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
}


std::string circuit::get_edge_current_answer() {
    std::stringstream answer;
    for (graph::edge edge_index = 0; edge_index < edge_current_matrix.get_row_number(); edge_index++) {
        auto [incoming_vertex, outcoming_vertex] = circuit_graph.get_tied_vertices(edge_index);
        answer << incoming_vertex + 1 << " -- " << outcoming_vertex + 1 << ": " <<
                    -edge_current_matrix[edge_index][0] << " A; ";
    }
    return answer.str();
}
// 1 -- 2, 4.0; 1 -- 3, 10.0; 1 -- 4, 2.0; -12.0V; 2 -- 3, 60.0; 2 -- 4, 22.0; 3 -- 4, 5.0;
// 1 -- 2: -0.442958 A; 1 -- 3: -0.631499 A; 1 -- 4: 1.07446 A; 2 -- 3: -0.0757193 A; 2 -- 4: -0.367239 A; 3 -- 4: -0.707219 A;