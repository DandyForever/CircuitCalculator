#include <fstream>
#include "input_preworker.h"



std::vector<std::string> input_preworker::tokenize(const std::string& line) {
    std::vector<std::string> tokens_;
    std::stringstream line_(line);
    if (std::isspace(line_.peek()) || line_.eof())
        tokens_.emplace_back("");
    line_ >> std::ws;
    while (!line_.eof()) {
        std::string token;
        if (line_.peek() == '-') {
            token.push_back(line_.get());
            if (line_.peek() == '-')
                token.push_back(line_.get());
            else while (std::isdigit(line_.peek()) || line_.peek() == '.')
                token.push_back(line_.get());
        } else {
            while (std::isdigit(line_.peek()) || line_.peek() == '.') {
                token.push_back(line_.get());
            }
            if (token.empty()) {
                while (std::isalpha(line_.peek()) || std::isdigit(line_.peek()) || line_.peek() == '.')
                    token.push_back(line_.get());
                if (token == "include") {
                    line_ >> std::ws;
                    tokens_.push_back(token);
                    token = "";
                    line_ >> token;
                    if (token.empty()) {
                        std::cout << "File path required with include" << std::endl;
                        throw UnexpectedTokenException();
                    }
                }
            }
            if (token.empty()) {
                token.push_back(line_.get());
                if (!Punctuation.count(token)) {
                    std::cout << "Unexpected token: " << token << std::endl;
                    throw UnexpectedTokenException();
                }
            }
        }
        line_ >> std::ws;
        tokens_.push_back(token);
    }
    return tokens_;
}

input_preworker::input_preworker(std::istream& input, const std::string& file, int max_vertex) {
    file_name_ = file;
    vertex_number = max_vertex;
    if (file_name_ != "input") {
        auto position = file_name_.find_last_of('/');
        if (position != std::string::npos) {
            file_path = file_name_.substr(0, position + 1);
            std::cout << file_path << " " << file_name_ << std::endl;
        }
    }
    std::string line;
    while (std::getline(input, line)) {
        tokens.emplace_back(tokenize(line));
        check_max_vertex();
    }
}

int input_preworker::check_vertex_index(std::string& token) const {
    for (char c : token) {
        if (!std::isdigit(c))
            return 0;
    }
    return std::stoi(token);
}

void input_preworker::check_max_vertex() {
    if (tokens.back()[0] != "define" && tokens.back()[0] != "include" && std::isalpha(tokens.back()[0][0])) {
        for (size_t token_index = 2; token_index < tokens.back().size() - 1; token_index += 2)
            vertex_number = std::max(vertex_number, check_vertex_index(tokens.back()[token_index]));
    }
    if (tokens.back().size() > 2 && tokens.back()[1] == "--") {
        vertex_number = std::max(vertex_number, check_vertex_index(tokens.back()[0]));
        vertex_number = std::max(vertex_number, check_vertex_index(tokens.back()[2]));
    }
    if (tokens.back().size() > 3 && tokens.back()[2] == "--") {
        vertex_number = std::max(vertex_number, check_vertex_index(tokens.back()[1]));
        vertex_number = std::max(vertex_number, check_vertex_index(tokens.back()[3]));
    }
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
    check_edge_token_line(token_line);
    for (auto& token : token_line)
        output << token;
    output << '\n';
}

void input_preworker::substitute_element(const std::vector<std::string> &token_line) {
    std::string element_name = token_line[0];
    check_element_token_line(token_line);
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
        output << parameter_value_mapping.at(element_edges[1]) << element_edges[2] <<
               parameter_value_mapping.at(element_edges[3]);
        for (size_t edge_attribute = 4; edge_attribute < element_edges.size(); edge_attribute++)
            output << element_edges[edge_attribute];
        output << '\n';
    }
}

