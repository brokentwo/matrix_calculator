/**
 * @file matrix.h
 * @brief Библиотека для работы с матрицами (калькулятор матриц).
 * @author Дурнов Е. К.
 * @version 1.0
 *
 * Способ обработки ошибок — исключения: при некорректных размерах
 * или несовместимых матрицах функции бросают std::invalid_argument,
 * при нехватке памяти — std::bad_alloc.
 */
#ifndef MATRIX_H
#define MATRIX_H

namespace matrix {

/**
 * @brief Структура матрицы.
 *
 * Хранит двумерный динамический массив и его размеры.
 * @warning Память освобождается через free_matrix().
 */
struct Matrix {
    double** data;  ///< Указатель на двумерный массив (массив строк).
    int rows;       ///< Количество строк.
    int cols;       ///< Количество столбцов.
};

/**
 * @brief Создание матрицы заданного размера.
 *
 * @param rows Количество строк (> 0).
 * @param cols Количество столбцов (> 0).
 * @return Matrix Инициализированная нулями матрица.
 *
 * @exception std::invalid_argument Если rows <= 0 или cols <= 0.
 * @exception std::bad_alloc Если не удалось выделить память.
 *
 * @note Время выполнения: O(rows * cols).
 * @see free_matrix()
 *
 * @code
 * Matrix A = create_matrix(3, 3);
 * @endcode
 */
Matrix create_matrix(int rows, int cols);

/**
 * @brief Освобождение памяти, занятой матрицей.
 *
 * Безопасна для пустой матрицы (data == nullptr).
 *
 * @param m Матрица, память которой нужно освободить.
 *
 * @warning Функция не обнуляет переданный оригинал: после вызова
 *          матрицу нельзя использовать и повторно освобождать.
 * @see create_matrix()
 */
void free_matrix(Matrix m);

/**
 * @brief Сложение двух матриц: C = A + B, c[i][j] = a[i][j] + b[i][j].
 *
 * @param a Первая матрица M×N.
 * @param b Вторая матрица M×N.
 * @return Matrix Новая матрица M×N — сумма.
 *
 * @exception std::invalid_argument Если матрицы пустые или их размеры
 *            не совпадают.
 *
 * @note Время выполнения: O(M*N).
 * @warning Результат нужно освобождать через free_matrix().
 */
Matrix matrix_add(Matrix a, Matrix b);

/**
 * @brief Умножение двух матриц: C = A × B, c[i][j] = Σ a[i][k] * b[k][j].
 *
 * @param a Матрица M×N.
 * @param b Матрица N×K.
 * @return Matrix Результат M×K.
 *
 * @exception std::invalid_argument Если матрицы пустые или cols(a) != rows(b).
 *
 * @note Время выполнения: O(M*N*K).
 * @warning Результат нужно освобождать через free_matrix().
 */
Matrix matrix_multiply(Matrix a, Matrix b);

/**
 * @brief Вывод матрицы в консоль в читаемом формате.
 *
 * Числа выводятся с двумя знаками после запятой и выравниваются по столбцам.
 * Для больших матриц (больше 10×8) показываются первые и последние
 * строки/столбцы, пропущенная часть заменяется многоточием.
 *
 * @param m Матрица для вывода. Для пустой матрицы выводится "[Empty matrix]".
 */
void print_matrix(Matrix m);

/**
 * @brief Создание матрицы из одномерного массива (заполнение по строкам).
 *
 * data[0]..data[cols-1] — первая строка, data[cols]..data[2*cols-1] — вторая и т.д.
 *
 * @param data Указатель на массив из rows*cols элементов.
 * @param rows Количество строк (> 0).
 * @param cols Количество столбцов (> 0).
 * @return Matrix Новая матрица с данными из массива.
 *
 * @exception std::invalid_argument Если data == nullptr или размеры <= 0.
 *
 * @code
 * double arr[] = {1, 2, 3, 4, 5, 6};
 * Matrix m = matrix_from_array(arr, 2, 3);  // [1 2 3]
 *                                           // [4 5 6]
 * @endcode
 */
Matrix matrix_from_array(const double* data, int rows, int cols);
/**
 * @brief Проверка равенства двух матриц с заданной точностью.
 *
 * Матрицы равны, если у них одинаковые размеры и для всех i, j
 * выполняется |a[i][j] - b[i][j]| <= tolerance.
 *
 * @param a Первая матрица.
 * @param b Вторая матрица.
 * @param tolerance Допустимая погрешность сравнения (>= 0).
 * @return true, если матрицы равны с точностью tolerance;
 *         false, если размеры различаются или хотя бы один элемент
 *         отличается больше чем на tolerance.
 *         Две пустые матрицы считаются равными.
 *
 * @exception std::invalid_argument Если tolerance < 0.
 *
 * @note Время выполнения: O(rows * cols).
 *
 * @code
 * bool same = matrix_equals(A, B, 1e-9);
 * @endcode
 */
bool matrix_equals(Matrix a, Matrix b, double tolerance);

}  // namespace matrix

#endif  // MATRIX_H