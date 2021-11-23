#ifndef CIRCUITS_MATRIX_H
#define CIRCUITS_MATRIX_H

#include <cstdio>
#include <iostream>
#include <vector>
#include <numeric>
#include <tuple>
#include <complex>

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
    matrix() = default;
    matrix(size_t row_number, size_t col_number);
    matrix(const matrix& another_matrix);
    matrix(matrix&& another_matrix) noexcept;

    size_t get_row_number() const { return row_number_; }
    size_t get_col_number() const { return col_number_; }

    row& operator[](const size_t row_index) { return data[row_index]; }

    const row& operator[](const size_t row_index) const { return data[row_index]; }

    matrix<T>& operator=(const matrix& another_matrix);
    matrix<T>& operator=(matrix&& another_matrix) noexcept;

    matrix<T>& operator+=(const matrix<T>& another_matrix);
    matrix<T>& operator-=(const matrix<T>& another_matrix);
        bool is_equal_size(const matrix<T>& another_matrix) const;

    matrix<T>& operator*=(const matrix<T>& another_matrix);
        bool is_multiplication_possible(const matrix<T>& another_matrix) const;

    matrix<T> transpose() const;

    matrix<T> inverse() const;
        bool is_square() const;
        matrix<T> get_minor(const size_t row_, const size_t col_) const;

    T get_determinant() const;
        std::pair<matrix<T>, matrix<T>> get_decomposition() const;

    void print() const;
    bool is_equal(const matrix<T>& another_matrix) const;
    ~matrix();

    class ZeroDivisionException : public std::exception {
        const char* what () const throw () { return "Trying to divide by zero!"; }
    };

private:
    row* data;
    size_t row_number_ = 0;
    size_t col_number_ = 0;

        void free_data();

        T get_inverse_element(const size_t row_, const size_t col_, const matrix<T>& minor) const;

        void initialize_lower() const;
        void fill_decomposition(matrix<T> &lower, matrix<T> &upper) const;
            void fill_upper_iteration(const matrix<T> &lower, matrix<T> &upper, size_t row_, size_t col_) const;
            void fill_lower_iteration(matrix<T> &lower, const matrix<T> &upper, size_t row_, size_t col_) const;

    bool is_equal(T matrix_element, T another_matrix_element) const;
};

template <typename T>
matrix<T>::matrix(size_t row_number, size_t col_number):
    row_number_(row_number),
    col_number_(col_number)
{
    try {
        data = new row[row_number];
    } catch (std::bad_alloc&) {
        std::cout << "Matrix constructor failed: not enough memory" << std::endl;
        throw;
    }
    for (size_t row_ = 0; row_ < row_number; row_++) {
        try {
            data[row_].allocate_data(col_number);
        } catch (std::bad_alloc&) {
            free_data();
            std::cout << "Matrix constructor failed: not enough memory" << std::endl;
            throw;
        }
        for (size_t col_ = 0; col_ < col_number_; col_++) {
            data[row_][col_] = static_cast<T>(0);
        }
    }
}

