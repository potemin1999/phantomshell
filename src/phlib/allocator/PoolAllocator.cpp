/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/28/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "Alloc.h"

#define ALLOCATION_INCREASE_COUNT 40

namespace {

typedef struct ChunkHeader {
    bool is_start_arena;
    bool is_free;
    Ptr next_chunk;
} ChunkHeader;


int chunk_header_size = sizeof(ChunkHeader);
ChunkHeader *first_chunk = nullptr;
ChunkHeader *last_chunk = nullptr;


Ptr allocate_new_chunks(Size object_size) {
    Size step = chunk_header_size + object_size;
    Ptr mem_ptr = phlib::malloc((chunk_header_size + object_size) * ALLOCATION_INCREASE_COUNT);
    char *pointer = (char *) mem_ptr;
    ChunkHeader *new_last_chunk = nullptr;
    for (int i = 0; i < ALLOCATION_INCREASE_COUNT; i++, pointer += step) {
        auto header = (ChunkHeader *) pointer;
        header->is_free = true;
        header->is_start_arena = (i == 0);
        if (i != ALLOCATION_INCREASE_COUNT - 1) {
            header->next_chunk = pointer + step;
        } else {
            header->next_chunk = nullptr;
            new_last_chunk = header;
        }
    }
    auto first_new_chunk = (ChunkHeader *) mem_ptr;
    if (first_chunk != nullptr) {
        last_chunk->next_chunk = first_new_chunk;
    } else {
        first_chunk = first_new_chunk;
    }
    last_chunk = new_last_chunk;
    return mem_ptr;
}


ChunkHeader *find_free_chunk() {
    Ptr current_ptr = (Ptr) first_chunk;
    ChunkHeader *header;
    while (current_ptr != nullptr) {
        header = (ChunkHeader *) current_ptr;
        if (header->is_free) {
            return header;
        }
        current_ptr = header->next_chunk;
    }
    return nullptr;
}

}

namespace phlib{
class PoolAllocator : public Allocator{

private:

    Size object_size;

public:

    PoolAllocator(Size object_size) noexcept;

    ~PoolAllocator();

    static PoolAllocator *get_instance(Size objects_size);

    Ptr allocate(Size size) override;

    void deallocate(Ptr ptr) override;

    void release() override;

    Ptr operator new(Size size);

    void operator delete(Ptr allocator_ptr);
};
}


phlib::PoolAllocator::PoolAllocator(Size object_size) noexcept : Allocator(){
    this->object_size = object_size;
}


phlib::PoolAllocator::~PoolAllocator() {
    release();
}


phlib::PoolAllocator *phlib::PoolAllocator::get_instance(Size objects_size) {
    return nullptr;
}


Ptr phlib::PoolAllocator::allocate(Size size) {
    ChunkHeader *free_chunk = find_free_chunk();
    if (free_chunk == nullptr) {
        allocate_new_chunks(object_size);
        free_chunk = find_free_chunk();
        if (free_chunk == nullptr)
            return nullptr;
    }
    free_chunk->is_free = false;
    char *ptr = (char *) free_chunk;
    return ptr + chunk_header_size;
}


void phlib::PoolAllocator::deallocate(Ptr ptr) {
    char *raw_ptr = (char *) ptr;
    auto header = (ChunkHeader *) (raw_ptr - chunk_header_size);
    header->is_free = true;
}


void phlib::PoolAllocator::release() {
    ChunkHeader *last_arena_starting_chunk = nullptr;
    ChunkHeader *current_chunk = first_chunk;
    while (current_chunk != nullptr) {
        if (current_chunk->is_start_arena) {
            if (last_arena_starting_chunk != nullptr) {
                phlib::free(last_arena_starting_chunk);
            }
            last_arena_starting_chunk = current_chunk;
        }
        current_chunk = (ChunkHeader *) current_chunk->next_chunk;
    }
}


Ptr phlib::PoolAllocator::operator new(Size size) {
    return phlib::malloc(size);
}


void phlib::PoolAllocator::operator delete(Ptr allocator_ptr) {
    phlib::free(allocator_ptr);
}
