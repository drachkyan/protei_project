cmake_minimum_required(VERSION 3.10)

project(myApp)

set(SOURCE_LIST 
    main.cpp
    utils.h
    utils.cpp)


add_executable(${PROJECT_NAME} ${SOURCE_LIST})