template <typename T>
matrix<T>::matrix(const matrix<T>& another_matrix):
    row_number_(another_matrix.row_number_),
    col_number_(another_matrix.col_number_)
{
    try {
        data = new row[another_matrix.row_number_];
    } catch (std::bad_alloc&) {
        std::cout << "Copy constructor failed: not enough memory" << std::endl;
        throw;
    }
    for (size_t row_ = 0; row_ < row_number_; row_++) {
        try {
            data[row_].allocate_data(col_number_);
        } catch (std::bad_alloc&) {
            free_data();
            std::cout << "Copy constructor failed: not enough memory" << std::endl;
            throw;
        }
        std::copy(another_matrix[row_].data, another_matrix[row_].data + col_number_, data[row_].data);
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
matrix<T>& matrix<T>::operator=(const matrix& another_matrix) {
    if (this == &another_matrix)
        return *this;
    this->~matrix();
    try {
        new(this) matrix(another_matrix);
    } catch (std::bad_alloc&) {
        std::cout << "Copy assignment failed: not enough memory" << std::endl;
        throw;
    }
    return *this;
}

template <typename T>
matrix<T>& matrix<T>::operator=(matrix&& another_matrix) noexcept {
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
matrix<T> operator-(const matrix<T>& left_matrix, const matrix<T>& right_matrix) {
    matrix<T> result{left_matrix};
    result -= right_matrix;
    return result;
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
matrix<T> operator*(const matrix<T>& left_matrix, const matrix<T>& right_matrix) {
    matrix<T> result{left_matrix};
    result *= right_matrix;
    return result;
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

template <typename T>
matrix<T> matrix<T>::inverse() const {
    if (!is_square())
        throw std::runtime_error("Invert matrix could not be calculated");

    const T determinant = get_determinant();
    auto result = matrix<T>(row_number_, col_number_);
    for (size_t row_ = 0; row_ < row_number_; row_++) {
        for (size_t col_ = 0; col_ < col_number_; col_++) {
            auto minor = get_minor(row_, col_);
            T inverse_element = get_inverse_element(row_, col_, minor);
            if (std::abs(determinant) < 1e-18) {
                std::cout << determinant << std::endl;
                throw ZeroDivisionException();
            }
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
T matrix<T>::get_determinant() const {
    if (row_number_ != col_number_)
        throw std::runtime_error("Cannot calculate matrix determinant");

    auto [lower, upper] = get_decomposition();
    T lower_determinant = static_cast<T>(1);
    T upper_determinant = static_cast<T>(1);
    for (size_t row_ = 0; row_ < row_number_; row_++) {
        lower_determinant *= lower[row_][row_];
        upper_determinant *= upper[row_][row_];
    }
    return lower_determinant * upper_determinant;
}

template <typename T>
std::pair<matrix<T>, matrix<T>> matrix<T>::get_decomposition() const {
    if (!is_square())
        throw std::runtime_error("Cannot make matrix decomposition");

    auto lower = matrix<T>(row_number_, col_number_);
    lower.initialize_lower();
    auto upper = matrix<T>(row_number_, col_number_);
    fill_decomposition(lower, upper);
    return std::make_pair(lower, upper);
}

template<typename T>
void matrix<T>::fill_decomposition(matrix<T>& lower, matrix<T>& upper) const {
    for (size_t row_ = 0; row_ < row_number_; row_++) {
        for (size_t col_ = 0; col_ < col_number_; col_++) {
            if (row_ <= col_) {
                fill_upper_iteration(lower, upper, row_, col_);
            } else {
                fill_lower_iteration(lower, upper, row_, col_);
            }
        }
    }
}

template<typename T>
void matrix<T>::fill_lower_iteration(matrix<T> &lower, const matrix<T> &upper, size_t row_, size_t col_) const {
    const T divider = upper[col_][col_];
    if (std::abs(divider) < 1e-18)
        throw ZeroDivisionException();
    lower[row_][col_] = data[row_][col_] / divider;
    for (size_t variate_col = 0; variate_col < col_; variate_col++) {
        lower[row_][col_] -= lower[row_][variate_col] * upper[variate_col][col_] / divider;
    }
}

template<typename T>
void matrix<T>::fill_upper_iteration(const matrix<T> &lower, matrix<T> &upper, size_t row_, size_t col_) const {
    upper[row_][col_] = data[row_][col_];
    for (size_t variate_row = 0; variate_row < row_; variate_row++) {
        upper[row_][col_] -= lower[row_][variate_row] * upper[variate_row][col_];
    }
}

template<typename T>
void matrix<T>::initialize_lower() const {
    for (size_t row_ = 0; row_ < row_number_; row_++) {
        for (size_t col_ = 0; col_ < col_number_; col_++) {
            if (row_ == col_) {
                data[row_][row_] = static_cast<T>(1);
            }
        }
    }
}

template <typename T>
void matrix<T>::print() const {
    std::cout << row_number_ << " " << col_number_ << std::endl;
    for (size_t row_ = 0; row_ < row_number_; row_++) {
        for (size_t col_ = 0; col_ < col_number_; col_++) {
            std::cout << data[row_][col_] << " ";
        }
        std::cout << std::endl;
    }
}

template <typename T>
bool matrix<T>::is_equal(const matrix<T>& another_matrix) const {
    if (!is_equal_size(another_matrix))
        return false;

    for (size_t row_ = 0; row_ < row_number_; row_++) {
        for (size_t col_ = 0; col_ < col_number_; col_++) {
            if (!is_equal(data[row_][col_], another_matrix[row_][col_]))
                return false;
        }
    }
    return true;
}

template <typename T>
bool matrix<T>::is_equal(T matrix_element, T another_matrix_element) const {
    return matrix_element == another_matrix_element;
}
template <>
bool matrix<double>::is_equal(double matrix_element, double another_matrix_element) const;
template <>
bool matrix<float>::is_equal(float matrix_element, float another_matrix_element) const;
template <>
bool matrix<std::complex<double>>::is_equal(std::complex<double> matrix_element,
                                            std::complex<double> another_matrix_element) const;

#endif //CIRCUITS_MATRIX_H
