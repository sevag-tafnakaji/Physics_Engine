cmake_minimum_required(VERSION 3.28)
project(physics_engine LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE sources
    src/*.cpp src/*.c include/*.h include/*.hpp utils/*.hpp utils/*.h)

add_executable(${PROJECT_NAME} ${sources})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wfloat-conversion)
target_include_directories(${PROJECT_NAME} PUBLIC include utils)
