#include <iostream>
#include "input_parser.h"

void input_parser::make_iteration() {
    fill_outcoming_vertex();
    check_symbol(Vertex_separator);
    check_symbol(Vertex_separator);
    fill_incoming_vertex();
    check_symbol(Input_separator);
    fill_resistance();
    if (is_ac) {
        make_ac_iteration();
    }
    check_symbol(Characteristic_separator);
    fill_emf();
    input >> std::ws;
}

void input_parser::make_ac_iteration() {
    check_symbol(Resistance_dimension);
    check_symbol(Characteristic_separator);
    fill_capacity();
    check_symbol(Characteristic_separator);
    fill_inductance();
}

bool input_parser::check_symbol(const char separator) {
    if (input.peek() == separator) {
        input.ignore(1);
        return true;
    }
    return false;
}

void input_parser::check_numeric() {
    if (input.peek() == '-') {
        input.get();
        if (!isdigit(input.peek())) {
            input.unget();
        } else {
            input.unget();
            return;
        }
    }
    if (!isdigit(input.peek()) && input.peek() != EOF) {
        std::cout << "Unexpected character: " << static_cast<char>(input.peek()) << std::endl;
        throw InvalidInputException();
    }
}

void input_parser::fill_emf() {
    input >> std::ws;
    check_numeric();
    input >> temporary_emf >> std::ws;
    if (check_symbol(EMF_dimension)) {
        is_previous_has_emf = true;
        double phase = get_emf_phase();
        current_edge_info.emf = std::polar(temporary_emf, phase);
        check_symbol(Characteristic_separator);
        input >> std::ws;
        return;
    }

    is_previous_has_emf = false;
}

void input_parser::fill_resistance() {
    current_edge_info.resistance = get_impedance();
}

double input_parser::get_impedance() {
    input >> std::ws;
    check_numeric();
    double impedance = 0.;
    input >> impedance >> std::ws;
    return impedance;
}

void input_parser::fill_incoming_vertex() {
    input >> std::ws;
    check_numeric();
    input >> current_edge_info.incoming_vertex >> std::ws;
}

void input_parser::fill_outcoming_vertex() {
    if (!is_previous_has_emf) {
        current_edge_info.outcoming_vertex = static_cast<size_t>(temporary_emf);
        return;
    }

    input >> std::ws;
    check_numeric();
    input >> current_edge_info.outcoming_vertex >> std::ws;
}

void input_parser::fill_capacity() {
    current_edge_info.resistance -= std::complex(0., get_impedance());
    check_symbol(Capacity_dimension);
}

void input_parser::fill_inductance() {
    current_edge_info.resistance += std::complex(0., get_impedance());
    check_symbol(Inductance_dimension);
}

double input_parser::get_emf_phase() {
    if (!is_ac)
        return 0;
    check_symbol(Input_separator);
    input >> std::ws;
    check_numeric();
    double phase = 0.;
    input >> phase >> std::ws;
    return -phase;
}