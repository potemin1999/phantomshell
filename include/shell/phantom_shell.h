/**
 * @headerfile
 * @author Ilya Potemin
 * @date 11/6/18.
 * @section LICENSE
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_PHANTOM_SHELL_H
#define PHANTOMSHELL_PHANTOM_SHELL_H

#include "shell_types.h"

namespace psh {

/**
 * @brief Parses shell flags from program startup arguments
 * @param flags struct to write into
 * @param argc arguments count, including program name
 * @param argv arguments - const char[][]
 * @return 0 if parsing was succeed, psh::shell_exit_code otherwise
 */
int parse_shell_flags(psh_arguments_t *flags, int argc, const char **argv);

/**
 * @brief Shell entry point
 * @param flags with flags data
 * @return psh::shell_exit_code
 */
int shell_main(psh_arguments_t *flags);

} //namespace psh

/**
 * @brief Program entry point
 * @param argc argv count
 * @param argv arguments array
 * @return psh::shell_exit_code from shell_main()
 */
int main(int argc, const char **argv);

#endif //PHANTOMSHELL_PHANTOM_SHELL_H
