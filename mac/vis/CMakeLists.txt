cmake_minimum_required(VERSION 2.6)
PROJECT(MACVis)

SET(CMAKE_RUNTIME_OUTPUT_DIR ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)

ADD_DEFINITIONS(-std=c++11)
ADD_DEFINITIONS(-Werror)
ADD_DEFINITIONS(-Wall)

# Include dirs
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../core/include ${CMAKE_CURRENT_SOURCE_DIR}/include /usr/local/include/eigen3)
# Autogen'd moc code appears in the BINARY_DIR I believe
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})


# Source files
SET(MACVis_SOURCES
    src/demomain.cpp
    src/glwidget.cpp
    src/mainwindow.cpp
    )
# Include files
SET(MACVis_TOMOC
    include/mainwindow.h
    include/glwidget.h
    )


FIND_PACKAGE(Qt4 REQUIRED)
SET(QT_USE_QTOPENGL TRUE)

QT4_WRAP_CPP(MACVis_MOCed
    ${MACVis_TOMOC})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

# Executable to build
ADD_EXECUTABLE(MACVis
    ${MACVis_SOURCES}
    ${MACVis_HEADERS}
    ${MACVis_MOCed}
    )

TARGET_LINK_LIBRARIES(MACVis ${QT_LIBRARIES} GL GLEW)
