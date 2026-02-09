cmake_minimum_required(VERSION 3.10)
project(learn_simd VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add SIMD/optimization flags conditionally
if(MSVC)
    message(STATUS "MSVC detected.")
    # Only set SIMD and /O2 in Release mode
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /arch:AVX2")
else()  # GCC/Clang
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -mavx2 -march=native")
endif()

# Source files
set(SOURCES
    main.cpp
    fix_parser.cpp
    line_parser.cpp
    ord_parser.cpp
)

add_executable(learn_simd ${SOURCES})
target_include_directories(learn_simd PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})