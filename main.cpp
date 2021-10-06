#include <iostream>
#include "matrix.h"

int main() {
    size_t row_number = 3, col_number = 3;
    auto m = matrix<int>(row_number, col_number);
    for (size_t row_ = 0; row_ < row_number; row_++) {
        for (size_t col_ = 0; col_ < col_number; col_++) {
            m[row_][col_] = row_ + row_ + col_;
        }
    }
    auto copy = matrix<int>(m);
    copy *= m;
    copy = copy.transpose();
    for (size_t row_ = 0; row_ < row_number; row_++) {
        for (size_t col_ = 0; col_ < col_number; col_++) {
            std::cout << copy[row_][col_] << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
