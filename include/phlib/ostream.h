/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/15/18
 * @section LICENSE
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_OSTREAM_H
#define PHANTOMSHELL_OSTREAM_H

#include "string.h"

namespace phlib {

/**
 * @class ostream represents Output Stream to some location
 */
class ostream {
protected:

    void *buffer;

public:

    /**
     * @brief Creates stream, writing to the file
     * @param file_path
     */
    ostream(string &file_path) {
    }

    /**
     * @brief Creates stream, writing to pointed buffer
     * @param buffer
     */
    ostream(void *buffer) {
    }

};

} //namespace phlib
#endif //PHANTOMSHELL_OSTREAM_H
