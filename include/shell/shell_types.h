/**
 * @headerfile
 * @author Ilya Potemin
 * @date 11/15/18.
 * @section LICENSE
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_SHELL_TYPES_H
#define PHANTOMSHELL_SHELL_TYPES_H

#include <phlib/istream.h>

namespace psh {

/**
 * @brief This structure stores all information from shell startup arguments
 */
typedef struct psh_arguments_t {
    unsigned debug_mode: 1; /**< If shell is running in debug mode, this is 1 */
    unsigned interactive_shell: 1; /**< 1 if shell is running in interactive mode */
    unsigned login_shell: 1; /**< 1 if shell is used as login shell */
    phlib::istream *input_stream; /**< istream where shell script is located*/
} psh_arguments_t;

/**
 * @brief Describes shell exit codes
 */
typedef enum shell_exit_codes {
    EXIT_NORMAL = 0x00, /**< Everything was correct, shell did its work*/
    EXIT_INVALID_ARGUMENTS = 0xa0 /**< Shell was unable to parse startup arguments*/
} shell_exit_codes;

} //namespace psh

#endif //PHANTOMSHELL_SHELL_TYPES_H
