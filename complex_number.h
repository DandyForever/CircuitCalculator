#ifndef CIRCUITS_COMPLEX_NUMBER_H
#define CIRCUITS_COMPLEX_NUMBER_H

#include <ostream>
#include <math.h>


class complex_number {
public:
    complex_number() = default;
    complex_number(double real, double imaginary);
    complex_number(int real);
    complex_number(double real);

    double get_amplitude() const;
    double get_phase() const;
    double get_real() const { return real_; }

    complex_number conjugate() const;
    complex_number& operator+=(const complex_number& another_number);
    complex_number& operator-=(const complex_number& another_number);
    complex_number& operator*=(const complex_number& another_number);
    complex_number& operator/=(const complex_number& another_number);

    complex_number& operator+=(double another_number);
    complex_number& operator-=(double another_number);
    complex_number& operator*=(double another_number);
    complex_number& operator/=(double another_number);

    bool is_equal(const complex_number& another_number) const;
    bool is_zero() const;

    friend std::ostream& operator<<(std::ostream& output, const complex_number& number);

    class ZeroDivisionException : public std::exception {
        const char* what () const throw () { return "Trying to divide by zero!"; }
    };

    constexpr static const double DELTA = 1e-10;

private:
    double real_ = 0;
    double imaginary_ = 0;
};

complex_number create_exponential(double amplitude, double phase);

complex_number operator+(const complex_number& left_number, const complex_number& right_number);
complex_number operator-(const complex_number& left_number, const complex_number& right_number);
complex_number operator*(const complex_number& left_number, const complex_number& right_number);
complex_number operator/(const complex_number& left_number, const complex_number& right_number);

complex_number operator+(const complex_number& left_number, double right_number);
complex_number operator-(const complex_number& left_number, double right_number);
complex_number operator*(const complex_number& left_number, double right_number);
complex_number operator/(const complex_number& left_number, double right_number);

bool operator==(const complex_number& left_number, const complex_number& right_number);
bool operator!=(const complex_number& left_number, const complex_number& right_number);

#endif //CIRCUITS_COMPLEX_NUMBER_H
