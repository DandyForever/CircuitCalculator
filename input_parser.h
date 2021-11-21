#ifndef CIRCUITS_INPUT_PARSER_H
#define CIRCUITS_INPUT_PARSER_H


#include <complex>
#include <string>
#include <sstream>
#include "graph.h"

class input_parser {
public:
    struct state {
        graph::vertex incoming_vertex;
        graph::vertex outcoming_vertex;
        std::complex<double> resistance;
        std::complex<double> emf;
    };
    input_parser(const std::string& input_string): input(input_string) {
        is_ac = input_string.find('C') != std::string::npos;
    };
    void make_iteration();
    bool is_eof() { return input.peek() == EOF; }
    bool is_alternating_current() const { return is_ac; }
    state get_current_edge_info() const { return current_edge_info; }
    bool is_emf_included() const { return is_previous_has_emf; }

    class InvalidInputException : std::exception {
        const char* what () const throw () { return "Unexpected character in input line!"; }
    };

private:
    std::stringstream input;
    bool is_ac = false;
    double temporary_emf = 0.;
    state current_edge_info;
    bool is_previous_has_emf = true;

    void fill_outcoming_vertex();
    void fill_incoming_vertex();
    void fill_resistance();
    void fill_emf();

    void fill_resistance_ac();
    void fill_capacity();
    void fill_inductance();
    void fill_emf_ac();

    double get_emf_phase();

    const char EMF_dimension = 'V';
    const char Resistance_dimension = 'R';
    const char Capacity_dimension = 'C';
    const char Inductance_dimension = 'L';
    const char Vertex_separator = '-';
    const char Input_separator = ',';
    const char Characteristic_separator = ';';

    bool check_symbol(const char separator);
    void check_numeric();

    void make_ac_iteration();

    double get_impedance();
};


#endif //CIRCUITS_INPUT_PARSER_H
