########## PLAYGROUND ##########

add_executable(playground playground.cpp)

target_include_directories(playground PUBLIC
                            ${PROJECT_SOURCE_DIR}/src)

########## TEST ##########

set(UNIT_TEST_SOURCES
    unittests/getOrientation.test.cpp
    unittests/linesegment.test.cpp)

add_executable(test test.cpp ${UNIT_TEST_SOURCES})

target_include_directories(test PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(test PUBLIC jumjum-geom)