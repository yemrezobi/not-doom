cmake_minimum_required(VERSION 3.22.1)

project(not-doom VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        src/main.cpp
        src/Game.hpp                    src/Game.cpp
        src/GraphicsComponent.hpp       src/GraphicsComponent.cpp
        src/PhysicsComponent.hpp        src/PhysicsComponent.cpp
        src/Vec2.hpp                    src/Vec2.cpp
        src/Vec3.hpp                    src/Vec3.cpp
        src/Player.hpp                  src/Player.cpp
        src/Camera.hpp                  src/Camera.cpp
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

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${SDL2_LIBRARIES}
        SDL2_image::SDL2_image
)

enable_testing()
set(TEST_NAME ${PROJECT_NAME}_test)
add_executable(${TEST_NAME})
target_sources(${TEST_NAME}
    PRIVATE
        tests/Game.test.cpp
        tests/GraphicsComponent.test.cpp
        tests/GraphicsComponent.test.cpp
        tests/Vec2.test.cpp
        tests/Vec3.test.cpp
        tests/Player.test.cpp
        tests/Camera.test.cpp
)
target_include_directories(${TEST_NAME}
    PRIVATE
        src
)
target_link_libraries(${TEST_NAME}
    PRIVATE
        GTest::gtest_main
)
SET_TARGET_PROPERTIES(${TEST_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)
include(GoogleTest)
gtest_discover_tests(${TEST_NAME})
