cmake_minimum_required(VERSION 3.10)

project(uds LANGUAGES C)

# add_definitions(-DTEST_WIN32)

include_directories(
    $(CMAKE_CURRENT_SOURCE_DIR)
    src/uds_server
    test
)

add_library(uds SHARED
    src/uds_server/uds_ap.c
    src/uds_server/uds_dl.c
    src/uds_server/uds_q.c
    src/uds_server/uds_tp.c
    src/uds_server/uds.c
    src/uds_server/uds_phycan.c
)

add_library(udsc SHARED
    src/uds_client/uds_ap.c
    src/uds_client/uds_dl.c
    src/uds_client/uds_q.c
    src/uds_client/uds_tp.c
    src/uds_client/uds.c
    src/uds_client/can_drv.c
)


add_executable(test_server
    test/uds_server_test_linux.c
)

target_link_libraries(test_server
    uds
    pthread
    canlib
)