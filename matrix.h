#ifndef CIRCUITS_MATRIX_H
#define CIRCUITS_MATRIX_H

#include <cstdio>
#include <iostream>
#include <vector>
#include <numeric>
#include <tuple>

template <typename T>
class matrix {
public:
    struct row {
        T* data;

        void allocate_data(const size_t col_number) {
            data = new T[col_number];
        }
        void deallocate_data() {
            delete [] data;
        }
        T& operator[](const size_t col_index) {
            return data[col_index];
        }
        const T& operator[](const size_t col_index) const {
            return data[col_index];
        }
    };

    matrix(size_t row_number, size_t col_number);
    matrix(const matrix& another_matrix);
    matrix(matrix&& another_matrix) noexcept;

    size_t get_row_number() const { return row_number_; }
    size_t get_col_number() const { return col_number_; }

    row& operator[](const size_t row_index) { return data[row_index]; }

    const row& operator[](const size_t row_index) const { return data[row_index]; }

    matrix<T>& operator=(const matrix& another_matrix);
    matrix<T>& operator=(matrix&& another_matrix);

    matrix<T>& operator+=(const matrix<T>& another_matrix);
    matrix<T>& operator-=(const matrix<T>& another_matrix);
        bool is_equal_size(const matrix<T>& another_matrix) const;

    matrix<T>& operator*=(const matrix<T>& another_matrix);
        bool is_multiplication_possible(const matrix<T>& another_matrix) const;

    matrix<T> transpose() const;

    matrix<T> inverse() const;
        bool is_square() const;
        matrix<T> get_minor(const size_t row_, const size_t col_) const;
        T get_inverse_element(const size_t row_, const size_t col_, const matrix<T>& minor) const;
        matrix<T> calculate_slae(const matrix<T>& free_members_column) const;
            bool is_column() const;
            bool is_aligned_for_slae(const matrix<T>& free_members_column) const;

    T get_determinant() const;
        std::pair<matrix<T>, matrix<T>> decomposition() const;
    ~matrix();

private:
    row* data;
    size_t row_number_;
    size_t col_number_;

        void free_data();
};

template <typename T>
matrix<T>::matrix(size_t row_number, size_t col_number):
    row_number_(row_number),
    col_number_(col_number)
{
    data = new row[row_number];
    for (size_t row_ = 0; row_ < row_number; row_++) {
        data[row_].allocate_data(col_number);
    }
}

template <typename T>
matrix<T>::matrix(const matrix<T>& another_matrix):
    matrix(another_matrix.row_number_, another_matrix.col_number_)
{
        for (size_t row_ = 0; row_ < row_number_; row_++) {
            for (size_t col_ = 0; col_ < col_number_; col_++) {
                data[row_][col_] = another_matrix[row_][col_];
            }
        }
}

template <typename T>
matrix<T>::matrix(matrix&& another_matrix) noexcept:
    data(another_matrix.data),
    row_number_(another_matrix.row_number_),
    col_number_(another_matrix.col_number_)
{
    another_matrix.data = nullptr;
}

template <typename T>
matrix<T>& matrix<T>::operator=(const matrix& another_matrix){
    if (this == &another_matrix)
        return *this;
    this->~matrix();
    new(this) matrix(another_matrix);
    return *this;
}

template <typename T>
matrix<T>& matrix<T>::operator=(matrix&& another_matrix){
    if (this == &another_matrix)
        return *this;
    free_data();
    row_number_ = std::move(another_matrix.row_number_);
    col_number_ = std::move(another_matrix.col_number_);
    data = another_matrix.data;
    another_matrix.data = nullptr;
    return *this;
}

template <typename T>
matrix<T>& matrix<T>::operator+= (const matrix<T>& another_matrix) {
    if (!is_equal_size(another_matrix))
        throw std::runtime_error("Matrices have different size!");

    for (size_t row_ = 0; row_ < row_number_; row_++) {
        for (size_t col_ = 0; col_ < col_number_; col_++) {
            data[row_][col_] += another_matrix[row_][col_];
        }
    }
    return *this;
}

