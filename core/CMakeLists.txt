CMAKE_MINIMUM_REQUIRED( VERSION 2.8.9 )
PROJECT( GraphicsSandboxCore )






SET(CMAKE_RUNTIME_OUTPUT_DIR ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include /usr/local/include/eigen3)

ADD_DEFINITIONS(-std=c++11)
ADD_DEFINITIONS(-Wall)
ADD_DEFINITIONS(-Werror)
ADD_DEFINITIONS(-Wno-unused-local-typedefs)


ENABLE_TESTING()

ADD_EXECUTABLE(kdopTest src/test_kdop.cpp include/kdop.h)


ADD_TEST(kdopTest ${CMAKE_CURRENT_BINARY_DIR}/kdopTest)







add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS kdopTest)
