cmake_minimum_required(VERSION 3.8)
project(PhantomShell)
set(CMAKE_CXX_STANDARD 11)
set(BUILD_NAME PhantomShell)
set(BUILD_PATH "${PROJECT_SOURCE_DIR}/build")

include_directories("${PROJECT_SOURCE_DIR}/include")

add_subdirectory(src)