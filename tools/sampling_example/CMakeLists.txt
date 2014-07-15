INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)


ADD_EXECUTABLE(sampling_example main.cpp)
TARGET_LINK_LIBRARIES(sampling_example glut GL )
