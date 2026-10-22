#include <exception>
#include <iostream>

#include "matrix.h"

namespace {

constexpr double kTolerance = 1e-9;

void print_titled(const char* title, matrix::Matrix m) {
    std::cout << title << ":\n";
    matrix::print_matrix(m);
}

void print_check(const char* title, bool value) {
    std::cout << title << ": " << (value ? "true" : "false") << '\n';
}

}  // namespace

int main() {
    using matrix::Matrix;

    try {
        const double a_values[] = {1, 2, 3, 4};
        const double b_values[] = {5, 6, 7, 8};
        const double expected_values[] = {19, 22, 43, 50};
        const double row_values[] = {1, 2, 3};
        Matrix a = matrix::matrix_from_array(a_values, 2, 2);
        Matrix b = matrix::matrix_from_array(b_values, 2, 2);
        Matrix expected = matrix::matrix_from_array(expected_values, 2, 2);
        Matrix row = matrix::matrix_from_array(row_values, 1, 3);

        Matrix sum = matrix::matrix_add(a, b);
        Matrix product = matrix::matrix_multiply(a, b);

        print_titled("Matrix A", a);
        print_titled("Matrix B", b);
        print_titled("Sum A+B", sum);
        print_titled("Product A*B", product);

        print_check("A*B equals [19 22; 43 50]", matrix::matrix_equals(product, expected, kTolerance));
        print_check("A equals B", matrix::matrix_equals(a, b, kTolerance));
        print_check("A (2x2) equals row (1x3)", matrix::matrix_equals(a, row, kTolerance));

        matrix::free_matrix(a);
        matrix::free_matrix(b);
        matrix::free_matrix(expected);
        matrix::free_matrix(row);
        matrix::free_matrix(sum);
        matrix::free_matrix(product);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}