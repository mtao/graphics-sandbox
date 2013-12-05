cmake_minimum_required(VERSION 2.6)
PROJECT(gradient)

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
INCLUDE_DIRECTORIES(/opt/intel/tbb/include)


# Source files
SET(gradient_SOURCES
    src/main.cpp
    )
# Include files
SET(gradient_HEADERS
    )

# Executable to build
ADD_EXECUTABLE(gradient
    ${gradient_SOURCES}
    ${gradient_HEADERS})

TARGET_LINK_LIBRARIES(gradient openvdb tbb Half)
