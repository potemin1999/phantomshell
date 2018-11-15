/**
 * @headerfile
 * @author Ilya Potemin
 * @date 11/15/18.
 * @section LICENSE
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_ISTREAM_H
#define PHANTOMSHELL_ISTREAM_H

#include "string.h"

namespace phlib {

/**
 * @class istream is Input Stream and it takes input from requested location
 */
class istream {
protected:

    void *buffer;

public:

    /**
     * @brief Creates input stream from file
     * @param file_path
     */
    istream(string &file_path) {
    }

    /**
     * @brief Creates input stream from object
     * @param byte_buffer to use as source
     * @param do_copy determines whether it should be copied
     */
    istream(void *byte_buffer, bool do_copy) {
    }


};

} //namespace phlib
#endif //PHANTOMSHELL_ISTREAM_H
