cmake_minimum_required(VERSION 3.8)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "-Wall -Wreturn-type")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O2")

project(PhantomShell)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(BUILD_NAME PhantomShell)
set(BUILD_PATH "${PROJECT_SOURCE_DIR}/build")
set(BUILD_TESTING true)
set(CMAKE_BINARY_DIR "${BUILD_PATH}/cmake/bin")
set(CMAKE_LIBRARY_PATH "${BUILD_PATH}/cmake/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BUILD_PATH}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BUILD_PATH}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_PATH}/bin")

add_compile_definitions(__simbuild__)
add_compile_definitions(__debug__)

#shell module
set(SHELL psh)
file(GLOB_RECURSE SHELL_SOURCE_FILES "src/*.c")
add_executable(${SHELL} ${SHELL_SOURCE_FILES})
set_target_properties(${SHELL} PROPERTIES LINKER_LANGUAGE C)