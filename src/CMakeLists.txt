project(SHELL C CXX)
set(CMAKE_CXX_STANDARD 11)
add_compile_definitions(__simbuild__)
add_compile_definitions(__debug__)
set(MODULES_ROOT "${PROJECT_SOURCE_DIR}")

#support library module for shell
file(GLOB_RECURSE PHLIB_SOURCE_FILES "phlib/*.cpp")
set(PHLIB phlib)
add_library(${PHLIB} STATIC ${PHLIB_SOURCE_FILES})
target_include_directories(${PHLIB} PUBLIC ${MODULES_ROOT}/phlib)

#shell module
file(GLOB_RECURSE SHELL_SOURCE_FILES "shell/*.cpp")
set(SHELL psh)
add_executable(${SHELL} ${SHELL_SOURCE_FILES})
target_include_directories(${SHELL} PUBLIC ${MODULES_ROOT}/phlib)
target_include_directories(${SHELL} PUBLIC ${MODULES_ROOT}/shell)
set_target_properties(${SHELL} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${SHELL} phlib)

#test modules, if needed
if (BUILD_TESTING)
    enable_testing()
    set_target_properties(${SHELL} PROPERTIES LINKER_LANGUAGE CXX)
    add_subdirectory(tests)
endif ()