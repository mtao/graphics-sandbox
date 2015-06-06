cmake_minimum_required(VERSION 2.8.9)
PROJECT(template)

SET(CMAKE_RUNTIME_OUTPUT_DIR ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)

ADD_DEFINITIONS(-std=c++1z)
#ADD_DEFINITIONS(-Werror)
ADD_DEFINITIONS(-Wall)
ADD_DEFINITIONS(-Wno-deprecated-declarations)
ADD_DEFINITIONS(-Wno-unused-variable)
ADD_DEFINITIONS(-Wno-unused-local-typedef)

# Include dirs
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include
    /usr/local/include/eigen3)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)

FIND_PACKAGE(Protobuf)
IF ( PROTOBUF_FOUND )
    INCLUDE_DIRECTORIES( ${PROTOBUF_INCLUDE_DIR} )
    SET(PROTOBUF_IMPORT_DIRS proto)
    #PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS proto/proto.proto)
    SET(PROTO_SRCS ${PROTO_SRCS})
    SET(PROTO_HDRS ${PROTO_HDRS})
    SET(PROTOBUF_LIBS ${PROTOBUF_LIBRARY})
ENDIF(PROTOBUF_FOUND)

# Source files
SET(SOURCES
    src/main.cpp
    src/window.cpp
    ${PROTO_SRCS}
    )

# Header files
SET(HEADERS
    include/window.h
    ${PROTO_HDRS}
    )

# Executable to build
ADD_EXECUTABLE(template ${SOURCES} ${HEADERS})

#qt5_use_modules(template Widgets OpenGL Gui)
TARGET_LINK_LIBRARIES(template Qt5::Widgets Qt5::Gui Qt5::Core
    ${PROTOBUF_LIBS})
