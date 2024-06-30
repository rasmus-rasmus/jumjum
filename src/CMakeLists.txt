set(PRIMITIVES_SOURCE_FILES
        primitives/point.cpp
        primitives/linesegment.cpp)

add_library(jumjum-geom ${PRIMITIVES_SOURCE_FILES})

add_subdirectory(executables)