set(SHELL "psh")
set(SHELL_SOURCE_FILES
        Lexer.cpp
        Parser.cpp
        PhantomShell.cpp
        Token.cpp
        Tree.cpp)
add_executable(${SHELL} ${SHELL_SOURCE_FILES})
target_include_directories(${SHELL} PUBLIC ${MODULES_ROOT}/phlib/headers)
target_include_directories(${SHELL} PUBLIC headers)
set_target_properties(${SHELL} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${SHELL} phlib)