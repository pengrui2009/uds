cmake_minimum_required(VERSION 3.10)

project(uds LANGUAGES C)

# add_definitions(-DTEST_WIN32)

include_directories(
    $(CMAKE_CURRENT_SOURCE_DIR)
)

add_library(udsc SHARED
    uds_ap.c
    uds_dl.c
    uds_q.c
    uds_tp.c
    uds.c
    can_drv.c
)

add_executable(uds_client
    uds_client_linux.c
)

target_link_libraries(uds_client
    udsc
    pthread
    canlib
)