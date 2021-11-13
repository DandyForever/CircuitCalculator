#include <gtest/gtest.h>

#include "../complex_number.h"

TEST(ConstructTest, DefaultConstructorTest) {
    complex_number test_number;
    EXPECT_TRUE(test_number.is_zero());
}

TEST(ConstructTest, SimpleConstructorTest) {
    complex_number test_number(1, 1);
    EXPECT_NE(test_number, complex_number(0, 0));
    EXPECT_EQ(test_number, complex_number(1, 1));
}

TEST(ConstructTest, ExponentialConstructorZeroPhaseTest) {
    complex_number test_number = create_exponential(2, 0);
    EXPECT_EQ(test_number, complex_number(2, 0));
}

TEST(ConstructTest, ExponentialConstructorHalfPIPhaseTest) {
    complex_number test_number = create_exponential(2, M_PI / 2);
    EXPECT_EQ(test_number, complex_number(0, 2));
}

TEST(ConstructTest, ExponentialConstructorPIPhaseTest) {
    complex_number test_number = create_exponential(2, M_PI);
    EXPECT_EQ(test_number, complex_number(-2, 0));
}

TEST(ConstructTest, ExponentialConstructorThreePIHalfPhaseTest) {
    complex_number test_number = create_exponential(2, 3 * M_PI / 2);
    EXPECT_EQ(test_number, complex_number(0, -2));
}

TEST(ConstructTest, ExponentialConstructorTwoPIPhaseTest) {
    complex_number test_number = create_exponential(2, 2 * M_PI);
    EXPECT_EQ(test_number, complex_number(2, 0));
}

TEST(ArithmeticTest, PlusEqualTest) {
    complex_number test_number(1, 2);
    complex_number add_number(3, 4);
    test_number += add_number;
    EXPECT_EQ(test_number, complex_number(4, 6));
}

TEST(ArithmeticTest, PlusEqualDoubleTest) {
    complex_number test_number(1, 2);
    test_number += 5;
    EXPECT_EQ(test_number, complex_number(6, 2));
}

TEST(ArithmeticTest, PlusTest) {
    complex_number test_number(1, 2);
    complex_number add_number(3, 4);
    test_number = test_number + add_number;
    EXPECT_EQ(test_number, complex_number(4, 6));
}

TEST(ArithmeticTest, PlusDoubleTest) {
    complex_number test_number(1, 2);
    test_number = test_number + 5;
    EXPECT_EQ(test_number, complex_number(6, 2));
}

TEST(ArithmeticTest, MinusEqualTest) {
    complex_number test_number(1, 2);
    complex_number add_number(3, 4);
    test_number -= add_number;
    EXPECT_EQ(test_number, complex_number(-2, -2));
}

TEST(ArithmeticTest, MinusEqualDoubleTest) {
    complex_number test_number(1, 2);
    test_number -= 5;
    EXPECT_EQ(test_number, complex_number(-4, 2));
}

TEST(ArithmeticTest, MinusTest) {
    complex_number test_number(1, 2);
    complex_number add_number(3, 4);
    test_number = test_number - add_number;
    EXPECT_EQ(test_number, complex_number(-2, -2));
}

TEST(ArithmeticTest, MinusDoubleTest) {
    complex_number test_number(1, 2);
    test_number = test_number - 5;
    EXPECT_EQ(test_number, complex_number(-4, 2));
}

TEST(ArithmeticTest, MultiplyEqualTest) {
    complex_number test_number(1, 2);
    complex_number add_number(3, 4);
    test_number *= add_number;
    EXPECT_EQ(test_number, complex_number(-5, 10));
}

TEST(ArithmeticTest, MultiplyEqualDoubleTest) {
    complex_number test_number(1, 2);
    test_number *= 5;
    EXPECT_EQ(test_number, complex_number(5, 10));
}

TEST(ArithmeticTest, MultiplyTest) {
    complex_number test_number(1, 2);
    complex_number add_number(3, 4);
    test_number = test_number * add_number;
    EXPECT_EQ(test_number, complex_number(-5, 10));
}

TEST(ArithmeticTest, MultiplyDoubleTest) {
    complex_number test_number(1, 2);
    test_number = test_number * 5;
    EXPECT_EQ(test_number, complex_number(5, 10));
}

