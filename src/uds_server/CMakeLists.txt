cmake_minimum_required(VERSION 3.10)

project(uds LANGUAGES C)

# add_definitions(-DTEST_WIN32)
add_definitions("-g")

include_directories(
    $(CMAKE_CURRENT_SOURCE_DIR)
    ../cmn
    ../drivers
)

add_library(udss SHARED
    uds_ap.c
    uds_dl.c
    uds_q.c
    uds_tp.c
    uds.c
)

add_executable(uds_server
    uds_server_linux.c
)

target_link_libraries(uds_server
    udss
    virtualcan
    pthread
    canlib
)