cmake_minimum_required(VERSION 3.10)

project(myApp)

set(SOURCE_LIST
        src/main.cpp
        include/utils.h
        src/utils.cpp
        include/interfaces.h)


add_executable(${PROJECT_NAME} ${SOURCE_LIST})