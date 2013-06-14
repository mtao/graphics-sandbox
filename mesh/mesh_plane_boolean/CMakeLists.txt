cmake_minimum_required(VERSION 2.6)
PROJECT(TrianglePlaneBool)

SET(CMAKE_RUNTIME_OUTPUT_DIR ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)

ADD_DEFINITIONS(-std=c++0x)
ADD_DEFINITIONS(-Werror)

# Include dirs
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
# Autogen'd moc code appears in the BINARY_DIR I believe
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# Source files
SET(TrianglePlaneBool_SOURCES
    src/main.cpp
    src/trianglemesh.cpp
    src/triangle.cpp
    src/plane.cpp
    )
# Include files
SET(TrianglePlaneBool_HEADERS
    include/triangle.h
    include/trianglemesh.h
    include/plane.h
    )


# Executable to build
ADD_EXECUTABLE(TrianglePlaneBool
    ${TrianglePlaneBool_SOURCES}
    ${TrianglePlaneBool_HEADERS})

