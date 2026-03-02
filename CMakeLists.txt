cmake_minimum_required(VERSION 3.14)

include(FetchContent)

project(myApp)

set(CMAKE_BUILD_TYPE Debug)

FetchContent_Declare(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.16.0)

FetchContent_MakeAvailable(spdlog)

set(SOURCE_LIST
        src/main.cpp
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
)

add_executable(${PROJECT_NAME} ${SOURCE_LIST})

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)