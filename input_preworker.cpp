#include <fstream>
#include <algorithm>
#include "input_preworker.h"

input_preworker::input_preworker(std::istream& input, const std::string& file, int max_vertex) {
    file_name_ = file;
    vertex_number = max_vertex;
    if (file_name_ != "input") {
        auto position = file_name_.find_last_of('/');
        if (position != std::string::npos) {
            file_path = file_name_.substr(0, position + 1);
        }
    }
    tokenize(input);
    for (auto& token_line : tokens) {
        for (auto& token : token_line)
            std::cout << '\'' << token << '\'';
        std::cout << std::endl;
    }
}

void input_preworker::tokenize(std::istream &input) {
    Scanner scanner(&input);
    for (;;) {
        switch (scanner.get_next_token()) {
            case Scanner::Token::END:
                break;
            case Scanner::Token::INCLUDE_LINE:
                {
                    tokenize_include(scanner);
                    continue;
                }
            case Scanner::Token::DEFINE_LINE:
                {
                    tokenize_define(scanner);
                    continue;
                }
            case Scanner::Token::INTERNAL_LINE:
                {
                    tokenize_internal(scanner);
                    continue;
                }
            case Scanner::Token::ELEMENT_LINE:
                {
                    tokenize_element_edge(scanner);
                    continue;
                }
            case Scanner::Token::EDGE_LINE:
                {
                    tokenize_edge(scanner);
                    continue;
                }
            case Scanner::Token::CALL_LINE:
                {
                    tokenize_substitution(scanner);
                    continue;
                }
            case Scanner::Token::INVALID_LINE:
                {
                    std::cout << "Invalid line " << scanner.lineno() << ": " << scanner.YYText() << std::endl;
                    throw UnexpectedTokenException();
                }
            case Scanner::Token::EOL:
                {
                    tokens.emplace_back(1);
                    continue;
                }
            default:
                break;
        }
        break;
    }
}

void input_preworker::tokenize_substitution(const Scanner &scanner) {
    std::stringstream call_line_ = simplify_element_line(scanner);
    std::vector<std::string> call_tokens(1);
    call_line_ >> call_tokens[0] >> std::ws;
    while (true) {
        int vertex = 0;
        call_line_ >> vertex >> std::ws;
        if (!vertex)
            break;
        vertex_number = std::max(vertex_number, vertex);
        call_tokens.push_back(std::to_string(vertex));
    }
    tokens.push_back(call_tokens);
}

void input_preworker::tokenize_edge(const Scanner &scanner) {
    std::stringstream edge_line_ = simplify_edge_line(scanner);
    std::vector<std::string> edge_tokens(5);
    int incoming_vertex, outcoming_vertex;
    edge_line_ >> outcoming_vertex >> std::ws >> incoming_vertex >> std::ws;
    vertex_number = std::max(vertex_number, outcoming_vertex);
    vertex_number = std::max(vertex_number, incoming_vertex);
    edge_tokens[0] = std::to_string(outcoming_vertex);
    edge_tokens[1] = std::to_string(incoming_vertex);
    tokenize_edge_attributes(edge_line_, edge_tokens, 0);
}

void input_preworker::tokenize_edge_attributes(std::stringstream &edge_line_, std::vector<std::string> &edge_tokens,
                                               int shift) {
    edge_line_ >> edge_tokens[shift + 2] >> std::ws >> edge_tokens[shift + 3] >> std::ws >> edge_tokens[shift + 4] >>
               std::ws;
    if (!edge_line_.eof()) {
        edge_tokens.resize(shift + 7);
        edge_line_ >> edge_tokens[shift + 5] >> std::ws >> edge_tokens[shift + 6];
    }
    tokens.push_back(edge_tokens);
}

void input_preworker::tokenize_element_edge(const Scanner &scanner) {
    std::stringstream element_line_ = simplify_edge_line(scanner);
    std::vector<std::string> element_tokens(6);
    element_line_ >> std::ws >> element_tokens[1] >> std::ws  >> element_tokens[2] >> std::ws;
    tokenize_edge_attributes(element_line_, element_tokens, 1);
}

