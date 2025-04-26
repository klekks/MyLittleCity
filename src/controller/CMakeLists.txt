cmake_minimum_required(VERSION "3.22")

project(MyLittleCity)

set(
    controller_sources
    commands/MouseCommands.cpp
    EditorController.cpp 
    InputBinding.cpp
    InputHandler.cpp
)

add_library(controller STATIC ${controller_sources})
target_link_libraries(controller sfml::sfml)

