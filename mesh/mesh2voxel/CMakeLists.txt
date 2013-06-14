cmake_minimum_required(VERSION 2.6)
PROJECT(Mesh2Voxel)

SET(CMAKE_RUNTIME_OUTPUT_DIR ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)

ADD_DEFINITIONS(-std=c++11)
ADD_DEFINITIONS(-Werror)
ADD_DEFINITIONS(-Wall)

# Include dirs
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
# Autogen'd moc code appears in the BINARY_DIR I believe
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# Source files
SET(Mesh2Voxel_SOURCES
    src/main.cpp
    src/trianglemesh.cpp
    src/triangle.cpp
    src/lattice.cpp
    src/bucket.cpp
    src/rayiterator.cpp
    )
# Include files
SET(Mesh2Voxel_HEADERS
    include/triangle.h
    include/trianglemesh.h
    include/lattice.h
    include/rayiterator.h
    include/bucket.h
    )


# Executable to build
ADD_EXECUTABLE(Mesh2Voxel
    ${Mesh2Voxel_SOURCES}
    ${Mesh2Voxel_HEADERS})

