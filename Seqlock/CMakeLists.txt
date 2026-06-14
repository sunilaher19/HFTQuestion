cmake_minimum_required(VERSION 3.15)
# Dynamically names the project based on the folder name ("OrderBook")
get_filename_component(PROJECT_NAME_STR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME_STR} LANGUAGES CXX)

# Enforce GCC 15 and C++23 standard
set(CMAKE_CXX_COMPILER g++-15)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Gather source files from the local src/ directory
file(GLOB SOURCES "src/*.cpp")

# Define the executable using the folder name
add_executable(${PROJECT_NAME_STR} ${SOURCES})

# Target the local inc/ directory for header files
target_include_directories(${PROJECT_NAME_STR} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
