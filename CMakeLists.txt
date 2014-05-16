CMAKE_MINIMUM_REQUIRED( VERSION 2.8.9 )
PROJECT( GraphicsSandbox )






SET(CMAKE_RUNTIME_OUTPUT_DIR ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include /usr/local/include/eigen3)

ADD_DEFINITIONS(-std=c++11)
ADD_DEFINITIONS(-Wall)
ADD_DEFINITIONS(-Werror)
ADD_DEFINITIONS(-g)
ADD_DEFINITIONS(-Wno-unused-local-typedefs)

SET(INCLUDE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX})

FILE(GLOB HEADERS 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*/*.h" 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*/*.hxx")
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
FIND_PACKAGE(Qt5Widgets)
FIND_PACKAGE(Qt5OpenGL)



SET(core_SOURCES
    src/linereader.cpp
    )
SET(core_HEADERS 
    include/coord.h
    include/eigen_hash.h  
    include/eigen_ordering.h
    include/kdop.h
    include/linereader.hpp
    include/types.h
    )

SET(mesh_SOURCES
    src/mesh/meshexceptions.cpp

    src/mesh/halfedge.cpp
    src/mesh/loaders/objloader.cpp

    src/mesh/hetrianglemesh.cpp
    src/mesh/hetriangletopology.cpp

    src/mesh/vftrianglemesh.cpp
    src/mesh/vftriangletopology.cpp
    )

SET(mesh_HEADERS
    include/mesh/meshbase.h
    include/mesh/meshtraits.h
    include/mesh/meshexceptions.h
    include/mesh/meshtopologybase.h
    include/mesh/meshconstructor.h
    include/mesh/meshconstructor.ipl
    include/mesh/meshtopologyconstructor.h
    include/mesh/meshtopologyconstructor.ipl
    include/mesh/halfedge.h
    include/mesh/loader.h
    include/mesh/loaders/objloader.h

    include/mesh/hetrianglemesh.h
    include/mesh/hetrianglemesh.ipl
    include/mesh/hetriangletopology.h
    
    include/mesh/vftrianglemesh.h
    include/mesh/vftrianglemesh.ipl
    include/mesh/vftriangletopology.h
    )



SET(ACTIVE_MODULES core mesh)

FOREACH(MOD ${ACTIVE_MODULES})
    STRING(REPLACE "include/" "./" MOD_HEADERS ${${MOD}_HEADERS})
   
    INSTALL(FILES ${${MOD}_HEADERS} DESTINATION
        include/mtao/${MOD_HEADERS})
    SET(TOTAL_LIB_SOURCES ${TOTAL_LIB_SOURCES} ${${MOD}_SOURCES})
    SET(TOTAL_LIB_HEADERS ${TOTAL_LIB_HEADERS} ${${MOD}_HEADERS})
    SET(TOTAL_LIB_RESOURCES ${TOTAL_LIB_RESOURCES} ${${MOD}_RESOURCES})
ENDFOREACH(MOD ${ACTIVE_MODULES})





QT5_ADD_RESOURCES(TOTAL_LIB_RESOURCE_SOURCES ${TOTAL_LIB_RESOURCES})

ADD_LIBRARY(mtaoGraphics ${TOTAL_LIB_SOURCES} ${TOTAL_LIB_HEADERS}
    ${TOTAL_LIB_RESOURCE_SOURCES})

INSTALL( TARGETS mtaoGraphics LIBRARY DESTINATION lib ARCHIVE DESTINATION
    lib/static )


ADD_SUBDIRECTORY(tools)
ADD_SUBDIRECTORY(tests EXCLUDE_FROM_ALL)




