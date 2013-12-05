CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
PROJECT( BrickFEM )










SET(CMAKE_RUNTIME_OUTPUT_DIR ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)

SET(CMAKE_INCLUDE_CURRENT_DIR_ON)
SET(CMAKE_AUTOMOC ON)
FIND_PACKAGE(Qt5Core)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include $ENV{blockFEM_PATH} /usr/local/include/eigen3)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../core/include)

ADD_DEFINITIONS(-std=c++11)
ADD_DEFINITIONS(-Wall)
ADD_DEFINITIONS(-Werror)

SET(blockFEM_SOURCES
    src/lattice.cpp
    )
SET(blockFEM_HEADERS
    include/lattice.h
    include/macros.h
    src/lattice.hpp
    )
ADD_CUSTOM_TARGET(headers SOURCES ${blockFEM_HEADERS})
ADD_LIBRARY(blockFEM STATIC ${blockFEM_SOURCES} ${blockFEM_HEADERS})
INSTALL(FILES ${blockFEM_HEADERS} DESTINATION /usr/local/include/blockFEM)


ADD_EXECUTABLE(test src/testmain.cpp ${blockFEM_HEADERS})



##Visualization side, keeping it optional of sorts
#
#SET(blockFEMvis_SOURCES
#    src/main.cpp
#    src/mainwindow.cpp
#    src/glwidget.cpp
#    src/glutil.cpp
#    src/qvoxelSolver.cpp
#    src/sidebar.cpp
#    )
#
#SET(blockFEMvis_HEADERS
#    include/glutil.h
#    include/mainwindow.h
#    include/qvoxelSolver.h
#    include/glwidget.h
#    include/sidebar.h
#    )
#
#SET(blockFEMvis_RESOURCES
#    shaders/shaders.qrc
#    )
#QT5_ADD_RESOURCES(blockFEMvis_QRC ${blockFEMvis_RESOURCES})
#SET(blockFEMvis_SHADERS
#    shaders/vertex.130.v.glsl
#    shaders/vertex.130.f.glsl
#    )
#
#ADD_CUSTOM_TARGET(THIS_LETS_QTCREATOR_SEE_SHADERS
#    SOURCES ${blockFEMvis_SHADERS}
#    )
#
#
#ADD_EXECUTABLE(blockFEMvis ${blockFEMvis_SOURCES} ${blockFEMvis_HEADERS} ${blockFEMvis_QRC} ${blockFEMvis_SOURCES}) 
#TARGET_LINK_LIBRARIES(blockFEMvis ${QT_LIBRARIES} blockFEM GL GLEW)
#QT5_USE_MODULES(blockFEMvis Widgets Gui)



