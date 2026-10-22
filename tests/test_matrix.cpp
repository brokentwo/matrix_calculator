#include <gtest/gtest.h>

#include <stdexcept>

#include "matrix.h"

using namespace matrix;  // функции библиотеки лежат в namespace matrix

namespace {
constexpr double kTolerance = 1e-9;
}  // namespace

// ===== 1. НОРМАЛЬНЫЕ СЛУЧАИ =====

// 1.1. Создание матрицы
TEST(MatrixTest, CreateMatrix_NormalCase) {
    Matrix A = create_matrix(3, 4);
    EXPECT_EQ(A.rows, 3);
    EXPECT_EQ(A.cols, 4);
    EXPECT_NE(A.data, nullptr);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            EXPECT_DOUBLE_EQ(A.data[i][j], 0.0);
        }
    }
    free_matrix(A);
}

// 1.2. Сложение матриц
TEST(MatrixTest, Addition_NormalCase) {
    Matrix A = create_matrix(2, 2);
    Matrix B = create_matrix(2, 2);
    A.data[0][0] = 1;
    A.data[0][1] = 2;
    A.data[1][0] = 3;
    A.data[1][1] = 4;
    B.data[0][0] = 5;
    B.data[0][1] = 6;
    B.data[1][0] = 7;
    B.data[1][1] = 8;
    Matrix C = matrix_add(A, B);
    EXPECT_DOUBLE_EQ(C.data[0][0], 6.0);
    EXPECT_DOUBLE_EQ(C.data[0][1], 8.0);
    EXPECT_DOUBLE_EQ(C.data[1][0], 10.0);
    EXPECT_DOUBLE_EQ(C.data[1][1], 12.0);
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}

// 1.3. Умножение матриц
TEST(MatrixTest, Multiplication_NormalCase) {
    Matrix A = create_matrix(2, 2);
    Matrix B = create_matrix(2, 2);
    A.data[0][0] = 1;
    A.data[0][1] = 2;
    A.data[1][0] = 3;
    A.data[1][1] = 4;
    B.data[0][0] = 5;
    B.data[0][1] = 6;
    B.data[1][0] = 7;
    B.data[1][1] = 8;
    Matrix C = matrix_multiply(A, B);
    EXPECT_DOUBLE_EQ(C.data[0][0], 19.0);
    EXPECT_DOUBLE_EQ(C.data[0][1], 22.0);
    EXPECT_DOUBLE_EQ(C.data[1][0], 43.0);
    EXPECT_DOUBLE_EQ(C.data[1][1], 50.0);
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}

// 1.4. Освобождение памяти — нормальный случай
TEST(MatrixTest, FreeMatrix_NormalCase) {
    Matrix A = create_matrix(3, 3);
    EXPECT_NO_THROW(free_matrix(A));
}

// ===== 2. ГРАНИЧНЫЕ СЛУЧАИ =====

// 2.1. Минимальный размер матрицы — 1×1
TEST(MatrixTest, CreateMatrix_MinimumSize) {
    Matrix A = create_matrix(1, 1);
    EXPECT_EQ(A.rows, 1);
    EXPECT_EQ(A.cols, 1);
    EXPECT_DOUBLE_EQ(A.data[0][0], 0.0);
    free_matrix(A);
}

// 2.2. Вектор-строка (1×3) × вектор-столбец (3×1) = матрица 1×1
TEST(MatrixTest, Multiplication_RowTimesColumn_1x1) {
    Matrix A = create_matrix(1, 3);
    Matrix B = create_matrix(3, 1);
    A.data[0][0] = 1;
    A.data[0][1] = 2;
    A.data[0][2] = 3;
    B.data[0][0] = 4;
    B.data[1][0] = 5;
    B.data[2][0] = 6;
    Matrix C = matrix_multiply(A, B);
    EXPECT_EQ(C.rows, 1);
    EXPECT_EQ(C.cols, 1);
    EXPECT_DOUBLE_EQ(C.data[0][0], 32.0);  // 1*4 + 2*5 + 3*6
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}

