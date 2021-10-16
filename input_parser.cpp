#include "input_parser.h"

void input_parser::make_iteration() {
    fill_outcoming_vertex();
    input.ignore(2);
    fill_incoming_vertex();
    input.ignore();
    fill_resistance();
    input.ignore();
    fill_emf();
    input >> std::ws;
}

void input_parser::fill_emf() {
    input >> std::ws >> current_edge_info.emf >> std::ws;
    if (input.get() == EMF_dimension) {
        is_previous_has_emf = true;
        input.ignore();
        return;
    }

    is_previous_has_emf = false;
}

void input_parser::fill_resistance() { input >> std::ws >> current_edge_info.resistance >> std::ws; }

void input_parser::fill_incoming_vertex() { input >> std::ws >> current_edge_info.incoming_vertex >> std::ws; }

void input_parser::fill_outcoming_vertex() {
    if (!is_previous_has_emf) {
        current_edge_info.outcoming_vertex = static_cast<size_t>(current_edge_info.emf);
        return;
    }

    input >> std::ws >> current_edge_info.outcoming_vertex >> std::ws;
}
