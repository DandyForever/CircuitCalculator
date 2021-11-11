#include "complex_number.h"

bool is_equal_(double left_number, double right_number) {
    return abs(left_number - right_number) < complex_number::DELTA;
}

complex_number::complex_number(double real, double imaginary):
    real_(real),
    imaginary_(imaginary)
{}

double complex_number::get_amplitude() const {
    return real_ * real_ + imaginary_ * imaginary_;
}

bool complex_number::is_equal(const complex_number &another_number) const {
    return is_equal_(real_, another_number.real_) && is_equal_(imaginary_, another_number.imaginary_);
}

bool complex_number::is_zero() const {
    return is_equal({0, 0});
}

complex_number complex_number::conjugate() const {
    return {real_, -imaginary_};
}

complex_number &complex_number::operator+=(const complex_number &another_number) {
    real_ += another_number.real_;
    imaginary_ += another_number.imaginary_;
    return *this;
}

complex_number &complex_number::operator+=(double another_number) {
    return operator+=({another_number, 0});
}

complex_number &complex_number::operator-=(const complex_number &another_number) {
    real_ -= another_number.real_;
    imaginary_ -= another_number.imaginary_;
    return *this;
}

complex_number &complex_number::operator-=(double another_number) {
    return operator-=({another_number, 0});
}

complex_number &complex_number::operator*=(const complex_number &another_number) {
    complex_number tmp(*this);
    real_ = tmp.real_ * another_number.real_ - tmp.imaginary_ * another_number.imaginary_;
    imaginary_ = tmp.real_ * another_number.imaginary_ + tmp.imaginary_ * another_number.real_;
    return *this;
}

complex_number &complex_number::operator*=(double another_number) {
    return operator*=({another_number, 0});
}

complex_number &complex_number::operator/=(double another_number) {
    if (is_equal_(another_number, 0))
        throw ZeroDivisionException();

    real_ /= another_number;
    imaginary_ /= another_number;
    return *this;
}

complex_number &complex_number::operator/=(const complex_number &another_number) {
    if (another_number.is_zero())
        throw ZeroDivisionException();

    *this *= another_number.conjugate();
    *this /= another_number.get_amplitude();
    return *this;
}

complex_number create_exponential(double amplitude, double phase) {
    return {amplitude * cos(phase), amplitude * sin(phase)};
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
    output << number.real_;
    if (number.imaginary_ < 0)
        output << '-';
    else
        output << "+";
    output << 'i' << abs(number.imaginary_);
    return output;
}