project(SHELL C CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-lstdc++")
set(SHELL_SOURCE_FILES
        phantom_shell.cpp
        )
add_compile_definitions(__simbuild__)
add_compile_definitions(__debug__)
include_directories("${PROJECT_SOURCE_DIR}/include")
add_executable(${PROJECT_NAME} ${SHELL_SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)