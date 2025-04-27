cmake_minimum_required(VERSION "3.22")

project(MyLittleCity)

add_subdirectory(roads)


add_library(game_objects STATIC GameObject.cpp OrientedBoundingBox.cpp)
target_link_libraries(game_objects core_roads_logic)