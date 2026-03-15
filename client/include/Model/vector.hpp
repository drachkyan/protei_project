#ifndef INCLUDE_VECTOR_HPP_
#define INCLUDE_VECTOR_HPP_

#include <cstdint>
#include <iostream>
#include <variant>
#include <memory>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


template <typename T>
struct Vector {
    using value_type = T;
    T**** arr;
    size_t m;
    size_t n;
    size_t i;
    size_t j;
    const std::string type_name{};
    explicit Vector(std::string type_name_, const size_t m = 2, const size_t n = 2,
        const size_t i = 2, const size_t j = 2):
   m(m), n(n), i(i), j(j), type_name(std::move(type_name_)) {
        arr = new T***[m];
        for (int a = 0; a < m; a++) {
            arr[a] = new T**[n];
            for (int b = 0; b < n; b++) {
                arr[a][b] = new T*[i];
                for (int c = 0; c < i; c++) {
                    arr[a][b][c] = new T[j];
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
}


using AnyVector = std::variant<
    std::unique_ptr<Vector<uint64_t>>,
    std::unique_ptr<Vector<uint16_t>>,
    std::unique_ptr<Vector<uint32_t>>,
    std::unique_ptr<Vector<int64_t>>,
    std::unique_ptr<Vector<int16_t>>,
    std::unique_ptr<Vector<int32_t>>,
    std::unique_ptr<Vector<float>>,
    std::unique_ptr<Vector<double>>,
    std::unique_ptr<Vector<char>>
>;

template <typename T>
void vectorToJson(json& j, const Vector<T>& v) {
    j["m"] = v.m;
    j["n"] = v.n;
    j["i"] = v.i;
    j["j"] = v.j;

    json arr = json::array();
    for (size_t a = 0; a < v.m; a++)
        for (size_t b = 0; b < v.n; b++)
            for (size_t c = 0; c < v.i; c++)
                for (size_t d = 0; d < v.j; d++)
                    arr.push_back(v.arr[a][b][c][d]);

    j["data"] = arr;
}




#endif  // INCLUDE_VECTOR_HPP_
