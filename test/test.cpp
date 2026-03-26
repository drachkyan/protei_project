#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <variant>
#include <vector>
#include <optional>
#include <string>
#include <filesystem>
#include <fstream>

#include "../myvector/include/MyVector.hpp"
#include "../myvector/include/VectorFactory.h"
#include "../client/include/Network/NetworkAddress.h"
#include "../client/include/Test/ResourceTest.h"
#include "../client/include/utils/utils.h"

using json = nlohmann::json;


TEST(VectorFactory, KnownTypesReturnValue) {
    for (const auto& t : {"int","float","double","char",
                           "uint64_t","uint16_t","uint32_t",
                           "int64_t","int16_t"}) {
        auto v = VectorFactory(4, t);
        EXPECT_TRUE(v.has_value()) << "type=" << t;
    }
}

TEST(VectorFactory, UnknownTypeReturnsNullopt) {
    auto v = VectorFactory(4, "bool");
    EXPECT_FALSE(v.has_value());
}

TEST(VectorFactory, CorrectSizeCreated) {
    auto v = VectorFactory(7, "int");
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(std::get<std::vector<int32_t>>(*v).size(), 7u);
}

TEST(VectorFactory, ZeroSize) {
    auto v = VectorFactory(0, "double");
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(std::get<std::vector<double>>(*v).size(), 0u);
}


TEST(MyVector, ValidTypeCreated) {
    MyVector v("int", 5);
    EXPECT_TRUE(v.getData().has_value());
    EXPECT_EQ(v.getTypeName(), "int");
}

TEST(MyVector, InvalidTypeNullopt) {
    MyVector v("bool", 5);
    EXPECT_FALSE(v.getData().has_value());
}

TEST(MyVector, ZeroSizeOk) {
    MyVector v("float", 0);
    EXPECT_TRUE(v.getData().has_value());
    auto& vec = std::get<std::vector<float>>(*v.getData());
    EXPECT_EQ(vec.size(), 0u);
}

TEST(MyVector, PushIntToIntVector) {
    MyVector v("int", 3);
    ASSERT_TRUE(v.getData().has_value());
    EXPECT_TRUE(v.push(42, 0));
    auto& vec = std::get<std::vector<int32_t>>(*v.getData());
    EXPECT_EQ(vec[0], 42);
}

TEST(MyVector, PushDoubleToDoubleVector) {
    MyVector v("double", 2);
    ASSERT_TRUE(v.getData().has_value());
    EXPECT_TRUE(v.push(3.14, 1));
    auto& vec = std::get<std::vector<double>>(*v.getData());
    EXPECT_DOUBLE_EQ(vec[1], 3.14);
}

TEST(MyVector, PushFloatToIntVectorFails) {
    MyVector v("int", 3);
    ASSERT_TRUE(v.getData().has_value());
    bool ok = v.push(1.5, 0);
    EXPECT_FALSE(ok);
}

TEST(MyVector, PushIntToFloatVectorOk) {
    MyVector v("float", 2);
    ASSERT_TRUE(v.getData().has_value());
    EXPECT_TRUE(v.push(5, 0));
}


TEST(MyVector, ToJsonRoundTrip_Int) {
    MyVector v("int", 3);
    v.push(1, 0); v.push(2, 1); v.push(3, 2);

    json j;
    v.toJson(j);

    EXPECT_EQ(j["size"], 3);
    EXPECT_EQ(j["type"], "int");
    EXPECT_EQ(j["data"][0], 1);
    EXPECT_EQ(j["data"][2], 3);
}

TEST(MyVector, FromJsonConstructor_Int) {
    json j;
    j["size"] = 3;
    j["type"] = "int";
    j["data"] = {10, 20, 30};

    MyVector v(j);
    ASSERT_TRUE(v.getData().has_value());
    auto& vec = std::get<std::vector<int32_t>>(*v.getData());
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);
}

TEST(MyVector, FromJsonConstructor_Double) {
    json j;
    j["size"] = 2;
    j["type"] = "double";
    j["data"] = {1.5, 2.5};

    MyVector v(j);
    ASSERT_TRUE(v.getData().has_value());
    auto& vec = std::get<std::vector<double>>(*v.getData());
    EXPECT_DOUBLE_EQ(vec[0], 1.5);
    EXPECT_DOUBLE_EQ(vec[1], 2.5);
}

TEST(MyVector, FromJsonBadType) {
    json j;
    j["size"] = 2;
    j["type"] = "bool";
    j["data"] = {1, 0};

    MyVector v(j);
    EXPECT_FALSE(v.getData().has_value());
}

TEST(MyVector, ToJsonEmptyVector) {
    MyVector v("int", 0);
    json j;
    v.toJson(j);
    EXPECT_EQ(j["size"], 0);
    EXPECT_TRUE(j["data"].empty());
}


TEST(MyVector, MultiplicationInt) {
    MyVector a("int", 3), b("int", 3);
    a.push(2,0); a.push(3,1); a.push(4,2);
    b.push(5,0); b.push(6,1); b.push(7,2);

    MyVector& res = a * b;
    auto& vec = std::get<std::vector<int32_t>>(*res.getData());
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 18);
    EXPECT_EQ(vec[2], 28);
}

TEST(MyVector, MultiplicationDouble) {
    MyVector a("double", 2), b("double", 2);
    a.push(1.5, 0); a.push(2.0, 1);
    b.push(2.0, 0); b.push(3.0, 1);

    a * b;
    auto& vec = std::get<std::vector<double>>(*a.getData());
    EXPECT_DOUBLE_EQ(vec[0], 3.0);
    EXPECT_DOUBLE_EQ(vec[1], 6.0);
}

