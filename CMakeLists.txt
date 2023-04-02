cmake_minimum_required(VERSION 3.10)

project(uds LANGUAGES C)

# 设置静态库文件目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 动态库文件目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 可执行文件目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


include_directories(
    cmn
)
# add_definitions(-DTEST_WIN32)

# add_subdirectory(src/cmn)
add_subdirectory(src/drivers)

add_subdirectory(src/uds_client)

add_subdirectory(src/uds_server)