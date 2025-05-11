cmake_minimum_required(VERSION 3.10)
project(physics-engine)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp")

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/raylib)
add_subdirectory(vendor/fmt)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} raylib fmt::fmt)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/vendor/raylib/src
    ${PROJECT_SOURCE_DIR}/vendor/fmt/include
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()