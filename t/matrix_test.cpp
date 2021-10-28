#include <gmock/gmock.h>

#include "../matrix.h"


int fill_int_matrix(matrix<int>& matrix_to_fill) {
    for (size_t row = 0; row < matrix_to_fill.get_row_number(); row++) {
        for (size_t col = 0; col < matrix_to_fill.get_col_number(); col++) {
            matrix_to_fill[row][col] = row + col;
        }
    }
    return 0;
}

TEST(SizeGettersTest, RowNumberTest) {
    matrix<int> test_matrix(5, 6);
    size_t row_number = 5;
    EXPECT_EQ(row_number, test_matrix.get_row_number());
}

TEST(SizeGettersTest, ColNumberTest) {
    matrix<int> test_matrix(5, 6);
    size_t col_number = 6;
    EXPECT_EQ(col_number, test_matrix.get_col_number());
}

TEST(ConstructorTest, ConstructorWithRowsAndColsTest) {
    matrix<int> test_matrix(5, 6);
    for (size_t row = 0; row < test_matrix.get_row_number(); row++) {
        for (size_t col = 0; col < test_matrix.get_col_number(); col++) {
            EXPECT_EQ(0, test_matrix[row][col]);
        }
    }
}

TEST(ConstructorTest, CopyConstructorTest) {
    matrix<int> base_matrix(5, 6);
    fill_int_matrix(base_matrix);
    matrix<int> test_matrix(base_matrix);
    EXPECT_TRUE(test_matrix.is_equal(base_matrix));
}

TEST(ConstructorTest, MoveConstructorTest) {
    matrix<int> base_matrix(5, 6);
    matrix<int> expected_matrix(5, 6);
    fill_int_matrix(base_matrix);
    fill_int_matrix(expected_matrix);
    matrix<int> test_matrix(std::move(base_matrix));
    EXPECT_TRUE(test_matrix.is_equal(expected_matrix));
}

TEST(AssignmentTest, CopyAssignmentTest) {
    matrix<int> base_matrix(5, 6);
    fill_int_matrix(base_matrix);
    matrix<int> test_matrix(0, 0);
    test_matrix = base_matrix;
    EXPECT_TRUE(test_matrix.is_equal(base_matrix));
}

TEST(AssignmentTest, MoveAssignmentTest) {
    matrix<int> base_matrix(5, 6);
    fill_int_matrix(base_matrix);
    matrix<int> expected_matrix(5, 6);
    fill_int_matrix(expected_matrix);
    matrix<int> test_matrix(0, 0);
    test_matrix = std::move(base_matrix);
    EXPECT_TRUE(test_matrix.is_equal(expected_matrix));
}

TEST(ArithmeticsTest, OperatorPlusAssignTest) {
    matrix<int> base_matrix(5, 6);
    fill_int_matrix(base_matrix);
    matrix<int> test_matrix(5, 6);
    test_matrix += base_matrix;
    EXPECT_TRUE(test_matrix.is_equal(base_matrix));
}

TEST(ArithmeticsTest, OperatorMinusAssignTest) {
    matrix<int> base_matrix(5, 6);
    fill_int_matrix(base_matrix);
    matrix<int> test_matrix(5, 6);
    test_matrix -= base_matrix;
    for (size_t row = 0; row < test_matrix.get_row_number(); row++) {
        for (size_t col = 0; col < test_matrix.get_col_number(); col++) {
            EXPECT_EQ(-row-col, test_matrix[row][col]);
        }
    }
}

TEST(ArithmeticsTest, OperatorMinusTest) {
    matrix<int> base_matrix1(5, 6);
    fill_int_matrix(base_matrix1);
    matrix<int> base_matrix2(5, 6);
    matrix<int> test_matrix = base_matrix2 - base_matrix1;
    for (size_t row = 0; row < test_matrix.get_row_number(); row++) {
        for (size_t col = 0; col < test_matrix.get_col_number(); col++) {
            EXPECT_EQ(-row-col, test_matrix[row][col]);
        }
    }
}

TEST(ArithmeticsTest, OperatorMultiplyAssignTest) {
    matrix<int> base_matrix(3, 2);
    matrix<int> test_matrix(2, 3);
    fill_int_matrix(base_matrix);
    fill_int_matrix(test_matrix);
    test_matrix *= base_matrix;
    EXPECT_EQ(2, test_matrix.get_row_number());
    EXPECT_EQ(2, test_matrix.get_col_number());
    EXPECT_EQ(5, test_matrix[0][0]);
    EXPECT_EQ(8, test_matrix[0][1]);
    EXPECT_EQ(8, test_matrix[1][0]);
    EXPECT_EQ(14, test_matrix[1][1]);
}

