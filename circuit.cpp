#include "circuit.h"

circuit::circuit(const std::string &input_string):
        conductivity_matrix(0, 0),
        emf_matrix(0, 0),
        incidence_matrix(0, 0)
{
    std::stringstream input(input_string);
    size_t num;
    graph::vertex output_vertex, input_vertex;
    double voltage, resistance;
    bool is_voltage = true;
    char c = '\0';
    std::vector<double> edge_resistance;
    std::vector<double> edge_voltage;
    while (c != EOF) {
        if (!is_voltage) {
            num = static_cast<size_t>(voltage);
        } else {
            input >> std::ws >> num >> std::ws;
        }
        input_vertex = num;
        input.get(c);
        input.get(c);
        input >> std::ws >> num >> std::ws;
        output_vertex = num;
        input.get(c);
        input >> std::ws >> resistance >> std::ws;
        edge_resistance.push_back(resistance);
        input.get(c);
        input >> std::ws >> voltage >> std::ws;
        input.get(c);
        if (c == 'V') {
            edge_voltage.push_back(voltage);
            is_voltage = true;
            input.get(c);
        } else {
            edge_voltage.push_back(0.);
            is_voltage = false;
        }
        input >> std::ws;
        c = input.peek();
        circuit_graph.add_edge(input_vertex, output_vertex);
    }
    size_t vertex_number = circuit_graph.get_vertex_number();
    size_t edge_number = circuit_graph.get_edge_number();
    conductivity_matrix = matrix<double>(edge_number, edge_number);
    for (graph::edge edge = 0; edge < edge_resistance.size(); edge++) {
        conductivity_matrix[edge][edge] = 1 / edge_resistance[edge];
    }
    emf_matrix = matrix<double>(edge_number, 1);
    for (graph::edge edge = 0; edge < edge_voltage.size(); edge++) {
        emf_matrix[edge][0] = edge_voltage[edge];
    }
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

void circuit::calculate() {
    auto flow_matrix = incidence_matrix;
    flow_matrix *= conductivity_matrix;
    flow_matrix *= incidence_matrix.transpose();
    flow_matrix = flow_matrix.inverse();
    flow_matrix *= incidence_matrix;
    flow_matrix *= conductivity_matrix;
    flow_matrix *= emf_matrix;
    auto potential_matrix = incidence_matrix.transpose();
    potential_matrix *= flow_matrix;
    auto total_voltage_matrix = emf_matrix;
    total_voltage_matrix -= potential_matrix;
    auto answer_matrix = conductivity_matrix;
    answer_matrix *= total_voltage_matrix;
    for (graph::edge edge = 0; edge < answer_matrix.get_row_number(); edge++) {
        std::cout << answer_matrix[edge][0] << std::endl;
    }
}

// 1 -- 2, 4.0; 1 -- 3, 10.0; 1 -- 4, 2.0; -12.0V; 2 -- 3, 60.0; 2 -- 4, 22.0; 3 -- 4, 5.0;