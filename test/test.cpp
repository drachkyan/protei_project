#include <gtest/gtest.h>
#include "../include/vector.hpp"
#include "../include/DataPool.h"
#include "../include/NetworkAddress.h"
#include "../include/AppSettings.h"
#include <spdlog/spdlog.h>

TEST(VectorTest, DefaultDimensionsAreTwo) {
    Vector<int> v;
    EXPECT_EQ(v.m, 2u) << "Размерность m по умолчанию должна быть 2, а не " << v.m;
    EXPECT_EQ(v.n, 2u) << "Размерность n по умолчанию должна быть 2, а не " << v.n;
    EXPECT_EQ(v.i, 2u) << "Размерность i по умолчанию должна быть 2, а не " << v.i;
    EXPECT_EQ(v.j, 2u) << "Размерность j по умолчанию должна быть 2, а не " << v.j;
}

TEST(VectorTest, AllElementsInitialisedToZero) {
    Vector<double> v(2, 2, 2, 2);
    for (size_t a = 0; a < v.m; ++a)
        for (size_t b = 0; b < v.n; ++b)
            for (size_t c = 0; c < v.i; ++c)
                for (size_t d = 0; d < v.j; ++d)
                    EXPECT_DOUBLE_EQ(v.arr[a][b][c][d], 0.0)
                        << "Элемент [" << a << "][" << b << "][" << c << "][" << d
                        << "] должен быть 0.0 после создания, а равен "
                        << v.arr[a][b][c][d];
}

TEST(VectorTest, CustomDimensions) {
    Vector<int> v(3, 4, 2, 5);
    EXPECT_EQ(v.m, 3u) << "Передали m=3, но сохранилось m=" << v.m;
    EXPECT_EQ(v.n, 4u) << "Передали n=4, но сохранилось n=" << v.n;
    EXPECT_EQ(v.i, 2u) << "Передали i=2, но сохранилось i=" << v.i;
    EXPECT_EQ(v.j, 5u) << "Передали j=5, но сохранилось j=" << v.j;
}

TEST(VectorTest, WriteAndReadElement) {
    Vector<int> v(2, 2, 2, 2);
    v.arr[1][1][1][1] = 42;
    EXPECT_EQ(v.arr[1][1][1][1], 42)
        << "Записали 42 в arr[1][1][1][1], но прочитали " << v.arr[1][1][1][1];
}

TEST(VectorTest, FloatElementsInitialisedToZero) {
    Vector<float> v(1, 1, 1, 3);
    for (size_t d = 0; d < 3; ++d)
        EXPECT_FLOAT_EQ(v.arr[0][0][0][d], 0.0f)
            << "float-элемент [0][0][0][" << d << "] должен быть 0.0f, а равен "
            << v.arr[0][0][0][d];
}

TEST(VectorTest, CharElementsInitialisedToZero) {
    Vector<char> v(2, 2, 2, 2);
    for (size_t a = 0; a < v.m; ++a)
        for (size_t b = 0; b < v.n; ++b)
            for (size_t c = 0; c < v.i; ++c)
                for (size_t d = 0; d < v.j; ++d)
                    EXPECT_EQ(v.arr[a][b][c][d], '\0')
                        << "char-элемент [" << a << "][" << b << "][" << c << "][" << d
                        << "] должен быть '\\0', а равен " << (int)v.arr[a][b][c][d];
}

TEST(VectorTest, LargeDimensionsAllocateCorrectly) {
    Vector<int64_t> v(4, 4, 4, 4);
    for (size_t a = 0; a < v.m; ++a)
        for (size_t b = 0; b < v.n; ++b)
            for (size_t c = 0; c < v.i; ++c)
                for (size_t d = 0; d < v.j; ++d)
                    v.arr[a][b][c][d] = static_cast<int64_t>(a + b + c + d);
    EXPECT_EQ(v.arr[3][3][3][3], 12)
        << "arr[3][3][3][3] должен быть 3+3+3+3=12, а равен " << v.arr[3][3][3][3];
}