template <typename T>
matrix<T>& matrix<T>::operator-= (const matrix<T>& another_matrix) {
    if (!is_equal_size(another_matrix))
        throw std::runtime_error("Matrices have different size!");

    for (size_t row_ = 0; row_ < row_number_; row_++) {
        for (size_t col_ = 0; col_ < col_number_; col_++) {
            data[row_][col_] -= another_matrix[row_][col_];
        }
    }
    return *this;
}

template <typename T>
bool matrix<T>::is_equal_size(const matrix<T>& another_matrix) const {
    return row_number_ == another_matrix.row_number_ && col_number_ == another_matrix.col_number_;
}

template <typename T>
matrix<T>& matrix<T>::operator*= (const matrix<T>& another_matrix) {
    if (!is_multiplication_possible(another_matrix))
        throw std::runtime_error("Matrices could not be multiplied!");

    auto result = matrix<T>(row_number_, another_matrix.col_number_);
    for (size_t row_ = 0; row_ < result.row_number_; row_++) {
        for (size_t col_ = 0; col_ < result.col_number_; col_++) {
            result[row_][col_] = static_cast<T>(0);
            for (size_t float_index = 0; float_index < col_number_; float_index++) {
                result[row_][col_] += data[row_][float_index] * another_matrix[float_index][col_];
            }
        }
    }
    *this = std::move(result);
    return *this;
}

template <typename T>
bool matrix<T>::is_multiplication_possible(const matrix<T>& another_matrix) const {
    return col_number_ == another_matrix.row_number_;
}

template <typename T>
matrix<T> matrix<T>::transpose() const {
     matrix<T> result(col_number_, row_number_);

    for (size_t row_ = 0; row_ < row_number_; row_++) {
        for (size_t col_ = 0; col_ < col_number_; col_++) {
            result[col_][row_] = data[row_][col_];
        }
    }

    return result;
}

template <typename T>
matrix<T>::~matrix() {
    if (data)
        free_data();
}

template <typename T>
void matrix<T>::free_data() {
    for (size_t row_ = 0; row_ < row_number_; row_++) {
        data[row_].deallocate_data();
    }
    delete [] data;
}

//template <typename T>
//matrix<T> matrix<T>::inverse() const {
//    if (!is_square())
//        throw std::runtime_error("Invert matrix could not be calculated");
//
//    auto result = matrix<T>(row_number_, col_number_);
//    auto identity_column = matrix<T>(row_number_, 1);
//    for (size_t col_ = 0; col_ < col_number_; col_++) {
//        for (size_t row_ = 0; row_ < row_number_; row_++) {
//            if (row_ == col_)
//                identity_column[row_][0] = static_cast<T>(1);
//            else
//                identity_column[row_][0] = static_cast<T>(0);
//        }
//        auto intermediate_result = calculate_slae(identity_column);
//        for (size_t row_ = 0; row_ < row_number_; row_++) {
//            result[row_][col_] = intermediate_result[row_][0];
//        }
//    }
//    return result;
//}

template <typename T>
matrix<T> matrix<T>::inverse() const {
    if (!is_square())
        throw std::runtime_error("Invert matrix could not be calculated");

    T determinant = get_determinant();
    auto result = matrix<T>(row_number_, col_number_);
    for (size_t row_ = 0; row_ < row_number_; row_++) {
        for (size_t col_ = 0; col_ < col_number_; col_++) {
            auto minor = get_minor(row_, col_);
            T inverse_element = get_inverse_element(row_, col_, minor);
            result[col_][row_] = inverse_element / determinant;
        }
    }
    return result;
}

template <typename T>
bool matrix<T>::is_square() const {
    return col_number_ == row_number_;
}

template <typename T>
matrix<T> matrix<T>::get_minor(const size_t row_, const size_t col_) const {
    auto result = matrix<T>(row_number_ - 1, col_number_ - 1);
    for (size_t minor_row = 0; minor_row < row_number_ - 1; minor_row++) {
        for (size_t minor_col = 0; minor_col < col_number_ - 1; minor_col++) {
            size_t data_row = minor_row, data_col = minor_col;
            if (data_row >= row_)
                data_row++;
            if (data_col >= col_)
                data_col++;
            result[minor_row][minor_col] = data[data_row][data_col];
        }
    }
    return result;
}

