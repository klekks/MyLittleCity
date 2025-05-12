cmake_minimum_required(VERSION "3.22")

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MyLittleCity)

find_package(SFML REQUIRED)

include_directories(${SFML_INCLUDE_DIRS})

add_subdirectory(src)
