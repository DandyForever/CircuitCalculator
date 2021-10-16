#include <iostream>
#include <sstream>
#include "matrix.h"
#include "circuit.h"

int main() {
    std::string inp;
    std::getline(std::cin, inp);
    circuit cir(inp);
    cir.calculate_edge_current();
    std::cout << cir.get_edge_current_answer() << std::endl;
    return 0;
}