std::stringstream input_preworker::simplify_edge_line(const Scanner &scanner) const {
    std::string element_line(scanner.YYText());
    auto comma_it = element_line.find(',');
    std::replace(element_line.begin(), element_line.begin() + comma_it, '-', ' ');
    std::replace(element_line.begin(), element_line.end(), ',', ' ');
    std::replace(element_line.begin(), element_line.end(), ';', ' ');
    std::stringstream element_line_(element_line);
    return element_line_;
}

void input_preworker::tokenize_internal(const Scanner &scanner) {
    tokenize_two_token_line(scanner, 1);
}

void input_preworker::tokenize_two_token_line(const Scanner &scanner, int shift) {
    std::stringstream internal_line(scanner.YYText());
    std::vector<std::string> internal_tokens(shift + 2);
    internal_line >> std::ws >> internal_tokens[shift] >> std::ws >> internal_tokens[shift + 2];
    tokens.push_back(internal_tokens);
}

void input_preworker::tokenize_define(const Scanner &scanner) {
    std::stringstream define_line_ = simplify_element_line(scanner);
    std::vector<std::string> define_tokens(2);
    define_line_ >> define_tokens[0] >> std::ws >> define_tokens[1] >> std::ws;
    while (true) {
        std::string variable;
        define_line_ >> variable >> std::ws;
        if (variable.empty())
            break;
        define_tokens.push_back(variable);
    }
    tokens.push_back(define_tokens);
}

std::stringstream input_preworker::simplify_element_line(const Scanner &scanner) const {
    std::string define_line(scanner.YYText());
    std::replace(define_line.begin(), define_line.end(), '(', ' ');
    std::replace(define_line.begin(), define_line.end(), ')', ' ');
    std::replace(define_line.begin(), define_line.end(), ',', ' ');
    std::stringstream define_line_(define_line);
    return define_line_;
}

void input_preworker::tokenize_include(const Scanner &scanner) {
    tokenize_two_token_line(scanner, 0);
}

void input_preworker::perform_prework() {
    for (line_index = 0; line_index < tokens.size(); line_index++) {
        auto &token_line = tokens[line_index];
        while (token_line[0] == "define") {
            create_element();
            if (line_index == tokens.size())
                return;
        }
        if (token_line[0] == "include") {
            manage_include(token_line);
        } else if (token_line.size() == 1 && token_line[0].empty()){
            continue;
        } else if (!std::isdigit(token_line[0][0])) {
            substitute_element(token_line);
        } else {
            add_edge(token_line);
        }
    }
}

void input_preworker::add_edge(std::vector<std::string> &token_line) {
    for (auto& token : token_line)
        output << token << ' ';
    output << '\n';
}

void input_preworker::substitute_element(const std::vector<std::string> &token_line) {
    std::string element_name = token_line[0];
    check_element_existance(element_name);
    auto &element_ = elements[element_name];
    std::map<std::string, std::string> parameter_value_mapping = map_parameters_values(token_line, element_);
    update_output_with_element_edges(element_, parameter_value_mapping);
}

void input_preworker::check_element_existance(const std::string &element_name) const {
    if (!elements.count(element_name)) {
        print_message_prefix();
        std::cout << "Unknown element " << element_name << std::endl;
        throw UnknownElementException();
    }
}

void input_preworker::print_message_prefix() const { std::cout << file_name_ << " line " << line_index + 1 << ": "; }

void input_preworker::update_output_with_element_edges(input_preworker::element &element_,
                                                       const std::map<std::string, std::string> &parameter_value_mapping) {
    for (auto &element_edges: element_.edges) {
        output << parameter_value_mapping.at(element_edges[1]) << " " <<
               parameter_value_mapping.at(element_edges[2]);
        for (size_t edge_attribute = 3; edge_attribute < element_edges.size(); edge_attribute++)
            output << ' ' << element_edges[edge_attribute];
        output << '\n';
    }
}

