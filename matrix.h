#ifndef CIRCUITS_MATRIX_H
#define CIRCUITS_MATRIX_H

#include <cstdio>
#include <iostream>

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
    matrix(matrix&& another_matrix);

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
matrix<T>::matrix(matrix&& another_matrix):
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

#endif //CIRCUITS_MATRIX_H
