cmake_minimum_required(VERSION "3.22")

project(MyLittleCity)


set(controller_SRC
    controller/InputController.cpp
)

add_executable(MyLittleCity main.cpp ${controller_SRC})
target_link_libraries(MyLittleCity sfml-graphics sfml-window sfml-system collider)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:${PROJECT_NAME}>" "${PROJECT_BINARY_DIR}/../distr/")

add_subdirectory(core)
