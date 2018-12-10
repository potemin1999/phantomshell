set(PHLIB_ALLOCATORS_TEST phlib_allocators_tests)
set(PHLIB_ALLOCATORS_TEST_SRC
        allocators_tests.cpp
        ../../phlib/allocator/pool_allocator.cpp
        ../../phlib/allocator/system_allocator.cpp
        )
add_executable(${PHLIB_ALLOCATORS_TEST} ${PHLIB_ALLOCATORS_TEST_SRC})
set_target_properties(${PHLIB_ALLOCATORS_TEST} PROPERTIES LINK_FLAGS -lstdc++)
target_link_libraries(${PHLIB_ALLOCATORS_TEST} phlib)
add_test(NAME ${PHLIB_ALLOCATORS_TEST} COMMAND ${PHLIB_ALLOCATORS_TEST})

set(PHLIB_ISTREAM_TEST phlib_istream_tests)
set(PHLIB_ISTREAM_TEST_SRC istream_tests.cpp)
add_executable(${PHLIB_ISTREAM_TEST} ${PHLIB_ISTREAM_TEST_SRC})
set_target_properties(${PHLIB_ISTREAM_TEST} PROPERTIES LINK_FLAGS -lstdc++)
target_link_libraries(${PHLIB_ISTREAM_TEST} phlib)
add_test(NAME ${PHLIB_ISTREAM_TEST} COMMAND ${PHLIB_ISTREAM_TEST})

set(PHLIB_STRING_TEST phlib_string_tests)
set(PHLIB_STRING_TEST_SRC string_tests.cpp)
add_executable(${PHLIB_STRING_TEST} ${PHLIB_STRING_TEST_SRC})
set_target_properties(${PHLIB_STRING_TEST} PROPERTIES LINK_FLAGS -lstdc++)
target_link_libraries(${PHLIB_STRING_TEST} phlib)
add_test(NAME ${PHLIB_STRING_TEST} COMMAND ${PHLIB_STRING_TEST})