TEST(VectorTest, UnsignedTypesWork) {
    Vector<uint16_t> v(1, 1, 1, 1);
    v.arr[0][0][0][0] = 65535u;
    EXPECT_EQ(v.arr[0][0][0][0], 65535u)
        << "Записали 65535 (uint16_t max), но прочитали " << v.arr[0][0][0][0];
}

TEST(DataPoolTest, FirstOnEmptyPoolReturnsNullptr) {
    DataPool pool;
    auto [ptr, type] = pool.first();
    EXPECT_EQ(ptr, nullptr)
        << "first() на пустом пуле должен вернуть nullptr, но вернул ненулевой указатель";
    EXPECT_EQ(type, Types::None)
        << "first() на пустом пуле должен вернуть Types::None";
}

TEST(DataPoolTest, InsertAndRetrieveSingleItem) {
    DataPool pool;
    auto* v = new Vector<int>(1, 1, 1, 1);
    v->arr[0][0][0][0] = 7;
    pool.insert(v, Types::INT_T);
    auto [ptr, type] = pool.first();
    EXPECT_EQ(type, Types::INT_T)
        << "Тип извлечённого элемента должен быть INT_T";
    ASSERT_NE(ptr, nullptr)
        << "Указатель из first() не должен быть nullptr после insert()";
    auto* recovered = static_cast<Vector<int>*>(ptr);
    EXPECT_EQ(recovered->arr[0][0][0][0], 7)
        << "Значение в извлечённом векторе должно быть 7, а равно "
        << recovered->arr[0][0][0][0];
    auto [ptr2, type2] = pool.first();
    EXPECT_EQ(ptr2, nullptr)
        << "После единственного first() пул должен быть пуст — следующий first() должен вернуть nullptr";
    delete recovered;
}

TEST(DataPoolTest, FIFOOrderPreserved) {
    DataPool pool;
    auto* v1 = new Vector<double>(1, 1, 1, 1);
    auto* v2 = new Vector<float>(1, 1, 1, 1);
    v1->arr[0][0][0][0] = 1.1;
    v2->arr[0][0][0][0] = 2.2f;
    pool.insert(v1, Types::DOUBLE);
    pool.insert(v2, Types::FLOAT);
    auto [p1, t1] = pool.first();
    EXPECT_EQ(t1, Types::DOUBLE)
        << "Первым вставлен DOUBLE — он должен выйти первым (FIFO), но вышел другой тип";
    delete static_cast<Vector<double>*>(p1);
    auto [p2, t2] = pool.first();
    EXPECT_EQ(t2, Types::FLOAT)
        << "Вторым вставлен FLOAT — он должен выйти вторым (FIFO), но вышел другой тип";
    delete static_cast<Vector<float>*>(p2);
    auto [p3, t3] = pool.first();
    EXPECT_EQ(p3, nullptr)
        << "После извлечения всех элементов пул должен быть пуст";
}

TEST(DataPoolTest, MultipleInsertionsAndRetrievals) {
    DataPool pool;
    constexpr int N = 5;
    for (int k = 0; k < N; ++k) {
        auto* v = new Vector<int>(1, 1, 1, 1);
        v->arr[0][0][0][0] = k;
        pool.insert(v, Types::INT_T);
    }
    for (int k = 0; k < N; ++k) {
        auto [ptr, type] = pool.first();
        ASSERT_NE(ptr, nullptr)
            << "Элемент #" << k << " должен существовать, но first() вернул nullptr";
        auto* v = static_cast<Vector<int>*>(ptr);
        EXPECT_EQ(v->arr[0][0][0][0], k)
            << "Элемент #" << k << " должен иметь значение " << k
            << ", но имеет " << v->arr[0][0][0][0];
        delete v;
    }
    auto [ptr, type] = pool.first();
    EXPECT_EQ(ptr, nullptr)
        << "После извлечения всех 5 элементов пул должен быть пуст";
}

