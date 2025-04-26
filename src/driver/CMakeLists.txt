cmake_minimum_required(VERSION "3.22")

project(MyLittleCity)

set(
    simulation_sources
    Simulation.cpp
    World.cpp
)

add_library(simulation STATIC ${simulation_sources} )
target_link_libraries(simulation sfml::sfml game_objects drawable_objects controller)

