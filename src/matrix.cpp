#include "matrix.h"
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <new>
#include <stdexcept>
#include <string>
#include <vector>

namespace matrix {

namespace {

constexpr int kMaxRowsToShow = 10;
constexpr int kMaxColsToShow = 8;
constexpr int kPrecision = 2;
constexpr int kCellWidth = 7;

bool is_empty(const Matrix& m) {
    return m.data == nullptr || m.rows <= 0 || m.cols <= 0;
}

void require_not_empty(const Matrix& m, const char* name) {
    if (is_empty(m)) {
        throw std::invalid_argument(std::string("Matrix ") + name + " is empty");
    }
}

// Номера строк/столбцов, которые будут показаны (-1 — место многоточия).
std::vector<int> visible_indices(int count, int limit) {
    std::vector<int> idx;
    if (count <= limit) {
        for (int i = 0; i < count; ++i) idx.push_back(i);
        return idx;
    }
    const int head = limit / 2;
    for (int i = 0; i < head; ++i) idx.push_back(i);
    idx.push_back(-1);
    for (int i = count - (limit - head); i < count; ++i) idx.push_back(i);
    return idx;
}

// row < 0 — строка-заглушка из многоточий на месте пропущенных строк.
void print_row(const Matrix& m, int row, const std::vector<int>& cols) {
    std::cout << "[";
    for (std::size_t k = 0; k < cols.size(); ++k) {
        if (row < 0 || cols[k] < 0) {
            std::cout << std::setw(kCellWidth) << "...";
        } else {
            std::cout << std::setw(kCellWidth) << m.data[row][cols[k]];
        }
    }
    std::cout << " ]\n";
}

}  // namespace

Matrix create_matrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive integers");
    }
    Matrix mat{nullptr, rows, cols};
    mat.data = new double*[rows];
    int allocated = 0;
    try {
        for (; allocated < rows; ++allocated) {
            mat.data[allocated] = new double[cols]();  // () — инициализация нулями
        }
    } catch (const std::bad_alloc&) {
        for (int i = 0; i < allocated; ++i) delete[] mat.data[i];
        delete[] mat.data;
        throw;
    }
    return mat;
}

void free_matrix(Matrix m) {
    if (m.data == nullptr) return;
    for (int i = 0; i < m.rows; ++i) {
        delete[] m.data[i];
        m.data[i] = nullptr;
    }
    delete[] m.data;
}

Matrix matrix_add(Matrix a, Matrix b) {
    require_not_empty(a, "a");
    require_not_empty(b, "b");
    if (a.rows != b.rows || a.cols != b.cols) {
        throw std::invalid_argument("Matrix sizes must match for addition");
    }
    Matrix c = create_matrix(a.rows, a.cols);
    for (int i = 0; i < a.rows; ++i) {
        for (int j = 0; j < a.cols; ++j) {
            c.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }
    return c;
}

Matrix matrix_multiply(Matrix a, Matrix b) {
    require_not_empty(a, "a");
    require_not_empty(b, "b");
    if (a.cols != b.rows) {
        throw std::invalid_argument("cols(a) must equal rows(b) for multiplication");
    }
    Matrix c = create_matrix(a.rows, b.cols);
    // Порядок i-k-j: строки b и c читаются последовательно (лучше для кэша).
    for (int i = 0; i < a.rows; ++i) {
        for (int k = 0; k < a.cols; ++k) {
            const double aik = a.data[i][k];
            for (int j = 0; j < b.cols; ++j) {
                c.data[i][j] += aik * b.data[k][j];
            }
        }
    }
    return c;
}

void print_matrix(Matrix m) {
    if (is_empty(m)) {
        std::cout << "[Empty matrix]\n";
        return;
    }
    const std::ios_base::fmtflags old_flags = std::cout.flags();
    const std::streamsize old_precision = std::cout.precision();
    std::cout << std::fixed << std::setprecision(kPrecision);

    const std::vector<int> rows = visible_indices(m.rows, kMaxRowsToShow);
    const std::vector<int> cols = visible_indices(m.cols, kMaxColsToShow);
    for (int r : rows) {
        print_row(m, r, cols);
    }
    if (m.rows > kMaxRowsToShow || m.cols > kMaxColsToShow) {
        std::cout << "Matrix " << m.rows << "x" << m.cols << " (shortened view)\n";
    }

    std::cout.flags(old_flags);
    std::cout.precision(old_precision);
}

Matrix matrix_from_array(const double* data, int rows, int cols) {
    if (data == nullptr) {
        throw std::invalid_argument("Source array is nullptr");
    }
    Matrix m = create_matrix(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            m.data[i][j] = data[i * cols + j];
        }
    }
    return m;
}
bool matrix_equals(Matrix a, Matrix b, double tolerance) {
    if (tolerance < 0.0) {
        throw std::invalid_argument("Tolerance must be non-negative");
    }
    const bool a_empty = is_empty(a);
    const bool b_empty = is_empty(b);
    if (a_empty || b_empty) {
        return a_empty && b_empty;
    }
    if (a.rows != b.rows || a.cols != b.cols) {
        return false;  // разные размеры — матрицы не равны
    }
    for (int i = 0; i < a.rows; ++i) {
        for (int j = 0; j < a.cols; ++j) {
            if (std::fabs(a.data[i][j] - b.data[i][j]) > tolerance) {
                return false;  // нашли отличающийся элемент — дальше не проверяем
            }
        }
    }
    return true;
}

}  // namespace matrix