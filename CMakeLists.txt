CMAKE_MINIMUM_REQUIRED( VERSION 3.7.1 )
PROJECT( GraphicsSandbox )




INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(SYSTEM /usr/include/eigen3)
ADD_DEFINITIONS(-std=c++1z)
ADD_DEFINITIONS(-Wall)
ADD_DEFINITIONS(-g)



ADD_SUBDIRECTORY(geometry)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/geometry/include)
ADD_SUBDIRECTORY(tools)
