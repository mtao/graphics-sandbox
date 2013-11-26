cmake_minimum_required(VERSION 2.8.9)
PROJECT(TrellisGenerator)

SET(CMAKE_RUNTIME_OUTPUT_DIR ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)

ADD_DEFINITIONS(-std=c++11)
ADD_DEFINITIONS(-Werror)
ADD_DEFINITIONS(-Wall)

# Include dirs
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)


# Source files
SET(trillis_generator_SOURCES
    src/main.cpp
    src/trellis.cpp
    src/trellis_generator.cpp
    )

SET(trellis_generator_HEADERS
    include/trellis.hpp
    include/trellis_generator.hpp
    )

# Executable to build
ADD_EXECUTABLE(trillis_generator
    ${trillis_generator_SOURCES}
    ${trillis_generator_HEADERS}
    )

TARGET_LINK_LIBRARIES(trillis_generator openvdb tbb Half)
