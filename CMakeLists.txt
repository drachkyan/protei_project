cmake_minimum_required(VERSION 3.14)

include(FetchContent)

project(myApp)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

set(CMAKE_BUILD_TYPE Debug)

FetchContent_Declare(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.16.0)

FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.16.0)

FetchContent_MakeAvailable(googletest)

set(LIB_SOURCES
        include/interfaces.h
        include/vector.hpp
        src/AppSettings.cpp
        include/AppSettings.h
        src/DataPool.cpp
        include/DataPool.h
        src/Menu.cpp
        include/Menu.h
        include/Test.h
        src/ConnectionTest.cpp
        include/ConnectionTest.h
        src/ResourceTest.cpp
        include/ResourceTest.h
        include/utils.h
        src/utils.cpp
        src/NetworkAddress.cpp
        include/NetworkAddress.h
)



add_executable(${PROJECT_NAME} src/main.cpp ${LIB_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

enable_testing()

add_executable(unit_tests test/test.cpp ${LIB_SOURCES})

target_include_directories(unit_tests PRIVATE ${googletest_SOURCE_DIR}/googletest/include)
target_link_libraries(unit_tests PRIVATE GTest::gtest GTest::gtest_main spdlog::spdlog)

add_test(NAME unit_tests COMMAND unit_tests)