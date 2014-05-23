INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

#QT5_ADD_RESOURCES(grid_renderer_RCC
#    resources/410/410_shaders.qrc
#    resources/120/120_shaders.qrc
#    )
SET(grid_renderer_HEADERS 
    include/glwidget.h
    include/mainwindow.h
    )
SET(grid_renderer_SOURCES
    src/glwidget.cpp
    src/main.cpp
    src/mainwindow.cpp
    )

ADD_EXECUTABLE(gridRenderer ${grid_renderer_SOURCES} ${grid_renderer_HEADERS}
    ${grid_renderer_RCC})
TARGET_LINK_LIBRARIES(gridRenderer Qt5::OpenGL mtaoGraphics mtaoRendering)
