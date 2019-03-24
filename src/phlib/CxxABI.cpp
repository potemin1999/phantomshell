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

void *__gxx_personality_v0;

namespace phabiv1 {
class __class_type_info {
    virtual void dummy();
};

class __si_class_type_info {
    virtual void dummy();
};
};

void phabiv1::__class_type_info::dummy() {}

void phabiv1::__si_class_type_info::dummy() {}

namespace abi = phabiv1;