TEST(ArithmeticsTest, OperatorMultiplyTest) {
    matrix<int> base_matrix1(3, 2);
    matrix<int> base_matrix2(2, 3);
    fill_int_matrix(base_matrix1);
    fill_int_matrix(base_matrix2);
    matrix<int> test_matrix = base_matrix2 * base_matrix1;
    EXPECT_EQ(2, test_matrix.get_row_number());
    EXPECT_EQ(2, test_matrix.get_col_number());
    EXPECT_EQ(5, test_matrix[0][0]);
    EXPECT_EQ(8, test_matrix[0][1]);
    EXPECT_EQ(8, test_matrix[1][0]);
    EXPECT_EQ(14, test_matrix[1][1]);
}

TEST(ArithmeticsTest, OperatorPlusAssignThrowsExceptionTest) {
    matrix<int> base_matrix(3, 4);
    matrix<int> test_matrix(4, 4);
    EXPECT_THROW(test_matrix += base_matrix, std::runtime_error);
}

TEST(ArithmeticsTest, OperatorMinusAssignThrowsExceptionTest) {
    matrix<int> base_matrix(3, 4);
    matrix<int> test_matrix(4, 4);
    EXPECT_THROW(test_matrix -= base_matrix, std::runtime_error);
}

TEST(ArithmeticsTest, OperatorMultiplyAssignThrowsExceptionTest) {
    matrix<int> base_matrix(3, 4);
    matrix<int> test_matrix(4, 4);
    EXPECT_THROW(test_matrix *= base_matrix, std::runtime_error);
}

TEST(TransposeTest, TransposeTest) {
    matrix<int> base_matrix(3, 4);
    matrix<int> test_matrix = base_matrix.transpose();
    EXPECT_EQ(4, test_matrix.get_row_number());
    EXPECT_EQ(3, test_matrix.get_col_number());
    for (size_t row = 0; row < base_matrix.get_row_number(); row++) {
        for (size_t col = 0; col < base_matrix.get_col_number(); col++) {
            EXPECT_EQ(base_matrix[row][col], test_matrix[col][row]);
        }
    }
}

TEST(DeterminantTest, DeterminantTest) {
    matrix<double> test_matrix(3, 3);
    test_matrix[0][0] = 1.;
    test_matrix[0][1] = -2.;
    test_matrix[0][2] = 3.;
    test_matrix[1][0] = 4.;
    test_matrix[1][1] = 0.;
    test_matrix[1][2] = 6.;
    test_matrix[2][0] = -7.;
    test_matrix[2][1] = 8.;
    test_matrix[2][2] = 9.;
    EXPECT_DOUBLE_EQ(204., test_matrix.get_determinant());
}

TEST(DeterminantTest, GetDeterminantThrowsExceptionTest) {
    matrix<double> test_matrix(3, 4);
    EXPECT_THROW(test_matrix.get_determinant(), std::runtime_error);
}

TEST(InverseMatrixTest, InverseMatrixTest) {
    matrix<double> test_matrix(3, 3);
    test_matrix[0][0] = -1.;
    test_matrix[0][1] = 2.;
    test_matrix[0][2] = -2.;
    test_matrix[1][0] = 2.;
    test_matrix[1][1] = -1.;
    test_matrix[1][2] = 5.;
    test_matrix[2][0] = 3.;
    test_matrix[2][1] = -2.;
    test_matrix[2][2] = 4.;
    matrix<double> expected_matrix(3, 3);
    expected_matrix[0][0] = 0.6;
    expected_matrix[0][1] = -0.4;
    expected_matrix[0][2] = 0.8;
    expected_matrix[1][0] = 0.7;
    expected_matrix[1][1] = 0.2;
    expected_matrix[1][2] = 0.1;
    expected_matrix[2][0] = -0.1;
    expected_matrix[2][1] = 0.4;
    expected_matrix[2][2] = -0.3;
    EXPECT_TRUE(test_matrix.inverse().is_equal(expected_matrix));
}

TEST(InverseMatrixTest, InverseThrowsExceptionTest) {
    matrix<double> test_matrix(2, 3);
    EXPECT_THROW(test_matrix.inverse(), std::runtime_error);
}

//TEST(BadAllocationTest, ConstructorThrowsBadAllocationTest) {
//    EXPECT_THROW(matrix<double> test_matrix(100000, 100000), std::bad_alloc);
//}
//
//TEST(BadAllocationTest, CopyConstructorThrowsBadAllocationTest) {
//    matrix<double> test_matrix(50000, 50000);
//    EXPECT_THROW(matrix<double> copy_test_matrix(test_matrix), std::bad_alloc);
//}
//
//TEST(BadAllocationTest, CopyAssignmentThrowsBadAllocationTest) {
//    matrix<double> test_matrix(50000, 50000), copy_test_matrix;
//    EXPECT_THROW(copy_test_matrix = test_matrix, std::bad_alloc);
//}