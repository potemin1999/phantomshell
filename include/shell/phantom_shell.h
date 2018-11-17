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

#define PHANTOM_SHELL_VERSION "PhantomShell version 0.001"

#include "shell_types.h"

namespace psh {

/**
 * @brief Parses shell flags from program startup arguments
 * @param args struct to write into
 * @param argc arguments count, including program name
 * @param argv arguments - const char[][]
 * @return 0 if parsing was succeed, psh::shell_exit_code otherwise
 */
uint32 parse_shell_args(psh_arguments_t *args, int argc, const char **argv);

/**
 * @brief Shows shell usage
 * @param args ,from which the output stream wil be taken
 * @return exit code value
 */
shell_exit_codes shell_show_usage(psh_arguments_t *args);

/**
 * @brief Shows shell version
 * @param args ,where output stream to write into is stored
 * @return exit code value
 */
shell_exit_codes shell_show_version(psh_arguments_t *args);

/**
 * @brief Shell entry point
 * @param args with flags data
 * @return psh::shell_exit_code
 */
shell_exit_codes shell_main(psh_arguments_t *args);

} //namespace psh

/**
 * @brief Program entry point
 * @param argc argv count
 * @param argv arguments array
 * @return psh::shell_exit_code from shell_main()
 */
int main(int argc, const char **argv);

#endif //PHANTOMSHELL_PHANTOM_SHELL_H
