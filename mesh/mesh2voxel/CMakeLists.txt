INCLUDE(../../CMakeDefaults.txt)
PROJECT(Mesh2Voxel)


# Include dirs


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
    include/eigen_boost_serialization.hpp
    include/serialization.hpp
    )

FIND_PACKAGE(Boost 1.50.0 COMPONENTS serialization REQUIRED)
# Executable to build
ADD_EXECUTABLE(Mesh2Voxel
    ${Mesh2Voxel_SOURCES}
    ${Mesh2Voxel_HEADERS})

TARGET_LINK_LIBRARIES(Mesh2Voxel ${Boost_LIBRARIES})
