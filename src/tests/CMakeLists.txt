#allocators tests
set(PHLIB_ALLOCATORS_TEST phlib_allocators_tests)
set(PHLIB_ALLOCATORS_TEST_SRC sources/phlib/allocators_tests.cpp)
add_executable(${PHLIB_ALLOCATORS_TEST} ${PHLIB_ALLOCATORS_TEST_SRC})
set_target_properties(${PHLIB_ALLOCATORS_TEST} PROPERTIES LINK_FLAGS -lstdc++)
target_link_libraries(${PHLIB_ALLOCATORS_TEST} phlib)
add_test(NAME ${PHLIB_ALLOCATORS_TEST} COMMAND ${PHLIB_ALLOCATORS_TEST})

#streams tests
set(PHLIB_ISTREAM_TEST phlib_istream_tests)
set(PHLIB_ISTREAM_TEST_SRC sources/phlib/istream_tests.cpp)
add_executable(${PHLIB_ISTREAM_TEST} ${PHLIB_ISTREAM_TEST_SRC})
set_target_properties(${PHLIB_ISTREAM_TEST} PROPERTIES LINK_FLAGS -lstdc++)
target_link_libraries(${PHLIB_ISTREAM_TEST} phlib)
add_test(NAME ${PHLIB_ISTREAM_TEST} COMMAND ${PHLIB_ISTREAM_TEST})

#phlib tests
set(PHLIB_PHLIB_TEST phlib_phlib_tests)
set(PHLIB_PHLIB_TEST_SRC sources/phlib/phlib_tests.cpp)
add_executable(${PHLIB_PHLIB_TEST} ${PHLIB_PHLIB_TEST_SRC})
set_target_properties(${PHLIB_PHLIB_TEST} PROPERTIES LINK_FLAGS -lstdc++)
target_link_libraries(${PHLIB_PHLIB_TEST} phlib)
add_test(NAME ${PHLIB_PHLIB_TEST} COMMAND ${PHLIB_PHLIB_TEST})

#strings tests
set(PHLIB_STRING_TEST phlib_string_tests)
set(PHLIB_STRING_TEST_SRC sources/phlib/string_tests.cpp)
add_executable(${PHLIB_STRING_TEST} ${PHLIB_STRING_TEST_SRC})
set_target_properties(${PHLIB_STRING_TEST} PROPERTIES LINK_FLAGS -lstdc++)
target_link_libraries(${PHLIB_STRING_TEST} phlib)
add_test(NAME ${PHLIB_STRING_TEST} COMMAND ${PHLIB_STRING_TEST})

#lexer tests
set(SHELL_LEXER_TEST shell_lexer_tests)
set(SHELL_LEXER_TEST_SRC
        sources/shell/lexer_tests.cpp
        ${MODULES_ROOT}/shell/Lexer.cpp
        ${MODULES_ROOT}/shell/Token.cpp
        )
add_executable(${SHELL_LEXER_TEST} ${SHELL_LEXER_TEST_SRC})
target_include_directories(${SHELL_LEXER_TEST} PUBLIC ${MODULES_ROOT}/shell)
set_target_properties(${SHELL_LEXER_TEST} PROPERTIES LINK_FLAGS -lstdc++)
target_link_libraries(${SHELL_LEXER_TEST} phlib)
add_test(NAME ${SHELL_LEXER_TEST} COMMAND ${SHELL_LEXER_TEST})