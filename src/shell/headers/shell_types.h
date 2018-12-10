/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/15/18.
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_SHELL_TYPES_H
#define PHANTOMSHELL_SHELL_TYPES_H

#include "istream.h"
#include "ostream.h"

/** @brief Default Shell namespace */
namespace psh {

/**
 * @brief This structure stores all information from shell startup arguments
 */
typedef struct PshArguments {
    unsigned show_usage: 1; /**< flag, which requires usage of shell to be shown*/
    unsigned show_version: 1; /**< this flag requires version to be shown */
    unsigned debug_mode: 1; /**< If shell is running in debug mode, this is 1 */
    unsigned interactive_shell: 1; /**< 1 if shell is running in interactive mode */
    unsigned login_shell: 1; /**< 1 if shell is used as login shell */
    phlib::IStream *input_stream; /**< istream where shell script is located*/
    phlib::OStream *output_stream; /**< ostream for shell output*/
} PshArguments;

/**
 * @brief Describes shell exit codes
 */
typedef enum ShellExitCode {
    EXIT_NORMAL = 0x00, /**< Everything was correct, shell did its work*/
    EXIT_INVALID_ARGUMENTS = 0xa0, /**< Shell was unable to parse startup arguments*/
    EXIT_DUPLICATED_ARGUMENT = 0xa1 /**< Shell found correct argument repeated twice*/
} ShellExitCode;

} //namespace psh

#endif //PHANTOMSHELL_SHELL_TYPES_H
