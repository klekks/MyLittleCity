cmake_minimum_required(VERSION "3.22")

project(MyLittileCity)

set(
    controller_sources
    MouseController.cpp
    WindowController.cpp 
    DelegateController.cpp
)

add_library(controller STATIC ${controller_sources})

