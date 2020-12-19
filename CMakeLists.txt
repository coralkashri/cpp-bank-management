cmake_minimum_required(VERSION 3.17)
project(BankManagement)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

# Find Boost
set(BOOST_REQUIRED_COMPONENTS
        system
        thread
        regex
        coroutine
        context
        filesystem
        date_time
        unit_test_framework
        program_options
        )
find_package(Boost "1.74.0" REQUIRED ${BOOST_REQUIRED_COMPONENTS})

# Find MongoCxx
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)

# Include Boost directories
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# Include MongoCxx directories
include_directories("/usr/local/include/mongocxx/v_noabi/")
include_directories("/usr/local/include/bsoncxx/v_noabi/")

# Set Sources
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*")

# Make executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link Boost
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

# Link Mongo
target_link_libraries(${PROJECT_NAME} mongo::bsoncxx_shared mongo::mongocxx_shared)