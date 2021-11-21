#include <iostream>
#include "complex_number.h"

bool is_equal_(double left_number, double right_number) {
    return abs(left_number - right_number) < complex_number::DELTA;
}

complex_number::complex_number(double real, double imaginary):
    number(real, imaginary)
{}

complex_number::complex_number(int real):
    number(real)
{}

complex_number::complex_number(double real):
    number(real)
{}

complex_number::complex_number(const std::complex<double>& number_):
    number(number_)
{}

double complex_number::get_amplitude() const {
    return std::abs(number);
}

double complex_number::get_phase() const {
    return std::arg(number);
}

bool complex_number::is_equal(const complex_number &another_number) const {
    return is_equal_(number.real(), another_number.number.real()) &&
            is_equal_(number.imag(), another_number.number.imag());
}

bool complex_number::is_zero() const {
    return is_equal({0., 0.});
}

complex_number complex_number::conjugate() const {
    return std::conj(number);
}

complex_number &complex_number::operator+=(const complex_number &another_number) {
    number += another_number.number;
    return *this;
}

complex_number &complex_number::operator+=(double another_number) {
    number += another_number;
    return *this;
}

complex_number &complex_number::operator-=(const complex_number &another_number) {
    number -= another_number.number;
    return *this;
}

complex_number &complex_number::operator-=(double another_number) {
    number -= another_number;
    return *this;
}

complex_number &complex_number::operator*=(const complex_number &another_number) {
    number *= another_number.number;
    return *this;
}

complex_number &complex_number::operator*=(double another_number) {
    number *= another_number;
    return *this;
}

complex_number &complex_number::operator/=(double another_number) {
    if (is_equal_(another_number, 0.))
        throw ZeroDivisionException();

    number /= another_number;
    return *this;
}

complex_number &complex_number::operator/=(const complex_number &another_number) {
    if (another_number.is_zero())
        throw ZeroDivisionException();

    number /= another_number.number;
    return *this;
}

complex_number create_exponential(double amplitude, double phase) {
    return std::polar(amplitude, phase);
}

complex_number operator+(const complex_number& left_number, const complex_number& right_number) {
    complex_number tmp{left_number};
    tmp += right_number;
    return tmp;
}

complex_number operator-(const complex_number& left_number, const complex_number& right_number) {
    complex_number tmp{left_number};
    tmp -= right_number;
    return tmp;
}

complex_number operator*(const complex_number& left_number, const complex_number& right_number) {
    complex_number tmp{left_number};
    tmp *= right_number;
    return tmp;
}

complex_number operator/(const complex_number& left_number, const complex_number& right_number) {
    complex_number tmp{left_number};
    tmp /= right_number;
    return tmp;
}

complex_number operator+(const complex_number& left_number, double right_number) {
    complex_number tmp{left_number};
    tmp += right_number;
    return tmp;
}

complex_number operator-(const complex_number& left_number, double right_number) {
    complex_number tmp{left_number};
    tmp -= right_number;
    return tmp;
}

complex_number operator*(const complex_number& left_number, double right_number) {
    complex_number tmp{left_number};
    tmp *= right_number;
    return tmp;
}

complex_number operator/(const complex_number& left_number, double right_number) {
    complex_number tmp{left_number};
    tmp /= right_number;
    return tmp;
}

bool operator==(const complex_number& left_number, const complex_number& right_number) {
    return left_number.is_equal(right_number);
}

bool operator!=(const complex_number& left_number, const complex_number& right_number) {
    return !left_number.is_equal(right_number);
}

std::ostream& operator<<(std::ostream& output, const complex_number& number) {
    output << number.number;
    return output;
}