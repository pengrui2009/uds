cmake_minimum_required(VERSION 3.10)

project(uds LANGUAGES C)

# add_definitions(-DTEST_WIN32)

add_subdirectory(src/uds_client)

add_subdirectory(src/uds_server)