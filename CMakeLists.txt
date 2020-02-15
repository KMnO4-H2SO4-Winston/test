cmake_minimum_required(VERSION 3.13)
project(SolveEquations)

set(CMAKE_CXX_STANDARD 11)

add_executable(SolveEquations main.cpp equation.cpp equation.h expression.cpp expression.h term.cpp term.h error.h)