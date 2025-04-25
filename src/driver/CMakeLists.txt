cmake_minimum_required(VERSION "3.22")

project(MyLittileCity)

set(
    simulation_sources
    Simulation.cpp
)

add_library(simulation STATIC ${simulation_sources})


