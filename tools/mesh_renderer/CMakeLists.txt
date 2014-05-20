INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

QT5_ADD_RESOURCES(mesh_renderer_RCC
    resources/410/410_shaders.qrc
    resources/120/120_shaders.qrc
    )
SET(mesh_renderer_HEADERS 
    include/glwidget.h
    )
SET(mesh_renderer_SOURCES
    src/glwidget.cpp
    src/main.cpp
    )

ADD_EXECUTABLE(meshRenderer ${mesh_renderer_SOURCES} ${mesh_renderer_HEADERS}
    ${mesh_renderer_RCC})
TARGET_LINK_LIBRARIES(meshRenderer Qt5::OpenGL mtaoGraphics mtaoRendering )
