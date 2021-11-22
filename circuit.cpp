#include "circuit.h"

template<>
void circuit<double>::fill_parameters(double &resistance, double &emf, const input_parser::state &state) {
    resistance = state.resistance.real();
    emf = state.emf.real();
}

template <>
void circuit<double>::modify_answer_loop(std::stringstream &answer, size_t vertex_index) const {
    answer << loop_current[vertex_index] << " A";
}

template <>
void circuit<double>::modify_answer_edge(std::stringstream &answer, graph::edge edge, size_t subgraph_index) {
    answer << edge_current_matrix[subgraph_index][edge][0] << " A";
}