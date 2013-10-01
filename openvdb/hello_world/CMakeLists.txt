cmake_minimum_required(VERSION 2.6)
PROJECT(hello_world)

SET(CMAKE_RUNTIME_OUTPUT_DIR ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)

ADD_DEFINITIONS(-std=c++11)
ADD_DEFINITIONS(-Werror)
ADD_DEFINITIONS(-Wall)

# Include dirs
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include /usr/local/include/eigen3)
# Autogen'd moc code appears in the BINARY_DIR I believe
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})


# Source files
SET(hello_world_SOURCES
    src/main.cpp
    )
# Include files
SET(hello_world_HEADERS
    )

# Executable to build
ADD_EXECUTABLE(hello_world
    ${hello_world_SOURCES}
    ${hello_world_HEADERS})

TARGET_LINK_LIBRARIES(hello_world openvdb tbb Half)