TEST(DataPoolTest, InsertMixedTypes) {
    DataPool pool;
    auto* vi = new Vector<int64_t>(1, 1, 1, 1);
    auto* vf = new Vector<float>(1, 1, 1, 1);
    pool.insert(vi, Types::INT_64_T);
    pool.insert(vf, Types::FLOAT);
    auto [p1, t1] = pool.first();
    EXPECT_EQ(t1, Types::INT_64_T)
        << "Первый вставленный элемент имел тип INT_64_T, но вышел с другим типом";
    delete static_cast<Vector<int64_t>*>(p1);
    auto [p2, t2] = pool.first();
    EXPECT_EQ(t2, Types::FLOAT)
        << "Второй вставленный элемент имел тип FLOAT, но вышел с другим типом";
    delete static_cast<Vector<float>*>(p2);
}


TEST(IpAddressTest, ConstructorStoresBytes) {
    IpAddress ip(192, 168, 1, 100);
    EXPECT_EQ(ip.b1, 192) << "b1 должен быть 192, а равен " << (int)ip.b1;
    EXPECT_EQ(ip.b2, 168) << "b2 должен быть 168, а равен " << (int)ip.b2;
    EXPECT_EQ(ip.b3,   1) << "b3 должен быть 1, а равен "   << (int)ip.b3;
    EXPECT_EQ(ip.b4, 100) << "b4 должен быть 100, а равен " << (int)ip.b4;
}

TEST(IpAddressTest, LoopbackAddress) {
    IpAddress ip(127, 0, 0, 1);
    EXPECT_EQ(ip.b1, 127) << "b1 loopback-адреса должен быть 127, а равен " << (int)ip.b1;
    EXPECT_EQ(ip.b2,   0) << "b2 loopback-адреса должен быть 0, а равен "   << (int)ip.b2;
    EXPECT_EQ(ip.b3,   0) << "b3 loopback-адреса должен быть 0, а равен "   << (int)ip.b3;
    EXPECT_EQ(ip.b4,   1) << "b4 loopback-адреса должен быть 1, а равен "   << (int)ip.b4;
}

TEST(IpAddressTest, BroadcastAddress) {
    IpAddress ip(255, 255, 255, 255);
    EXPECT_EQ(ip.b1, 255) << "b1 broadcast-адреса должен быть 255, а равен " << (int)ip.b1;
    EXPECT_EQ(ip.b2, 255) << "b2 broadcast-адреса должен быть 255, а равен " << (int)ip.b2;
    EXPECT_EQ(ip.b3, 255) << "b3 broadcast-адреса должен быть 255, а равен " << (int)ip.b3;
    EXPECT_EQ(ip.b4, 255) << "b4 broadcast-адреса должен быть 255, а равен " << (int)ip.b4;
}

TEST(IpAddressTest, ZeroAddress) {
    IpAddress ip(0, 0, 0, 0);
    EXPECT_EQ(ip.b1, 0) << "b1 нулевого адреса должен быть 0, а равен " << (int)ip.b1;
    EXPECT_EQ(ip.b4, 0) << "b4 нулевого адреса должен быть 0, а равен " << (int)ip.b4;
}


TEST(PoolItemTest, ConstructorSetsFields) {
    int dummy = 42;
    PoolItem item(&dummy, Types::INT_T);
    EXPECT_EQ(item.vector, &dummy)
        << "PoolItem должен хранить переданный указатель, но хранит другой";
    EXPECT_EQ(item.type, Types::INT_T)
        << "PoolItem должен хранить тип INT_T";
    EXPECT_EQ(item.next, nullptr)
        << "next нового PoolItem должен быть nullptr";
}

TEST(PoolItemTest, NextPointerCanBeLinked) {
    int a = 1, b = 2;
    PoolItem item1(&a, Types::INT_T);
    PoolItem item2(&b, Types::DOUBLE);
    item1.next = &item2;
    EXPECT_EQ(item1.next, &item2)
        << "item1.next должен указывать на item2";
    EXPECT_EQ(item1.next->vector, &b)
        << "Через item1.next->vector должен быть доступен адрес переменной b";
}


int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::off);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}