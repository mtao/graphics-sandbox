CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
INCLUDE(../../CMakeDefaults.txt)
PROJECT(MeshCore)



SET(MeshCore_SOURCES
    src/halfedge.cpp
    src/hetrianglemesh.cpp
    )
# Include files
SET(MeshCore_HEADERS
    include/eigen_ordering.h
    include/vftrianglemesh.h
    include/vftrianglemesh.ipl
    include/veftrianglemesh.h
    include/veftrianglemesh.ipl
    include/hetrianglemesh.h
    )

ENABLE_TESTING()
ADD_EXECUTABLE(compile_test src/compile.cpp ${MeshCore_SOURCES})
ADD_TEST(compilation compile_test)
