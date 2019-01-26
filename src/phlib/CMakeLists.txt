set(PHLIB "phlib")
set(PHLIB_SOURCE_FILES
        sources/allocator/pool_allocator.cpp
        sources/allocator/system_allocator.cpp
        sources/alloc.cpp
        sources/istream.cpp
        sources/ostream.cpp
        sources/string.cpp
        )
add_library(${PHLIB} STATIC ${PHLIB_SOURCE_FILES})
target_include_directories(${PHLIB} PUBLIC headers)