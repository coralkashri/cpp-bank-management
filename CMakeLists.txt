cmake_minimum_required(VERSION 3.17)
project(BankManagement)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

find_package(Boost CONFIG)
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)

include_directories("/usr/local/include/mongocxx/v_noabi/")
include_directories("/usr/local/include/bsoncxx/v_noabi/")

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*")
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} mongo::bsoncxx_shared mongo::mongocxx_shared)