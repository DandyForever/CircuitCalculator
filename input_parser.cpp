#include <iostream>
#include "input_parser.h"

void input_parser::make_iteration() {
    fill_outcoming_vertex();
    check_symbol(Vertex_separator);
    check_symbol(Vertex_separator);
    fill_incoming_vertex();
    check_symbol(Resistance_separator);
    fill_resistance();
    check_symbol(Characteristic_separator);
    fill_emf();
    input >> std::ws;
}

bool input_parser::check_symbol(const char separator) {
    if (input.peek() == separator) {
        input.ignore(1);
        return true;
    }
    return false;
}

void input_parser::check_numeric() {
    if (!isdigit(input.peek()) && input.peek() != EOF)
        throw InvalidInputException(input.peek());
}

void input_parser::fill_emf() {
    input >> std::ws;
    check_numeric();
    input >> current_edge_info.emf >> std::ws;
    if (check_symbol(EMF_dimension)) {
        is_previous_has_emf = true;
        check_symbol(Characteristic_separator);
        input >> std::ws;
        return;
    }

    is_previous_has_emf = false;
}

void input_parser::fill_resistance() {
    input >> std::ws;
    check_numeric();
    input >> current_edge_info.resistance >> std::ws;
}

void input_parser::fill_incoming_vertex() {
    input >> std::ws;
    check_numeric();
    input >> current_edge_info.incoming_vertex >> std::ws;
}

void input_parser::fill_outcoming_vertex() {
    if (!is_previous_has_emf) {
        current_edge_info.outcoming_vertex = static_cast<size_t>(current_edge_info.emf);
        return;
    }

    input >> std::ws;
    check_numeric();
    input >> current_edge_info.outcoming_vertex >> std::ws;
}
