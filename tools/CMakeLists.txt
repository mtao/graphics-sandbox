CMAKE_MINIMUM_REQUIRED( VERSION 3.7.1 )
PROJECT( Tools )




INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(SYSTEM /usr/include/eigen3)
ADD_DEFINITIONS(-std=c++14)
ADD_DEFINITIONS(-Wall)
ADD_DEFINITIONS(-g)



find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

find_package(Threads)






SET(VIS_HDRS
    include/renderer.hpp
    include/imgui_impl.h
    include/shader_program.hpp
    include/line_renderer.hpp
    include/mesh_renderer.hpp
    )
SET(VIS_SRCS
    src/line_renderer.cpp
    src/renderer.cpp
    src/imgui_impl.cpp
    src/shader_program.cpp
    src/mesh_renderer.cpp

    external/imgui/imgui_draw.cpp
    external/imgui/imgui.cpp
    )




#SET(CORE_LIBS imgproto ${PROTOBUF_LIBRARY} ${ImageMagick_LIBRARIES}) 


ADD_EXECUTABLE(vis_demo
    src/visualization_client.cpp
    ${VIS_SRCS}
    ${VIS_HDRS}
    )


TARGET_INCLUDE_DIRECTORIES(vis_demo SYSTEM PUBLIC ${GLFW3_INCLUDE_DIRS})
TARGET_INCLUDE_DIRECTORIES(vis_demo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)
TARGET_LINK_LIBRARIES(vis_demo GLEW glfw ${OPENGL_GL_LIBRARY} GL
    ${CMAKE_THREAD_LIBS_INIT})




ADD_EXECUTABLE(voronoi_test
    src/voronoi_test.cpp
    ${VIS_SRCS}
    ${VIS_HDRS}
    )


TARGET_INCLUDE_DIRECTORIES(voronoi_test SYSTEM PUBLIC ${GLFW3_INCLUDE_DIRS})
TARGET_INCLUDE_DIRECTORIES(voronoi_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)
TARGET_LINK_LIBRARIES(voronoi_test GLEW glfw ${OPENGL_GL_LIBRARY} GL
    ${CMAKE_THREAD_LIBS_INIT})

