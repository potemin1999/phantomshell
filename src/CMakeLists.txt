project(SHELL C CXX)
set(CMAKE_CXX_STANDARD 11)
add_compile_definitions(__simbuild__)
add_compile_definitions(__debug__)
set(MODULES_ROOT "${PROJECT_SOURCE_DIR}")

#support library module for shell
add_subdirectory(phlib)

#shell module
add_subdirectory(shell)

#test modules, if needed
if (BUILD_TESTING)
    enable_testing()
    set_target_properties(${SHELL} PROPERTIES LINKER_LANGUAGE CXX)
    add_subdirectory(tests)
endif ()