cmake_minimum_required(VERSION 3.17)
project(BankManagement)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*")

add_executable(BankManagement ${SOURCES})