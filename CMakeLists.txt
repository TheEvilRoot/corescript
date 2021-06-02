cmake_minimum_required(VERSION 3.10)

project(corescript CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
    main.cpp
    token.h
    tokens.h
    expression.h
    expressions.h
)
