#include <iostream>
#include <sstream>
#include "matrix.h"
#include "circuit.h"

int main() {
    std::string inp;
    std::string input_string;
    while (std::getline(std::cin, inp)) {
        inp.push_back(' ');
        input_string.insert(input_string.length(), inp);
    }
    circuit cir(input_string);
    cir.calculate_edge_current();
    std::cout << cir.get_edge_current_answer() << std::endl;
    return 0;
}
