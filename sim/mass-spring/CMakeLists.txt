cmake_minimum_required(VERSION 2.8.9)
PROJECT(MassSpring)

SET(CMAKE_RUNTIME_OUTPUT_DIR ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)

ADD_DEFINITIONS(-std=c++11)
ADD_DEFINITIONS(-Werror)
ADD_DEFINITIONS(-Wall)

# Include dirs
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)

# Source files
SET(MassSpring_SOURCES
    src/main.cpp
	src/mass_spring_system.cpp
    src/glwidget.cpp
    )

# Header files
SET(MassSpring_HEADERS
	include/mass_spring_system.hpp
    include/glwidget.h
    )

# Executable to build
ADD_EXECUTABLE(mass_spring_test
	${MassSpring_SOURCES}
	${MassSpring_HEADERS}
    )

qt5_use_modules(mass_spring_test Widgets OpenGL)
TARGET_LINK_LIBRARIES(mass_spring_test GL GLEW)
