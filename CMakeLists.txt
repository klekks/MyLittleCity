cmake_minimum_required(VERSION "3.22")

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(MyLittleCity)

find_package(SFML REQUIRED)

include_directories(${SFML_INCLUDE_DIRS})

add_subdirectory(src)
