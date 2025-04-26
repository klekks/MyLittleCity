cmake_minimum_required(VERSION "3.22")

project(MyLittleCity)

set( 
    drawable_object_sources
    roads/RoadView.cpp 
    Renderer.cpp
)

add_library(drawable_objects STATIC ${drawable_object_sources})
target_link_libraries(drawable_objects sfml::sfml game_objects simulation)
