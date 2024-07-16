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
    unittests/events.test.cpp)

set(END_TO_END_TEST_SOURCES
    endtoendtests/planesweep.test.cpp)

add_executable(test test.cpp ${UNIT_TEST_SOURCES}
                             ${END_TO_END_TEST_SOURCES})

target_include_directories(test PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(test PUBLIC jumjum-geom)