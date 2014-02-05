CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(MeshCore)

SET(CMAKE_RUNTIME_OUTPUT_DIR ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)

# Include dirs
INCLUDE_DIRECTORIES(../../core/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include /usr/local/include/eigen3)

ADD_DEFINITIONS(-std=c++11)
ADD_DEFINITIONS(-Wall)
ADD_DEFINITIONS(-Werror)

SET(MeshCore_SOURCES
    src/halfedge.cpp
    src/hetrianglemesh.cpp
    src/hetriangletopology.cpp
    )
# Include files
SET(MeshCore_HEADERS
    include/vftrianglemesh.h
    include/vftrianglemesh.ipl
    include/veftrianglemesh.h
    include/veftrianglemesh.ipl
    include/hetrianglemesh.h
    include/hetriangletopology.h
    #include/mesh_reader.h
    #include/mesh_reader.ipl
    include/meshconstructor.h
    )

ENABLE_TESTING()
ADD_EXECUTABLE(compile_test src/compile.cpp ${MeshCore_SOURCES}
    ${MeshCore_HEADERS})
ADD_TEST(compilation compile_test)
ADD_EXECUTABLE(halfedge_test src/hetriangletests.cpp ${MeshCore_SOURCES}
    ${MeshCore_HEADERS})
ADD_TEST(halfedge halfedge_test)
