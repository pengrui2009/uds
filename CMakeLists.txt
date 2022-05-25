cmake_minimum_required(VERSION 3.10)

project(uds LANGUAGES C)

# add_definitions(-DTEST_WIN32)

include_directories(
    $(CMAKE_CURRENT_SOURCE_DIR)
    src
    test
)

add_library(uds SHARED
    src/uds_ap.c
    src/uds_dl.c
    src/uds_q.c
    src/uds_tp.c
    src/uds.c
)

add_executable(test_server
    test/test_server_linux.c
)
target_link_libraries(test_server
    uds
    pthread
)