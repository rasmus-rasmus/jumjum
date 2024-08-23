set(PRIMITIVES_SOURCE_FILES
        primitives/point.cpp
        primitives/linesegment.cpp
        primitives/circle.cpp
        primitives/triangle.cpp)

set(ALGORITHMS_SOURCE_FILES
        algorithms/planesweep/events.cpp
        algorithms/planesweep/planesweep.cpp
        algorithms/delaunay/triangulation.cpp)

set(UTILITY_SOURCE_FILES
        utility/geomutils.cpp
        utility/io.cpp)

add_library(jumjum-geom ${PRIMITIVES_SOURCE_FILES}
                        ${ALGORITHMS_SOURCE_FILES}
                        ${UTILITY_SOURCE_FILES})

target_include_directories(jumjum-geom PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_subdirectory(executables)