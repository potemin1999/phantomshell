set(SHELL_LEXER_TEST shell_lexer_tests)
set(SHELL_LEXER_TEST_SRC lexer_tests.cpp ../../shell/lexer.cpp)
add_executable(${SHELL_LEXER_TEST} ${SHELL_LEXER_TEST_SRC})
add_test(NAME ${SHELL_LEXER_TEST} COMMAND ${SHELL_LEXER_TEST})