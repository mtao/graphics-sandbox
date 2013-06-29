CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
INCLUDE(../../CMakeDefaults.txt)
PROJECT(MeshCore)



# Include files
SET(Mesh2Voxel_HEADERS
    include/eigen_ordering.h
    include/vftrianglemesh.h
    include/vftrianglemesh.ipl
    include/veftrianglemesh.h
    include/veftrianglemesh.ipl
    )

ENABLE_TESTING()
ADD_EXECUTABLE(compile_test src/compile.cpp)
ADD_TEST(compilation compile_test)
