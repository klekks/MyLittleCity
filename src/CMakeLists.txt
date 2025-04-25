cmake_minimum_required(VERSION "3.22")

project(MyLittileCity)

add_subdirectory(core)
add_subdirectory(driver)
add_subdirectory(controller)


add_executable(${PROJECT_NAME} main.cpp )
target_link_libraries(${PROJECT_NAME} core_roads_logic simulation controller)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:${PROJECT_NAME}>" "${PROJECT_BINARY_DIR}/../distr/")
