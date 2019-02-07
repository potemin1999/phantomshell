set(PHLIB "phlib")
set(PHLIB_SOURCE_FILES
        sources/allocator/pool_allocator.cpp
        sources/allocator/SystemAllocator.cpp
        sources/Allocator.cpp
        sources/InputStream.cpp
        sources/OutputStream.cpp
        sources/String.cpp
        )
add_library(${PHLIB} STATIC ${PHLIB_SOURCE_FILES})
target_include_directories(${PHLIB} PUBLIC headers)