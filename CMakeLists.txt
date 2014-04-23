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
    src/core/linereader.cpp
    )
SET(core_HEADERS 
    include/core/coord.h
    include/core/eigen_hash.h  
    include/core/eigen_ordering.h
    include/core/kdop.h
    include/core/linereader.hpp
    include/core/types.h
    )



SET(ACTIVE_MODULES core )

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






QT5_ADD_RESOURCES(mesh_renderer_RCC
    resources/mesh/mesh_renderer/mesh_renderer.qrc)
SET(mesh_renderer_HEADERS 
    ${core_HEADERS}
    include/mesh/mesh_renderer/glwidget.h
    include/mesh/mesh_renderer/scene.h
    )
SET(mesh_renderer_SOURCES
    ${core_SOURCES}
    src/mesh/mesh_renderer/glwidget.cpp
    src/mesh/mesh_renderer/main.cpp
    src/mesh/mesh_renderer/scene.cpp
    )

ADD_EXECUTABLE(meshRenderer ${mesh_renderer_SOURCES} ${mesh_renderer_HEADERS}
    ${mesh_renderer_RCC})
TARGET_LINK_LIBRARIES(meshRenderer Qt5::OpenGL)
