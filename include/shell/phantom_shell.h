/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/6/18.
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_PHANTOM_SHELL_H
#define PHANTOMSHELL_PHANTOM_SHELL_H

#define PHANTOM_SHELL_VERSION "PhantomShell version 0.001"

#include "shell_types.h"
#include "lexer.h"
#include "parser.h"

/** @brief Default Shell namespace */
namespace psh {

/**
 * @brief Root level class in shell structure
 */
class PhantomShell {

private:

    PshArguments* psh_arguments;
    Lexer* lexer;

public:

    PhantomShell(PshArguments* args);

    ~PhantomShell();

    ShellExitCode run();

};

/**
 * @brief Parses shell flags from program startup arguments
 * @param args struct to write into
 * @param argc arguments count, including program name
 * @param argv arguments - const char[][]
 * @return 0 if parsing was succeed, @ref ShellExitCode otherwise
 */
uint32 parse_shell_args(PshArguments *args, int argc, const char **argv);

/**
 * @brief Can parse shell option, starts with "-"
 * @param args to write into
 * @param option to parse
 * @return 0 if succeed, @ref ShellExitCode otherwise
 */
uint32 parse_shell_short_options(PshArguments *args,const char* option);

/**
 * @brief Can parse shell option, starts with "--"
 * @param args to write into
 * @param option to parse
 * @return 0 if succeed, @ref ShellExitCode otherwise
 */
uint32 parse_shell_long_options(PshArguments *args,const char* option);

/**
 *
 * @param args
 * @return
 */
uint32 cleanup_shell_args(PshArguments *args);

/**
 * @brief Shows shell usage
 * @param args from which the output stream wil be taken
 * @return exit code value
 */
ShellExitCode shell_show_usage(PshArguments *args);

/**
 * @brief Shows shell version
 * @param args where output stream to write into is stored
 * @return exit code value
 */
ShellExitCode shell_show_version(PshArguments *args);

/**
 * @brief Shell entry point
 * @param args with flags data
 * @return psh::shell_exit_code
 */
ShellExitCode shell_main(PshArguments *args);

} //namespace psh

/**
 * @brief Program entry point
 * @param argc argv count
 * @param argv arguments array
 * @return psh::shell_exit_code from shell_main()
 */
int main(int argc, const char **argv);

#endif //PHANTOMSHELL_PHANTOM_SHELL_H
