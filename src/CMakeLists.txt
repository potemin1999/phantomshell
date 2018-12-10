project(SHELL C CXX)
set(CMAKE_CXX_STANDARD 11)
set(PHLIB_SOURCE_FILES
        phlib/allocator/pool_allocator.cpp
        phlib/allocator/system_allocator.cpp
        phlib/alloc.cpp
        phlib/istream.cpp
        phlib/ostream.cpp
        phlib/string.cpp
        )
add_library(phlib STATIC ${PHLIB_SOURCE_FILES})

set(SHELL_SOURCE_FILES
        shell/lexer.cpp
        shell/parser.cpp
        shell/phantom_shell.cpp
        shell/token.cpp
        )
add_compile_definitions(__simbuild__)
add_compile_definitions(__debug__)
include_directories("../include")
add_executable(${PROJECT_NAME} ${SHELL_SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS -lstdc++)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME} phlib)

if (BUILD_TESTING)
    enable_testing()
    set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
    add_subdirectory(tests)
endif ()