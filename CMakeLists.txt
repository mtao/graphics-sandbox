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
#ADD_DEFINITIONS(-Wno-unused-local-typedefs)

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


SET(core_RENDER_SOURCES
    src/render/scene.cpp
    src/render/internalnode.cpp
    src/render/transformnode.cpp
    src/render/scenenode.cpp
    src/render/qt/shadernode.cpp
    )
SET(core_RENDER_HEADERS
    include/render/scene.h
    include/render/internalnode.h
    include/render/scenenode.h
    include/render/transformnode.h
    include/render/qt/shadernode.h
    )

SET(mesh_SOURCES
    src/mesh/meshexceptions.cpp

    src/mesh/loaders/objloader.cpp
    src/mesh/loaders/offloader.cpp

    src/mesh/halfedge.cpp
    src/mesh/hetrianglemesh.cpp
    src/mesh/hetriangletopology.cpp

    src/mesh/vftrianglemesh.cpp
    src/mesh/vftriangletopology.cpp
    )

SET(mesh_RENDER_SOURCES
    src/render/mesh/meshnode.cpp
    )
SET(mesh_RENDER_HEADERS
    include/render/mesh/meshnode.h
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
    include/mesh/loaders/offloader.h

    include/mesh/hetrianglemesh.h
    include/mesh/hetrianglemesh.ipl
    include/mesh/hetriangletopology.h
    
    include/mesh/vftrianglemesh.h
    include/mesh/vftrianglemesh.ipl
    include/mesh/vftriangletopology.h
    )

SET(openvdb_SOURCES
    )

SET(openvdb_HEADERS
    include/openvdb/vdb_conversion.h
    include/openvdb/vdb_utils.h
    )

SET(sampling_SOURCES
    )

SET(sampling_HEADERS
    include/sampling/poisson_bridson.h
    include/sampling/poisson_bridson.ipl
    )

SET(ACTIVE_MODULES core mesh)

FOREACH(MOD ${ACTIVE_MODULES})
    STRING(REPLACE "include/" "./" MOD_HEADERS ${${MOD}_HEADERS})
   
    INSTALL(FILES ${${MOD}_HEADERS} DESTINATION
        include/mtao/${MOD_HEADERS})
    SET(TOTAL_LIB_SOURCES ${TOTAL_LIB_SOURCES} ${${MOD}_SOURCES})
    SET(TOTAL_LIB_HEADERS ${TOTAL_LIB_HEADERS} ${${MOD}_HEADERS})
    SET(TOTAL_LIB_RESOURCES ${TOTAL_LIB_RESOURCES} ${${MOD}_RESOURCES})

    SET(TOTAL_RENDER_LIB_SOURCES ${TOTAL_RENDER_LIB_SOURCES} ${${MOD}_RENDER_SOURCES})
    SET(TOTAL_RENDER_LIB_HEADERS ${TOTAL_RENDER_LIB_HEADERS} ${${MOD}_RENDER_HEADERS})
    SET(TOTAL_RENDER_LIB_RESOURCES ${TOTAL_RENDER_LIB_RESOURCES} ${${MOD}_RENDER_RESOURCES})

ENDFOREACH(MOD ${ACTIVE_MODULES})





QT5_ADD_RESOURCES(TOTAL_LIB_RESOURCE_SOURCES ${TOTAL_LIB_RESOURCES})

ADD_LIBRARY(mtaoGraphics ${TOTAL_LIB_SOURCES} ${TOTAL_LIB_HEADERS}
    ${TOTAL_LIB_RESOURCE_SOURCES})

INSTALL( TARGETS mtaoGraphics LIBRARY DESTINATION lib ARCHIVE DESTINATION
    lib/static )






QT5_ADD_RESOURCES(TOTAL_RENDER_LIB_RESOURCE_SOURCES ${TOTAL_RENDER_LIB_RESOURCES})

ADD_LIBRARY(mtaoRendering ${TOTAL_RENDER_LIB_SOURCES} ${TOTAL_RENDER_LIB_HEADERS}
    ${TOTAL_RENDER_LIB_RESOURCE_SOURCES})


TARGET_LINK_LIBRARIES(mtaoRendering Qt5::OpenGL mtaoGraphics GLEW GL)

INSTALL( TARGETS mtaoRendering LIBRARY DESTINATION lib ARCHIVE DESTINATION
    lib/static )


ADD_SUBDIRECTORY(tools)
ADD_SUBDIRECTORY(tests EXCLUDE_FROM_ALL)




