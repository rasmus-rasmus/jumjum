

add_executable(playground playground.cpp)

target_include_directories(playground PUBLIC
                            ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(playground PUBLIC jumjum-geom)