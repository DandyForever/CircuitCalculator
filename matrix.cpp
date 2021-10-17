#include "matrix.h"

template <>
bool matrix<double>::is_equal(double matrix_element, double another_matrix_element) const {
    return std::abs(matrix_element - another_matrix_element) < 1e-6;
}

template <>
bool matrix<float>::is_equal(float matrix_element, float another_matrix_element) const {
    return std::abs(matrix_element - another_matrix_element) < 1e-6;
}