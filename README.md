# Matrix Calculator

Лабораторная работа № 3 «Полный цикл разработки калькулятора матриц на C++».
Дурнов Е. К., ИУ5-14Б. Индивидуальное задание — вариант 5 (`matrix_equals`).

## Описание задачи

Библиотека для работы с матрицами `double` на C++17: создание и освобождение
матриц, сложение, умножение, вывод, создание из массива и проверка равенства
матриц с заданной точностью. Проект покрыт unit-тестами (Google Test),
документирован (Doxygen), собирается в Docker-контейнер и проверяется в CI
(GitHub Actions).

## Структура

```
include/matrix.h        — интерфейс библиотеки (+ комментарии Doxygen)
src/matrix.cpp          — реализация
src/main.cpp            — демонстрационная программа
tests/test_matrix.cpp   — unit-тесты Google Test
tests/CMakeLists.txt    — сборка тестов (googletest через FetchContent)
.github/workflows/ci.yml— CI: сборка, тесты, Valgrind, Doxygen, Docker
CMakeLists.txt, Doxyfile, Dockerfile, .clang-format, .gitignore
```

## Сборка и запуск

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/matrix_calculator          # демонстрационная программа
cd build && ctest --verbose        # тесты
```

Документация: `doxygen Doxyfile`, затем открыть `docs/html/index.html`.

Docker:

```bash
docker build -t matrix-calculator .
docker run -it --rm matrix-calculator
```

## Обработка ошибок

Используются **исключения**:

- `std::invalid_argument` — неположительные размеры, пустая матрица,
  несовместимые размеры при сложении/умножении, `nullptr` вместо массива,
  отрицательная точность в `matrix_equals`;
- `std::bad_alloc` — нехватка памяти (уже выделенная память освобождается).

`main` перехватывает исключения и завершается с кодом 1.

## Функции

| Подзадача | Функция |
|---|---|
| Создание матрицы, заполненной нулями | `create_matrix(rows, cols)` |
| Освобождение памяти | `free_matrix(m)` |
| Сложение | `matrix_add(a, b)` |
| Умножение | `matrix_multiply(a, b)` |
| Вывод в консоль | `print_matrix(m)` |
| Создание из одномерного массива | `matrix_from_array(data, rows, cols)` |
| Проверка равенства с точностью (вариант 5) | `matrix_equals(a, b, tolerance)` |

Все функции находятся в пространстве имён `matrix`.

## Проверяемые сценарии (tests/test_matrix.cpp, 16 тестов)

- нормальные: создание 3×4, сложение 2×2, умножение 2×2, освобождение;
- граничные: матрица 1×1, вектор-строка × вектор-столбец → 1×1;
- некорректные: нулевые/отрицательные размеры, сложение и умножение
  несовместимых матриц, повторное освобождение пустой матрицы;
- вариант 5: одинаковые матрицы, разница меньше точности, разные значения,
  разные размеры, отрицательная точность.

## Пример

Вход: `A = [1 2; 3 4]`, `B = [5 6; 7 8]`.

```
Sum A+B:
[   6.00   8.00 ]
[  10.00  12.00 ]
Product A*B:
[  19.00  22.00 ]
[  43.00  50.00 ]
A*B equals [19 22; 43 50]: true
A equals B: false
A (2x2) equals row (1x3): false
```
