########## PLAYGROUND ##########

add_executable(playground playground.cpp)

target_include_directories(playground PUBLIC
                            ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(playground PUBLIC jumjum-geom)

########## TEST ##########

set(UNIT_TEST_SOURCES
    unittests/getOrientation.test.cpp
    unittests/linesegment.test.cpp
    unittests/point.test.cpp
    unittests/circle.test.cpp
    unittests/events.test.cpp
    unittests/triangulation.test.cpp
    unittests/triangle.test.cpp
    unittests/trianglesearch.test.cpp)

set(CORRECTNESS_TEST_SOURCES
    correctnesstests/planesweep.test.cpp
    correctnesstests/triangulation.test.cpp)

add_executable(test test.cpp ${UNIT_TEST_SOURCES}
                             ${CORRECTNESS_TEST_SOURCES})

target_include_directories(test PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(test PUBLIC jumjum-geom)