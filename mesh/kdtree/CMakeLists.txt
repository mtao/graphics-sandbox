INCLUDE(../../CMakeDefaults.txt)
PROJECT(KDTree)


# Include dirs


# Source files
SET(KDTree_SOURCES
    src/main.cpp
    src/kdtree.cpp
    )
# Include files
SET(KDTree_HEADERS
    include/kdtree.h
    include/kdtreebase.h
    )

ADD_EXECUTABLE(KDTree
    ${KDTree_SOURCES}
    ${KDTree_HEADERS})