TEST(ArithmeticTest, DivideEqualTest) {
    complex_number test_number(2, 5);
    complex_number add_number(4, -1);
    test_number /= add_number;
    EXPECT_EQ(test_number, complex_number(3./17, 22./17));
}

TEST(ArithmeticTest, DivideEqualThrowsZeroDivisionExceptionTest) {
    complex_number test_number(2, 5);
    complex_number add_number(0, 0);
    EXPECT_THROW(test_number /= add_number, complex_number::ZeroDivisionException);
}

TEST(ArithmeticTest, DivideEqualDoubleTest) {
    complex_number test_number(10, 5);
    test_number /= 5;
    EXPECT_EQ(test_number, complex_number(2, 1));
}

TEST(ArithmeticTest, DivideEqualDoubleThrowsZeroDivisionExceptionTest) {
    complex_number test_number(10, 5);
    EXPECT_THROW(test_number /= 0, complex_number::ZeroDivisionException);
}

TEST(ArithmeticTest, DivideTest) {
    complex_number test_number(2, 5);
    complex_number add_number(4, -1);
    test_number = test_number / add_number;
    EXPECT_EQ(test_number, complex_number(3./17, 22./17));
}

TEST(ArithmeticTest, DivideThrowsZeroDivisionExceptionTest) {
    complex_number test_number(2, 5);
    complex_number add_number(0, 0);
    EXPECT_THROW(test_number = test_number / add_number, complex_number::ZeroDivisionException);
}

TEST(ArithmeticTest, DivideDoubleTest) {
    complex_number test_number(10, 5);
    test_number = test_number / 5;
    EXPECT_EQ(test_number, complex_number(2, 1));
}

TEST(ArithmeticTest, DivideDoubleThrowsZeroDivisionExceptionTest) {
    complex_number test_number(10, 5);
    EXPECT_THROW(test_number = test_number / 0, complex_number::ZeroDivisionException);
}

TEST(AmplitudeTest, AmplitudeTest) {
    complex_number test_number(3, 4);
    EXPECT_EQ(test_number.get_amplitude(), 25);
}

TEST(ConjugaisonTest, ConjugaisonTest) {
    complex_number test_number(3, 4);
    EXPECT_EQ(test_number.conjugate(), complex_number(3, -4));
}

TEST(ConjugaisonTest, ZeroConjugaisonTest) {
    complex_number test_number(0, 0);
    EXPECT_TRUE(test_number.conjugate().is_zero());
}

TEST(LogicalTest, EqualityTrueTest) {
    complex_number test_number(2, 3);
    complex_number another_number(2, 3);
    EXPECT_TRUE(test_number == another_number);
}

TEST(LogicalTest, EqualityFalseTest) {
    complex_number test_number(2, 3);
    complex_number another_number(-2, 3);
    EXPECT_FALSE(test_number == another_number);
}

TEST(LogicalTest, NotEqualityTrueTest) {
    complex_number test_number(2, 3);
    complex_number another_number(-2, 3);
    EXPECT_TRUE(test_number != another_number);
}

TEST(LogicalTest, NotEqualityFalseTest) {
    complex_number test_number(2, 3);
    complex_number another_number(2, 3);
    EXPECT_FALSE(test_number != another_number);
}

TEST(OutputTest, PositiveRealPositiveImaginaryTest) {
    complex_number test_number(2, 3);
    std::stringstream output;
    output << test_number;
    std::string answer("2+i3");
    EXPECT_EQ(output.str(), answer);
}

TEST(OutputTest, NegativeRealPositiveImaginaryTest) {
    complex_number test_number(-2, 3);
    std::stringstream output;
    output << test_number;
    std::string answer("-2+i3");
    EXPECT_EQ(output.str(), answer);
}

TEST(OutputTest, NegativeRealNegativeImaginaryTest) {
    complex_number test_number(-2, -3);
    std::stringstream output;
    output << test_number;
    std::string answer("-2-i3");
    EXPECT_EQ(output.str(), answer);
}

TEST(OutputTest, PositiveRealNegativeImaginaryTest) {
    complex_number test_number(2, -3);
    std::stringstream output;
    output << test_number;
    std::string answer("2-i3");
    EXPECT_EQ(output.str(), answer);
}

TEST(CastTest, StaticCastZeroTest) {
    auto test_number = static_cast<complex_number>(0);
    EXPECT_TRUE(test_number.is_zero());
}