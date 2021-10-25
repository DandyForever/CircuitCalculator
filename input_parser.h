#ifndef CIRCUITS_INPUT_PARSER_H
#define CIRCUITS_INPUT_PARSER_H


#include <string>
#include <sstream>
#include "graph.h"

class input_parser {
public:
    struct state {
        graph::vertex incoming_vertex;
        graph::vertex outcoming_vertex;
        double resistance;
        double emf;
    };
    input_parser(const std::string& input_string): input(input_string) {};
    void make_iteration();
    bool is_eof() { return input.peek() == EOF; }
    state get_current_edge_info() const { return current_edge_info; }
    bool is_emf_included() const { return is_previous_has_emf; }

    class InvalidInputException : std::exception {
        const char* what () const throw () { return "Unexpected character in input line!"; }
    };

private:
    std::stringstream input;
    state current_edge_info;
    bool is_previous_has_emf = true;

    void fill_outcoming_vertex();
    void fill_incoming_vertex();
    void fill_resistance();
    void fill_emf();

    const char EMF_dimension = 'V';
    const char Vertex_separator = '-';
    const char Resistance_separator = ',';
    const char Characteristic_separator = ';';

    bool check_symbol(const char separator);
    void check_numeric();
};


#endif //CIRCUITS_INPUT_PARSER_H