std::map<std::string, std::string>
input_preworker::map_parameters_values(const std::vector<std::string> &token_line, input_preworker::element &element_) {
    std::vector<std::string> element_values;
    for (size_t value_index = 2; value_index < token_line.size() - 1; value_index += 2) {
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
        std::cout << "internal " << internal_name << " was enumerate with " << vertex_number << std::endl;
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
    check_include_token_line(token_line);
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
    check_define_token_line(token_line);
    element element_;
    element_.name = token_line[1];
    for (size_t parameter_index = 3; parameter_index < token_line.size() - 1; parameter_index += 2) {
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
            check_internal_token_line(token_line);
            element_.internal_variables.push_back(token_line[2]);
            element_.variables.insert(token_line[2]);
        } else {
            check_define_edge_token_line(token_line);
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
    if (!element_.variables.count(token_line[3])) {
        print_message_prefix();
        std::cout << "Parameter " << token_line[3] << " in element " << element_.name << "is undefined" << std::endl;
        throw UndefinedVariableException();
    }
}

void input_preworker::check_define_token_line(const std::vector<std::string> &token_line) const {
    check_tokens_equal(token_line[0], "define");
    if (token_line.size() < 4) {
        print_message_prefix();
        std::cout << "Incomplete define line" << std::endl;
        throw UnexpectedTokenException();
    }
    check_name(token_line[1]);
    if (elements.count(token_line[1])) {
        print_message_prefix();
        std::cout << "Multiple definition of element " << token_line[1] << std::endl;
        throw MultipleDefinitionException();
    }
    check_tokens_equal(token_line[2], "(");
    for (size_t token_index = 3; token_index < token_line.size() - 1; token_index += 2)
        check_name(token_line[token_index]);
    for (size_t token_index = 4; token_index < token_line.size() - 1; token_index += 2)
        check_tokens_equal(token_line[token_index], ",");
    if (token_line[token_line.size() - 2] == ",") {
        print_message_prefix();
        std::cout << "Unexpected token \',\' in element " << token_line[1] << " definition" << std::endl;
        throw UnexpectedTokenException();
    }
    check_tokens_equal(token_line[token_line.size() - 1], ")");
}

void input_preworker::check_element_token_line(const std::vector<std::string> &token_line) const {
    if (token_line.size() < 3) {
        print_message_prefix();
        std::cout << "Incomplete define line" << std::endl;
        throw UnexpectedTokenException();
    }
    check_name(token_line[0]);
    check_tokens_equal(token_line[1], "(");
    for (size_t token_index = 2; token_index < token_line.size() - 1; token_index += 2)
        check_integer(token_line[token_index]);
    for (size_t token_index = 3; token_index < token_line.size() - 1; token_index += 2)
        check_tokens_equal(token_line[token_index], ",");
    if (token_line[token_line.size() - 2] == ",") {
        print_message_prefix();
        std::cout << "Unexpected token \',\' in element " << token_line[0] << " substitution" << std::endl;
        throw UnexpectedTokenException();
    }
    check_tokens_equal(token_line[token_line.size() - 1], ")");
}

void input_preworker::check_include_token_line(const std::vector<std::string> &token_line) const {
    check_tokens_equal(token_line[0], "include");
    if (token_line.size() != 2) {
        print_message_prefix();
        std::cout << "Include line should contain file name only" << std::endl;
        throw UnexpectedTokenException();
    }
}

void input_preworker::check_second_token_is_name(const std::vector<std::string> &token_line, size_t line_size) const {
    if (token_line.size() < line_size) {
        print_message_prefix();
        std::cout << "Line should contain variable name" << std::endl;
        throw UnexpectedTokenException();
    }
    check_name(token_line[1]);
    if (token_line.size() > line_size) {
        print_message_prefix();
        std::cout << "Line should contain only variable name" << std::endl;
        throw UnexpectedTokenException();
    }
}

void input_preworker::check_internal_token_line(const std::vector<std::string> &token_line) const {
    check_tokens_equal(token_line[0], "");
    check_tokens_equal(token_line[1], "internal");
    check_second_token_is_name(token_line, 3);
}

void input_preworker::check_edge_token_line(const std::vector<std::string> &token_line) const {
    if (token_line.size() < 12) {
        print_message_prefix();
        std::cout << "Incomplete edge token line" << std::endl;
        throw UnexpectedTokenException();
    }
    check_integer(token_line[0]);
    check_tokens_equal(token_line[1], "--");
    check_integer(token_line[2]);
    check_edge_attributes(token_line, 18);
}

void input_preworker::check_define_edge_token_line(const std::vector<std::string> &token_line) const {
    if (token_line.size() < 13) {
        print_message_prefix();
        std::cout << "Incomplete edge token line" << std::endl;
        throw UnexpectedTokenException();
    }
    check_tokens_equal(token_line[0], "");
    check_name(token_line[1]);
    check_tokens_equal(token_line[2], "--");
    check_name(token_line[3]);
    check_edge_attributes(token_line, 19);
}

void input_preworker::check_edge_attributes(const std::vector<std::string> &token_line, size_t max_tokens) const {
    check_tokens_equal(token_line[max_tokens - 15], ",");
    check_double(token_line[max_tokens - 14]);
    check_tokens_equal(token_line[max_tokens - 13], "R");
    check_tokens_equal(token_line[max_tokens - 12], ";");
    check_double(token_line[max_tokens - 11]);
    check_tokens_equal(token_line[max_tokens - 10], "C");
    check_tokens_equal(token_line[max_tokens - 9], ";");
    check_double(token_line[max_tokens - 8]);
    check_tokens_equal(token_line[max_tokens - 7], "L");
    if (token_line.size() > max_tokens - 6) {
        check_tokens_equal(token_line[max_tokens - 6], ";");
        if (token_line.size() > max_tokens - 5) {
            if (token_line.size() < max_tokens - 1) {
                print_message_prefix();
                std::cout << "Incomplete edge token line" << std::endl;
                throw UnexpectedTokenException();
            }
            check_double(token_line[max_tokens - 5]);
            check_tokens_equal(token_line[max_tokens - 4], "V");
            check_tokens_equal(token_line[max_tokens - 3], ",");
            check_double(token_line[max_tokens - 2]);
            if (token_line.size() > max_tokens - 1) {
                check_tokens_equal(token_line[max_tokens - 1], ";");
                if (token_line.size() > max_tokens) {
                    print_message_prefix();
                    std::cout << "Edge token line include unexpected tokens" << std::endl;
                    throw UnexpectedTokenException();
                }
            }
        }
    }
}

void input_preworker::check_name(const std::string& token) const {
    if (!std::isalpha(token[0])) {
        print_message_prefix();
        std::cout << "Invalid name " << token << ": should start with a letter" << std::endl;
        throw UnexpectedTokenException();
    }
    for (char symbol : token) {
        if (!std::isalpha(symbol) && !std::isdigit(symbol) && symbol != '.') {
            print_message_prefix();
            std::cout << "Invalid name " << token << ": unacceptable character " << symbol << std::endl;
            throw UnexpectedTokenException();
        }
    }
}

void input_preworker::check_integer(const std::string& token) const {
    for (char symbol : token) {
        if (!std::isdigit(symbol)) {
            print_message_prefix();
            std::cout << "Invalid integer " << token << ": unacceptable character " << symbol << std::endl;
            throw UnexpectedTokenException();
        }
    }
}

void input_preworker::check_double(const std::string& token) const {
    size_t dot_count = 0;
    for (char symbol : token) {
        if (!std::isdigit(symbol) && symbol != '.' && symbol != '-') {
            print_message_prefix();
            std::cout << "Invalid double " << token << ": unacceptable character " << symbol << std::endl;
            throw UnexpectedTokenException();
        }
        if (symbol == '.')
            dot_count++;
    }
    if (dot_count > 1) {
        print_message_prefix();
        std::cout << "Invalid double " << token << ": more than one dot found" << std::endl;
        throw UnexpectedTokenException();
    }
}

void input_preworker::check_tokens_equal(const std::string &token, const std::string &expected_token) const {
    if (token != expected_token) {
        print_message_prefix();
        std::cout << "Unexpected token " << token << ": " << expected_token << " was expected" << std::endl;
        throw UnexpectedTokenException();
    }
}