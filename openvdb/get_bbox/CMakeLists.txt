cmake_minimum_required(VERSION 2.6)
PROJECT(vdb_get_bbox)

SET(CMAKE_RUNTIME_OUTPUT_DIR ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)

ADD_DEFINITIONS(-std=c++11)
ADD_DEFINITIONS(-Werror)
ADD_DEFINITIONS(-Wall)

# Include dirs
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include
    /opt/intel/tbb/include/)
LINK_DIRECTORIES( /opt/intel/tbb/lib/intel64/gcc4.4  )


# Source files
SET(vdb_get_bbox_SOURCES
    src/main.cpp
    )

# Executable to build
ADD_EXECUTABLE(vdb_get_bbox
    ${vdb_get_bbox_SOURCES}
    )

TARGET_LINK_LIBRARIES(vdb_get_bbox openvdb tbb Half)
