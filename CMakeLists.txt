cmake_minimum_required(VERSION 3.27)

project(not-doom VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DEBUG_POSTFIX d)

add_executable(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/release
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
)

set(SOURCES
    src/Game.hpp                    src/Game.cpp
    src/Doom.hpp                    src/Doom.cpp
    src/EntityManager.hpp           src/EntityManager.cpp
    src/ComponentManager.hpp
    src/ComponentVector.hpp
    src/PackedArray.hpp
    src/PhysicsComponent.hpp
    src/RenderComponent.hpp
    src/TransformComponent.hpp
    src/InputManager.hpp            src/InputManager.cpp
    src/ResourceManager.hpp         src/ResourceManager.cpp
    src/Vector2.hpp
    src/Vector3.hpp
    src/Quaternion.hpp
    src/LoggingManager.hpp          src/LoggingManager.cpp
)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/main.cpp
        ${SOURCES}
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            "-Werror"
            "-Wall"
            "-Wextra"
            "-Wpedantic")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            "-Werror"
            "-Wall"
            "-Wextra"
            "-Wpedantic"
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MCSVC")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            "/Wall"
            "/WX"
            #https://github.com/friendlyanon/cmake-init-executable/blob/master/CMakePresets.json#L78
            "/sdl"
            "/utf-8"
            "/diagnostics:caret"
            "/permissive-"
            "/volatile:iso"
            "/Zc:inline"
            "/Zc:enumTypes"
            "/Zc:__cplusplus"
            "/Zc:externConstexpr"
            "/Zc:throwingNew"
            "/EHsc"
    )
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} SYSTEM)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL2::SDL2
        SDL2_image::SDL2_image
)

find_package(GTest REQUIRED)
enable_testing()

set(TEST_EXECUTABLE_NAME ${PROJECT_NAME}_test)
add_executable(${TEST_EXECUTABLE_NAME})
set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)

target_sources(${TEST_EXECUTABLE_NAME}
    PRIVATE
        ${SOURCES}
        tests/ComponentManager.test.cpp
        tests/EntityManager.test.cpp
        tests/Quaternion.test.cpp
)

target_include_directories(${TEST_EXECUTABLE_NAME}
    PRIVATE
        src
)

target_link_libraries(${TEST_EXECUTABLE_NAME}
    PRIVATE
        SDL2::SDL2
        SDL2_image::SDL2_image
        GTest::gtest_main
        GTest::gmock_main
)

gtest_discover_tests(${TEST_EXECUTABLE_NAME})
