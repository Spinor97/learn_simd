cmake_minimum_required(VERSION 3.10)

project(learn_simd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Safe SIMD compile options for VMs or older CPUs
if(NOT MSVC)
    add_compile_options(-O3 -msse2)
endif()

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect all .cpp files in src/
file(GLOB SRC_FILES CONFIGURE_DEPENDS src/*.cpp)

# Build the executable
add_executable(learn_simd ${SRC_FILES})