TEST(MyVector, MultiplicationSizeMismatchNoChange) {
    MyVector a("int", 2), b("int", 3);
    a.push(2,0); a.push(3,1);
    b.push(5,0); b.push(6,1); b.push(7,2);

    a * b;
    auto& vec = std::get<std::vector<int32_t>>(*a.getData());
    EXPECT_EQ(vec[0], 2); // без изменений
    EXPECT_EQ(vec[1], 3);
}

TEST(MyVector, SquareViaMultiplication) {
    MyVector v("int", 3);
    v.push(3,0); v.push(4,1); v.push(5,2);
    v * v;
    auto& vec = std::get<std::vector<int32_t>>(*v.getData());
    EXPECT_EQ(vec[0], 9);
    EXPECT_EQ(vec[1], 16);
    EXPECT_EQ(vec[2], 25);
}

TEST(NetworkAddress, ValidAddress) {
    NetworkAddress na("8080", "127.0.0.1");
    EXPECT_TRUE(na.isCorrect());
    EXPECT_EQ(na.getPort(), 8080);
    EXPECT_EQ(na.getIpAddress(), "127.0.0.1");
}

TEST(NetworkAddress, InvalidPort_Zero) {
    NetworkAddress na("0", "127.0.0.1");
    EXPECT_FALSE(na.isCorrect());
}

TEST(NetworkAddress, InvalidPort_TooLarge) {
    NetworkAddress na("99999", "127.0.0.1");
    EXPECT_FALSE(na.isCorrect());
}

TEST(NetworkAddress, InvalidPort_NonNumeric) {
    NetworkAddress na("abc", "127.0.0.1");
    EXPECT_FALSE(na.isCorrect());
}

TEST(NetworkAddress, InvalidIp_TooFewOctets) {
    NetworkAddress na("8080", "127.0.1");
    EXPECT_FALSE(na.isCorrect());
}

TEST(NetworkAddress, InvalidIp_OctetOutOfRange) {
    NetworkAddress na("8080", "256.0.0.1");
    EXPECT_FALSE(na.isCorrect());
}

TEST(NetworkAddress, BoundaryPort_1) {
    NetworkAddress na("1", "192.168.1.1");
    EXPECT_TRUE(na.isCorrect());
    EXPECT_EQ(na.getPort(), 1);
}

TEST(NetworkAddress, BoundaryPort_65535) {
    NetworkAddress na("65535", "192.168.1.1");
    EXPECT_TRUE(na.isCorrect());
    EXPECT_EQ(na.getPort(), 65535);
}

TEST(Utils, HashSameStrings) {
    EXPECT_EQ(hash("hello"), hash("hello"));
}

TEST(Utils, HashDifferentStrings) {
    EXPECT_NE(hash("hello"), hash("world"));
}

TEST(Utils, HashEmptyString) {
    EXPECT_EQ(hash(""), 0u);
}

TEST(Utils, HashHelp) {
    EXPECT_EQ(hash("--help"), hash("--help"));
}

TEST(Utils, ToLowerCase) {
    std::string s = "INT";
    toLowerCase(s);
    EXPECT_EQ(s, "int");
}

TEST(Utils, ToLowerCaseMixed) {
    std::string s = "Float";
    toLowerCase(s);
    EXPECT_EQ(s, "float");
}

TEST(Utils, ToLowerCaseAlreadyLower) {
    std::string s = "double";
    toLowerCase(s);
    EXPECT_EQ(s, "double");
}


TEST(ResourceTest, ExistingFile) {
    const std::string tmp = "/tmp/gtest_resource_test_file.txt";
    { std::ofstream f(tmp); f << "ok"; }
    ResourceTest rt({tmp});
    EXPECT_EQ(rt.test(), 0);
    std::filesystem::remove(tmp);
}

TEST(ResourceTest, MissingFile) {
    ResourceTest rt({"/tmp/this_file_does_not_exist_12345.txt"});
    EXPECT_EQ(rt.test(), 1);
}

TEST(ResourceTest, EmptyFileList) {
    ResourceTest rt({});
    EXPECT_EQ(rt.test(), 0);
}

TEST(ResourceTest, MixedFiles) {
    const std::string tmp = "/tmp/gtest_mixed_file.txt";
    { std::ofstream f(tmp); f << "ok"; }
    ResourceTest rt({tmp, "/tmp/missing_file_xyz.txt"});
    EXPECT_EQ(rt.test(), 1);
    std::filesystem::remove(tmp);
}


TEST(ServerLogic, SquareVectorViaJson) {
    MyVector input("int", 4);
    input.push(1,0); input.push(2,1); input.push(3,2); input.push(4,3);

    json req;
    input.toJson(req);
    MyVector parsed(req);
    ASSERT_TRUE(parsed.getData().has_value());
    parsed * parsed;

    json res;
    parsed.toJson(res);

    EXPECT_EQ(res["data"][0], 1);
    EXPECT_EQ(res["data"][1], 4);
    EXPECT_EQ(res["data"][2], 9);
    EXPECT_EQ(res["data"][3], 16);
}

TEST(ServerLogic, SquareFloatVectorViaJson) {
    MyVector input("float", 3);
    input.push(2.0f, 0); input.push(3.0f, 1); input.push(4.0f, 2);

    json req;
    input.toJson(req);

    MyVector parsed(req);
    ASSERT_TRUE(parsed.getData().has_value());
    parsed * parsed;

    json res;
    parsed.toJson(res);

    EXPECT_FLOAT_EQ(res["data"][0].get<float>(), 4.0f);
    EXPECT_FLOAT_EQ(res["data"][1].get<float>(), 9.0f);
    EXPECT_FLOAT_EQ(res["data"][2].get<float>(), 16.0f);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
