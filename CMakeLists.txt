cmake_minimum_required(VERSION 3.8)
project(PhantomShell)
set(CMAKE_CXX_STANDARD 11)
set(BUILD_NAME PhantomShell)
set(BUILD_PATH "${PROJECT_SOURCE_DIR}/build")

include_directories("${PROJECT_SOURCE_DIR}/include")

#set(SHELL_NAME PhantomShellMain)

#add_subdirectory(src/phlib)
add_subdirectory(src/shell)

#set(SOURCE_FILES src/main.cpp)
#add_executable(${BUILD_NAME} ${SOURCE_FILES})
#target_link_libraries(${BUILD_NAME} SHELL PHLIB)
#set_target_properties(${BUILD_NAME} PROPERTIES LINKER_LANGUAGE CXX)