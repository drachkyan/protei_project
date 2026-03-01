#ifndef INCLUDE_VECTOR_HPP_
#define INCLUDE_VECTOR_HPP_

#include <iostream>

enum class Types {
    U_INT64_T, U_INT16_T, U_INT,
    INT_64_T, INT_16_T, INT_T,
    DOUBLE, FLOAT, CHAR, None
};

template <typename T>
struct Vector {
    T**** arr;
    size_t m;
    size_t n;
    size_t i;
    size_t j;

    explicit Vector(const size_t m = 2, const size_t n = 2,
        const size_t i = 2, const size_t j = 2):
    m(m), n(n), i(i), j(j) {
        arr = new T***[m];
        for (int a = 0; a < m; a++) {
            arr[a] = new T**[n];
            for (int b = 0; b < n; b++) {
                arr[a][b] = new T*[i];
                for (int c = 0; c < i; c++) {
                    arr[a][b][c] = new T[i];
                    for (int d = 0; d < j; d++) {
                        arr[a][b][c][d] = static_cast<T>(0);
                    }
                }
            }
        }
    }
    ~Vector(){
        for (size_t a = 0; a < m; a++) {
            for (size_t b = 0; b < n; b++) {
                for (size_t c = 0; c < i; c++) {
                    delete[] arr[a][b][c];
                }
                delete[] arr[a][b];
            }
            delete[] arr[a];
        }
        delete[] arr;
    }
};



template <typename T>
void input_vector(Vector<T>&vector) {
    for (int m = 0; m < vector.m; m++) {
        for (size_t n = 0; n < vector.n; n++) {
            for (size_t i = 0; i < vector.i; i++) {
                for (size_t j = 0; j < vector.j; j++) {
                    std::cin >> vector.arr[m][n][i][j];
                }
            }
        }
    }
}

template <typename T>
void print_vector(const Vector<T>&vector) {
    for (size_t m = 0; m < vector.m; m++) {
        for (size_t n = 0; n < vector.n; n++) {
            for (size_t i = 0; i < vector.i; i++) {
                for (size_t j = 0; j < vector.j; j++) {
                    std::cout << vector.arr[m][n][i][j] << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }
}а

// диспатчер для того чтобы вызывать функции для
// любых типов данных по войд поинтеру

template <typename func_t>
void dispatch(const Types type, void* vectorPtr, func_t func) {
    switch (type) {
        case Types::U_INT64_T:
            func(static_cast<Vector<u_int64_t>*>(vectorPtr)); break;
        case Types::U_INT16_T:
            func(static_cast<Vector<u_int16_t>*>(vectorPtr)); break;
        case Types::U_INT:
            func(static_cast<Vector<u_int32_t>*>(vectorPtr)); break;
        case Types::INT_64_T:
            func(static_cast<Vector<int64_t>*>(vectorPtr)); break;
        case Types::INT_16_T:
            func(static_cast<Vector<int16_t>*>(vectorPtr)); break;
        case Types::INT_T:
            func(static_cast<Vector<int32_t>*>(vectorPtr)); break;
        case Types::FLOAT:
            func(static_cast<Vector<float>*>(vectorPtr)); break;
        case Types::CHAR:
            func(static_cast<Vector<char>*>(vectorPtr)); break;
        case Types::DOUBLE:
            func(static_cast<Vector<double>*>(vectorPtr)); break;
        default: break;
    }
}

#endif  // INCLUDE_VECTOR_HPP_
