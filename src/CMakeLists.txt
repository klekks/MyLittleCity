cmake_minimum_required(VERSION "3.26")

project(MyLittileCity)

# set(
#     path_finder_sources
#     Path/Coordinates.cpp
#     Path/Pathfinder.cpp
# )

# set(
#     city_sources
#     City/CityMap.cpp
# )

# set(
#     simulator_sources
#     Simulator/Simulator.cpp
# )

# set(
#     transport_sources
#     Transport/Car.cpp
# )

# add_executable(${PROJECT_NAME} main.cpp ${path_finder_sources} ${city_sources} ${simulator_sources} ${transport_sources})

# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     "$<TARGET_FILE:${PROJECT_NAME}>" "${PROJECT_BINARY_DIR}/../distr/")
