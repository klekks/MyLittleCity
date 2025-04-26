cmake_minimum_required(VERSION "3.22")

project(MyLittleCity)

add_subdirectory(core)
add_subdirectory(driver)
add_subdirectory(controller)
add_subdirectory(graphics)


add_executable(${PROJECT_NAME} main.cpp )
target_link_libraries(${PROJECT_NAME} simulation )


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:${PROJECT_NAME}>" "${PROJECT_BINARY_DIR}/../distr/")

# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     "${PROJECT_SOURCE_DIR}/../artifacts/ArialRegular.ttf" "${PROJECT_BINARY_DIR}/../distr/")