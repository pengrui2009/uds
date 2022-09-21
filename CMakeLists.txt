cmake_minimum_required(VERSION 3.10)

project(uds_test LANGUAGES C)

# add_definitions(-DTEST_WIN32)

include_directories(
    $(CMAKE_CURRENT_SOURCE_DIR)
    src
    test
)

add_library(udsc SHARED
    src/uds_ap.c
    src/uds_dl.c
    src/uds_q.c
    src/uds_tp.c
    src/uds.c
)

add_executable(uds_test
    test/test.c
)

target_link_libraries(uds_test
    udsc
    pthread
    canlib
)