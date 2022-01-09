cmake_minimum_required(VERSION 3.18)
project(POSDownloader C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS -lcurl)

add_executable(POSDownloader main.c)

find_package(CURL REQUIRED)
target_link_libraries(POSDownloader CURL::libcurl pthread)