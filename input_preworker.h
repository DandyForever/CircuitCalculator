#ifndef CIRCUITS_INPUT_PREWORKER_H
#define CIRCUITS_INPUT_PREWORKER_H

#include <istream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include "scanner.h"

class input_preworker {
public:
    struct element {
        std::string name;
        std::vector<std::string> parameters;
        std::set<std::string> variables;
        std::vector<std::string> internal_variables;
        std::vector<std::vector<std::string>> edges;
    };

    explicit input_preworker(std::istream& input, const std::string& file, int max_vertex);
    void perform_prework();
    std::string get_output() const { return output.str(); }
    auto& get_tokens() const { return tokens; }
    auto& get_elements() const { return elements; }

    class UnknownElementException : std::exception {
        const char* what () const throw () { return "Element was not defined"; }
    };

    class InvalidParameterNumberException : std::exception {
        const char* what () const throw () { return "Parameter number of element substitution does not match the definition"; }
    };

    class UnexpectedTokenException : std::exception {
        const char* what () const throw () { return "Unexpected token"; }
    };

    class UndefinedVariableException : std::exception {
        const char* what () const throw () { return "Element parameter is undefined"; }
    };

    class MultipleDefinitionException : std::exception {
        const char* what () const throw () { return "Element is already defined"; }
    };

    class FileOpenException : std::exception {
        const char* what () const throw () { return "File was not opened"; }
    };

private:
    std::string file_name_;
    std::string file_path;
    std::stringstream output;
    std::vector<std::vector<std::string>> tokens;
    std::map<std::string, element> elements;
    int vertex_number = 0;
    size_t line_index = 0;

    void create_element();

    void manage_include(const std::vector<std::string> &token_line);
    void substitute_element(const std::vector<std::string> &token_line);
    void add_edge(std::vector<std::string> &token_line);
    std::map<std::string, std::string>
    map_parameters_values(const std::vector<std::string> &token_line, element &element_);
    void update_output_with_element_edges(element &element_,
                                     const std::map<std::string, std::string> &parameter_value_mapping);
    void check_element_existance(const std::string &element_name) const;
    void check_parameters_number(const element &element_, const std::vector<std::string> &element_values) const;
    void check_parameter_defined(const std::vector<std::string> &token_line, const element &element_) const;
    void print_message_prefix() const;

    const std::set<std::string> Punctuation{";", ",", "(", ")"};

    void tokenize(std::istream &input);

    void tokenize_include(const Scanner &scanner);

    void tokenize_define(const Scanner &scanner);

    void tokenize_internal(const Scanner &scanner);

    void tokenize_element_edge(const Scanner &scanner);

    void tokenize_edge(const Scanner &scanner);

    void tokenize_substitution(const Scanner &scanner);

    void tokenize_two_token_line(const Scanner &scanner, int shift);

    std::stringstream simplify_element_line(const Scanner &scanner) const;

    std::stringstream simplify_edge_line(const Scanner &scanner) const;

    void tokenize_edge_attributes(std::stringstream &edge_line_, std::vector<std::string> &edge_tokens, int shift);

    void check_element_redefinition(std::string name);
};


#endif //CIRCUITS_INPUT_PREWORKER_H
