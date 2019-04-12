/**
 * @file
 * @author Ilya Potemin <i.potemin@innopolis.ru>
 * @date 3/24/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */
#include "Alloc.h"
#include "Types.h"

#define ABI_NAMESPACE __cxxabiv1

namespace ABI_NAMESPACE {

class __UNUSED __class_type_info {
    __UNUSED virtual void dummy();
};

class __UNUSED __si_class_type_info {
    __UNUSED virtual void dummy();
};
}

void ABI_NAMESPACE::__class_type_info::dummy() {}

void ABI_NAMESPACE::__si_class_type_info::dummy() {}

extern "C" {

__UNUSED void *__gxx_personality_v0;

__UNUSED void __cxa_throw_bad_array_new_length() {}

}