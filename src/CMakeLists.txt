project(SHELL C CXX)
set(CMAKE_CXX_STANDARD 11)
set(SHELL_SOURCE_FILES
        shell/lexer.cpp
        shell/parser.cpp
        shell/phantom_shell.cpp
        )
add_compile_definitions(__simbuild__)
add_compile_definitions(__debug__)
include_directories("../include")
add_executable(${PROJECT_NAME} ${SHELL_SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)