template <typename T>
T matrix<T>::get_inverse_element(const size_t row_, const size_t col_, const matrix<T>& minor) const {
    T element = minor.get_determinant();
    if ((row_ + col_) % 2 == 1)
        element *= -1;
    return element;
}

template <typename T>
matrix<T> matrix<T>::calculate_slae(const matrix<T>& free_members_column) const {
    if (!free_members_column.is_column() || !is_aligned_for_slae(free_members_column))
        throw std::runtime_error("Cannot Solve System of linear algebraic equations!");

    auto solution = matrix<T>(row_number_, 1);
    auto iteration = matrix<T>(row_number_, 1);
    for (size_t row_ = 0; row_ < row_number_; row_++) {
        solution[row_][0] = free_members_column[row_][0] / data[row_][row_];
    }

    T eps = static_cast<T>(0.01);
    do {
        for (size_t row_ = 0; row_ < row_number_; row_++) {
            iteration[row_][0] = free_members_column[row_][0] / data[row_][row_];
            for (size_t col_ = 0; col_ < col_number_; col_++) {
                if (row_ != col_) {
                    iteration[row_][0] -= data[row_][col_] / data[row_][row_] * solution[col_][0];
                }
            }
        }

        bool is_accurate = true;

        for (size_t row_ = 0; row_ < row_number_; row_++) {
            if (std::abs(iteration[row_][0] - solution[row_][0]) > eps) {
                is_accurate = false;
                break;
            }
        }

        solution = iteration;
        std::cout << std::endl;
        if (is_accurate)
            break;
    } while (true);

    return solution;
}

template <typename T>
bool matrix<T>::is_column() const {
    return col_number_ == 1;
}

template <typename T>
bool matrix<T>::is_aligned_for_slae(const matrix<T>& free_members_column) const {
    return row_number_ == free_members_column.row_number_;
}

template <typename T>
T matrix<T>::get_determinant() const {
    if (row_number_ != col_number_)
        throw std::runtime_error("Cannot calculate matrix determinant");

    auto [lower, upper] = decomposition();
    T lower_determinant = static_cast<T>(1);
    T upper_determinant = static_cast<T>(1);
    for (size_t row_ = 0; row_ < row_number_; row_++) {
        lower_determinant *= lower[row_][row_];
        upper_determinant *= upper[row_][row_];
    }
    return lower_determinant * upper_determinant;
}

template <typename T>
std::pair<matrix<T>, matrix<T>> matrix<T>::decomposition() const {
    if (!is_square())
        throw std::runtime_error("Cannot make matrix decomposition");

    auto lower = matrix<T>(row_number_, col_number_);
    auto upper = matrix<T>(row_number_, col_number_);
    for (size_t row_ = 0; row_ < row_number_; row_++) {
        for (size_t col_ = 0; col_ < col_number_; col_++) {
            if (row_ == col_) {
                lower[row_][row_] = static_cast<T>(1);
            } else {
                lower[row_][col_] = static_cast<T>(0);
                upper[row_][col_] = static_cast<T>(0);
            }
        }
    }
    for (size_t row_ = 0; row_ < row_number_; row_++) {
        for (size_t col_ = 0; col_ < col_number_; col_++) {
            if (row_ <= col_) {
                upper[row_][col_] = data[row_][col_];
                for (size_t variate_row = 0; variate_row < row_; variate_row++) {
                    upper[row_][col_] -= lower[row_][variate_row] * upper[variate_row][col_];
                }
            } else {
                lower[row_][col_] = data[row_][col_] / upper[col_][col_];
                for (size_t variate_col = 0; variate_col < col_; variate_col++) {
                    lower[row_][col_] -= lower[row_][variate_col] * upper[variate_col][col_] / upper[col_][col_];
                }
            }
        }
    }
    return std::make_pair(lower, upper);
}

#endif //CIRCUITS_MATRIX_H
