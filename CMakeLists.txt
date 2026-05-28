cmake_minimum_required(VERSION 3.16)
project(PGOF LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(pgof STATIC src/pgof.cpp)
target_include_directories(pgof PUBLIC include)

add_executable(pgof_tests tests/pgof_tests.cpp)
target_link_libraries(pgof_tests PRIVATE pgof)

enable_testing()
add_test(NAME pgof_tests COMMAND pgof_tests)
