cmake_minimum_required(VERSION "3.22")

project(MyLittileCity)

set(
    roads_source_files
    Intersection.cpp
    Lane.cpp 
    Point.cpp 
    Road.cpp 
)

add_library(core_roads_logic STATIC ${roads_source_files})


