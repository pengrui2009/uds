cmake_minimum_required(VERSION 3.10)

project(uds LANGUAGES C)

# add_definitions(-DTEST_WIN32)

include_directories(
    $(CMAKE_CURRENT_SOURCE_DIR)
    ../cmn
)

add_library(virtualcan SHARED
    virtual_socketcan.c
)

add_library(kvasercan SHARED
    kvasercan.c
)

# add_executable(uds_client
#     uds_client_linux.c
# )

# target_link_libraries(uds_client
#     udsc
#     pthread
#     canlib
# )