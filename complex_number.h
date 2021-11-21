#ifndef CIRCUITS_COMPLEX_NUMBER_H
#define CIRCUITS_COMPLEX_NUMBER_H

#include <complex>

bool operator==(const std::complex<double>& lhs, const std::complex<double>& rhs) {
    return std::abs(lhs.real() - rhs.real()) < 1e-10 &&
            std::abs(lhs.imag() - rhs.imag()) < 1e-10;
}

#endif //CIRCUITS_COMPLEX_NUMBER_H
