cmake_minimum_required(VERSION "3.22")

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)


project(MyLittileCity)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_subdirectory(src)
