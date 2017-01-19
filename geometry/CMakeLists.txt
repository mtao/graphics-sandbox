CMAKE_MINIMUM_REQUIRED( VERSION 3.7.1 )
PROJECT( Geometry )




INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})


SET(GEO_HDRS
    include/util/matrix_iterator.h
    include/mesh/trianglemesh.h
    include/mesh/simplexmesh.h
    )

SET(GEO_SRCS
    src/mesh/trianglemesh.cpp
    )


ADD_LIBRARY(geometry ${GEO_HDRS} ${GEO_SRCS})

