cmake_minimum_required(VERSION 3.22.1)

project(not-doom VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        src/main.cpp
        src/Game.hpp src/Game.cpp
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Werror
        -Wall
        -Wextra
        -Wpedantic
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src
)

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/release
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${SDL2_LIBRARIES}
)

