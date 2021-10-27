#include "circuit.h"

circuit::circuit(const std::string &input_string) {
    input_parser parser(input_string);
    auto [edge_resistance, edge_emf] = fill_circuit_graph(parser);
    resize_subgraph_number();
    fill_circuit_matrices(edge_resistance, edge_emf);
}

void circuit::resize_subgraph_number() {
    conductivity_matrix.resize(circuit_subgraphs.size());
    emf_matrix.resize(circuit_subgraphs.size());
    incidence_matrix.resize(circuit_subgraphs.size());
    edge_current_matrix.resize(circuit_subgraphs.size());
}

std::pair<std::vector<double>, std::vector<double>> circuit::fill_circuit_graph(input_parser &parser) {
    std::vector<double> edge_resistance, edge_emf;
    while (!parser.is_eof()) {
        parser.make_iteration();
        auto [incoming, outcoming, resistance, emf] = parser.get_current_edge_info();
        check_vertices(incoming, outcoming);
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
    circuit_subgraphs = circuit_graph.get_coherent_subgraphs();

    return {edge_resistance, edge_emf};
}

void circuit::check_vertices(graph::vertex incoming, graph::vertex outcoming) const {
    if (!incoming || !outcoming)
        throw ZeroVerticesException();
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
    for (size_t subgraph_index = 0; subgraph_index < circuit_subgraphs.size(); subgraph_index++) {
        fill_subgraph_matrices(edge_resistance, edge_voltage, subgraph_index);
    }
}

void
circuit::fill_subgraph_matrices(const std::vector<double> &edge_resistance, const std::vector<double> &edge_voltage,
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

std::pair<std::vector<double>, std::vector<double>>
circuit::fill_subgraph_edges_parameters(size_t subgraph_index, const std::vector<double>& edge_resistance,
                                        const std::vector<double>& edge_voltage) const {
    std::vector<double> subgraph_edge_resistance(circuit_subgraphs[subgraph_index].get_edge_number()),
                        subgraph_edge_voltage(circuit_subgraphs[subgraph_index].get_edge_number());

    for (size_t edge_index = 0; edge_index < circuit_subgraphs[subgraph_index].get_edge_number(); edge_index++) {
        graph::edge circuit_graph_edge = circuit_graph.get_subgraph_mapping_edge({subgraph_index, edge_index});
        subgraph_edge_resistance[edge_index] = edge_resistance[circuit_graph_edge];
        subgraph_edge_voltage[edge_index] = edge_voltage[circuit_graph_edge];
    }

    return {subgraph_edge_resistance, subgraph_edge_voltage};
}

void circuit::fill_incidence_matrix(size_t vertex_number, size_t edge_number, size_t subgraph_index) {
    incidence_matrix[subgraph_index] = matrix<double>(vertex_number - 1, edge_number);
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

void circuit::fill_emf_matrix(const std::vector<double> &edge_voltage, size_t edge_number, size_t subgraph_index) {
    emf_matrix[subgraph_index] = matrix<double>(edge_number, 1);
    for (graph::edge edge = 0; edge < edge_voltage.size(); edge++) {
        emf_matrix[subgraph_index][edge][0] = edge_voltage[edge];
    }
}

void circuit::fill_conductivity_matrix(const std::vector<double> &edge_resistance, size_t edge_number, size_t subgraph_index) {
    conductivity_matrix[subgraph_index] = matrix<double>(edge_number, edge_number);
    for (graph::edge edge = 0; edge < edge_resistance.size(); edge++) {
        conductivity_matrix[subgraph_index][edge][edge] = 1 / edge_resistance[edge];
    }
}

void circuit::calculate_edge_current() {
    for (size_t subgraph_index = 0; subgraph_index < circuit_subgraphs.size(); subgraph_index++) {
        auto flow_matrix = incidence_matrix[subgraph_index] * conductivity_matrix[subgraph_index] *
                            incidence_matrix[subgraph_index].transpose();
        flow_matrix = flow_matrix.inverse() * incidence_matrix[subgraph_index] * conductivity_matrix[subgraph_index] *
                        emf_matrix[subgraph_index];
        auto voltage_matrix = incidence_matrix[subgraph_index].transpose() * flow_matrix;
        edge_current_matrix[subgraph_index] = conductivity_matrix[subgraph_index] *
                                                (emf_matrix[subgraph_index] - voltage_matrix);
    }
    calculate_loop_current();
}

void circuit::calculate_loop_current() {
    for (size_t loop_index = 0; loop_index < loop_resistance.size(); loop_index++) {
        loop_current.push_back(loop_emf[loop_index] / loop_resistance[loop_index]);
    }
}

std::string circuit::get_edge_current_answer() {
    std::stringstream answer;
    modify_loops_current_answer(answer);
    for (size_t subgraph_index = 0; subgraph_index < circuit_subgraphs.size(); subgraph_index++) {
        modify_subgraph_edge_current_answer(answer, subgraph_index);
    }
    std::string answer_string = answer.str();
    answer_string.pop_back();
    return answer_string;
}

void circuit::modify_subgraph_edge_current_answer(std::stringstream &answer, size_t subgraph_index) {
    if (!conductivity_matrix[subgraph_index].get_row_number())
        return;
    for (graph::edge edge_index = 0; edge_index < edge_current_matrix[subgraph_index].get_row_number(); edge_index++) {
        modify_single_edge_current_answer(answer, edge_index, subgraph_index);
        answer << '\n';
    }
}

void circuit::modify_loops_current_answer(std::stringstream &answer) const {
    auto& loops = circuit_graph.get_loops();
    for (size_t vertex_index = 0; vertex_index < loops.size(); vertex_index++) {
        answer << loops[vertex_index] + 1 << " -- " << loops[vertex_index] + 1 << ": " <<
               loop_current[vertex_index] << " A;" << '\n';
    }
}

void circuit::modify_single_edge_current_answer(std::stringstream &answer, graph::edge edge, size_t subgraph_index) {
    auto [incoming_vertex, outcoming_vertex] = circuit_graph.get_tied_vertices(circuit_graph.get_subgraph_mapping_edge({subgraph_index, edge}));
    answer << incoming_vertex + 1 << " -- " << outcoming_vertex + 1 << ": " <<
           edge_current_matrix[subgraph_index][edge][0] << " A;";
}
