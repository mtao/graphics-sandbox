CMAKE_MINIMUM_REQUIRED( VERSION 2.8.9 )
PROJECT(MeshRender)

SET(INSTALL_PATH /usr/local/)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/include
    ./include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    /usr/local/include/eigen3/unsupported
    /usr/local/include/eigen3
    /home/mtao/hg/eigen
    /home/mtao/svn/vcglib
    )
ADD_DEFINITIONS(-std=c++11)
SET(MeshRender_SOURCES
    src/mainwindow.cpp
    src/glwidget.cpp
    src/plylib.cpp
    )
SET(MeshRender_HEADERS
    include/mainwindow.h
    include/glwidget.h
    include/mesh.h
    )
SET(MeshRender_RESOURCE
    shaders/shaders.qrc
    )
SET(MeshRender_SHADERS
    shaders/shader.v.glsl
    shaders/shader.130.v.glsl
    shaders/shader.f.glsl
    shaders/shader.130.f.glsl
    shaders/noneshader.130.v.glsl
    shaders/shader.g.glsl
    shaders/faceshader.g.glsl
    shaders/edgeshader.g.glsl
    shaders/vertshader.g.glsl
    shaders/particle.f.glsl
    shaders/particle.v.glsl
    )
#This target has an obvious purpose :)
ADD_CUSTOM_TARGET(MAKE_SHADERS_VISIBLE_IN_QTCREATOR
    SOURCES ${MeshRender_SHADERS})

# Qt
SET(CMAKE_INCLUDE_CURRENT_DIR_ON)
SET(CMAKE_AUTOMOC ON)
FIND_PACKAGE(Qt5Core)

QT5_ADD_RESOURCES(MeshRender_RCC ${MeshRender_RESOURCE})

ADD_LIBRARY(MeshRender STATIC ${MeshRender_SOURCES} ${MeshRender_HEADERS} ${MeshRender_RCC} ${MeshRender_SHADERS})

TARGET_LINK_LIBRARIES(MeshRender ${QT_LIBRARIES} GLEW GL GLU mtaoMeshCore)


ADD_EXECUTABLE(meshRender src/main.cpp)
TARGET_LINK_LIBRARIES(meshRender MeshRender)


QT5_USE_MODULES(MeshRender Widgets OpenGL)
QT5_USE_MODULES(meshRender Widgets OpenGL)


INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/meshRender DESTINATION ${INSTALL_PATH}/bin)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/libMeshRender.a DESTINATION
    ${INSTALL_PATH}/lib)
INSTALL(FILES ${MeshRender_HEADERS} DESTINATION
    ${INSTALL_PATH}/include/meshRender)
