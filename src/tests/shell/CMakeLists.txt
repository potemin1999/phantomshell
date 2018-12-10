set(SHELL_LEXER_TEST shell_lexer_tests)
set(SHELL_LEXER_TEST_SRC lexer_tests.cpp
        ../../shell/lexer.cpp
        ../../shell/token.cpp
        ../../phlib/allocator/pool_allocator.cpp
        )
add_executable(${SHELL_LEXER_TEST} ${SHELL_LEXER_TEST_SRC})
set_target_properties(${SHELL_LEXER_TEST} PROPERTIES LINK_FLAGS -lstdc++)
target_link_libraries(${SHELL_LEXER_TEST} phlib)
add_test(NAME ${SHELL_LEXER_TEST} COMMAND ${SHELL_LEXER_TEST})