std::map<std::string, std::string>
input_preworker::map_parameters_values(const std::vector<std::string> &token_line, input_preworker::element &element_) {
    std::vector<std::string> element_values;
    for (size_t value_index = 1; value_index < token_line.size(); value_index++) {
        element_values.push_back(token_line[value_index]);
    }
    check_parameters_number(element_, element_values);
    std::map<std::string, std::string> parameter_value_mapping;
    for (size_t value_index = 0; value_index < element_values.size(); value_index++) {
        parameter_value_mapping[element_.parameters[value_index]] = element_values[value_index];
    }
    for (auto &internal_name: element_.internal_variables) {
        parameter_value_mapping[internal_name] = std::to_string(++vertex_number);
        print_message_prefix();
        std::cout << "internal " << internal_name << " was enumerated with " << vertex_number << std::endl;
    }
    return parameter_value_mapping;
}

void input_preworker::check_parameters_number(const input_preworker::element &element_,
                                              const std::vector<std::string> &element_values) const {
    if (element_values.size() != element_.parameters.size()) {
        print_message_prefix();
        std::cout << element_.name << " requires " << element_.parameters.size() << " parameters, but " <<
        element_values.size() << " was given" << std::endl;
        throw InvalidParameterNumberException();
    }
}

void input_preworker::manage_include(const std::vector<std::string> &token_line) {
    std::string file_name = file_path + token_line[1];
    std::ifstream file_;
    file_.open(file_name);
    if (!file_.is_open()) {
        print_message_prefix();
        std::cout << "File " << token_line[1] << " was not opened" << std::endl;
        throw FileOpenException();
    }
    input_preworker file_preworker(file_, file_name, vertex_number);
    file_.close();
    file_preworker.perform_prework();
    output << file_preworker.get_output();
    auto &file_elements = file_preworker.get_elements();
    for (auto& [name, element] : file_elements) {
        if (elements.count(name)) {
            print_message_prefix();
            std::cout << "Multiple definition of element " << name << std::endl;
            throw MultipleDefinitionException();
        }
    }
    elements.insert(file_elements.begin(), file_elements.end());
    vertex_number = std::max(vertex_number, file_preworker.vertex_number);
}

void input_preworker::create_element() {
    auto &token_line = tokens[line_index];
    element element_;
    element_.name = token_line[1];
    for (size_t parameter_index = 2; parameter_index < token_line.size(); parameter_index++) {
        element_.parameters.push_back(token_line[parameter_index]);
        element_.variables.insert(token_line[parameter_index]);
    }
    if (line_index == tokens.size() - 1) {
        line_index++;
        elements[element_.name] = element_;
        return;
    }
    token_line = tokens[++line_index];
    while (token_line[0].empty()) {
        if (token_line.size() == 1) {
            if (line_index == tokens.size() - 1) {
                line_index++;
                elements[element_.name] = element_;
                return;
            }
            token_line = tokens[++line_index];
            continue;
        }
        if (token_line[1] == "internal") {
            element_.internal_variables.push_back(token_line[2]);
            element_.variables.insert(token_line[2]);
        } else {
            check_parameter_defined(token_line, element_);
            element_.edges.push_back(token_line);
        }
        if (line_index == tokens.size() - 1) {
            elements[element_.name] = element_;
            line_index++;
            return;
        }
        token_line = tokens[++line_index];
    }
    elements[element_.name] = element_;
}

void input_preworker::check_parameter_defined(const std::vector<std::string> &token_line,
                                              const input_preworker::element &element_) const {
    if (!element_.variables.count(token_line[1])) {
        print_message_prefix();
        std::cout << "Parameter " << token_line[1] << " in element " << element_.name << "is undefined" << std::endl;
        throw UndefinedVariableException();
    }
    if (!element_.variables.count(token_line[2])) {
        print_message_prefix();
        std::cout << "Parameter " << token_line[2] << " in element " << element_.name << "is undefined" << std::endl;
        throw UndefinedVariableException();
    }
}
