#include <iostream>
#include <sstream>
#include "matrix.h"
#include "circuit.h"

int main() {
    size_t row_number = 3, col_number = 3;
    auto m = matrix<double>(row_number, col_number);
    m[0][0] = 1.;
    m[0][1] = -2.;
    m[0][2] = 2.;
    m[1][0] = 2.;
    m[1][1] = 1.;
    m[1][2] = -1.;
    m[2][0] = 4.;
    m[2][1] = -3.;
    m[2][2] = 5.;
    auto inverted = m.inverse();
    m *= inverted;
    for (size_t row_ = 0; row_ < row_number; row_++) {
        for (size_t col_ = 0; col_ < col_number; col_++) {
            std::cout << m[row_][col_] << " ";
        }
        std::cout << std::endl;
    }
    std::string inp;
    std::getline(std::cin, inp);
//    std::stringstream inp_ss(inp);

    circuit cir(inp);
    cir.calculate();
    return 0;
}
