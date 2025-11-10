cmake_minimum_required(VERSION 3.28)
project(GoCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(GoCompiler main.cpp parser.cpp scanner.cpp token.cpp ast.cpp visitor.cpp)