// ===== 3. НЕКОРРЕКТНЫЕ СЛУЧАИ =====

// 3.1. Создание матрицы с нулевым числом строк
TEST(MatrixTest, CreateMatrix_ZeroRows) {
    EXPECT_THROW(create_matrix(0, 3), std::invalid_argument);
}

// 3.2. Создание матрицы с отрицательным числом столбцов
TEST(MatrixTest, CreateMatrix_NegativeCols) {
    EXPECT_THROW(create_matrix(3, -1), std::invalid_argument);
}

// 3.3. Сложение матриц разных размеров
TEST(MatrixTest, Addition_SizeMismatch) {
    Matrix A = create_matrix(2, 2);
    Matrix B = create_matrix(3, 3);
    EXPECT_THROW(matrix_add(A, B), std::invalid_argument);
    free_matrix(A);
    free_matrix(B);
}

// 3.4. Умножение матриц с несовместимыми размерами
TEST(MatrixTest, Multiplication_SizeMismatch) {
    Matrix A = create_matrix(2, 3);
    Matrix B = create_matrix(2, 2);
    EXPECT_THROW(matrix_multiply(A, B), std::invalid_argument);
    free_matrix(A);
    free_matrix(B);
}

// 3.5. Повторное освобождение пустой матрицы безопасно
TEST(MatrixTest, FreeMatrix_EmptyIsSafe) {
    Matrix empty{nullptr, 0, 0};
    EXPECT_NO_THROW(free_matrix(empty));
    EXPECT_NO_THROW(free_matrix(empty));
}

// ===== 4. ИНДИВИДУАЛЬНОЕ ЗАДАНИЕ: matrix_equals (вариант 5) =====

// 4.1. Одинаковые матрицы равны
TEST(MatrixTest, Equals_SameMatrices) {
    double values[] = {1, 2, 3, 4};
    Matrix A = matrix_from_array(values, 2, 2);
    Matrix B = matrix_from_array(values, 2, 2);
    EXPECT_TRUE(matrix_equals(A, B, kTolerance));
    free_matrix(A);
    free_matrix(B);
}

// 4.2. Разница меньше точности — матрицы считаются равными
TEST(MatrixTest, Equals_WithinTolerance) {
    double a_values[] = {1.0, 2.0, 3.0, 4.0};
    double b_values[] = {1.0 + 1e-12, 2.0, 3.0, 4.0};
    Matrix A = matrix_from_array(a_values, 2, 2);
    Matrix B = matrix_from_array(b_values, 2, 2);
    EXPECT_TRUE(matrix_equals(A, B, kTolerance));
    free_matrix(A);
    free_matrix(B);
}

// 4.3. Отличается один элемент — матрицы не равны
TEST(MatrixTest, Equals_DifferentValues) {
    double a_values[] = {1, 2, 3, 4};
    double b_values[] = {1, 2, 3, 5};
    Matrix A = matrix_from_array(a_values, 2, 2);
    Matrix B = matrix_from_array(b_values, 2, 2);
    EXPECT_FALSE(matrix_equals(A, B, kTolerance));
    free_matrix(A);
    free_matrix(B);
}

// 4.4. Разные размеры — не равны, исключения нет
TEST(MatrixTest, Equals_DifferentSizes) {
    Matrix A = create_matrix(2, 2);
    Matrix B = create_matrix(1, 3);
    EXPECT_FALSE(matrix_equals(A, B, kTolerance));
    free_matrix(A);
    free_matrix(B);
}

// 4.5. Отрицательная точность — исключение
TEST(MatrixTest, Equals_NegativeTolerance) {
    Matrix A = create_matrix(2, 2);
    EXPECT_THROW(matrix_equals(A, A, -1.0), std::invalid_argument);
    free_matrix(A);
}