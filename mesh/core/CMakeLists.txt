CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
INCLUDE(../../CMakeDefaults.txt)
PROJECT(MeshCore)

INCLUDE_DIRECTORIES(../../core/include)


SET(MeshCore_SOURCES
    src/halfedge.cpp
    src/hetrianglemesh.cpp
    )
# Include files
SET(MeshCore_HEADERS
    include/vftrianglemesh.h
    include/vftrianglemesh.ipl
    include/veftrianglemesh.h
    include/veftrianglemesh.ipl
    include/hetrianglemesh.h
    include/mesh_reader.h
    include/mesh_reader.ipl
    include/meshconstructor.h
    )

ENABLE_TESTING()
ADD_EXECUTABLE(compile_test src/compile.cpp ${MeshCore_SOURCES}
    ${MeshCore_HEADERS})
ADD_TEST(compilation compile_test)
