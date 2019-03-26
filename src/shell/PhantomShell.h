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

#include "IStream.h"
#include "OStream.h"
#include "Lexer.h"
#include "Parser.h"

/** @brief Default Shell namespace */
namespace psh {

//@formatter:off
/**
 * @brief This structure stores all information from shell startup arguments
 */
typedef struct PshArguments {
    unsigned int   showUsage: 1;        /**< flag, which requires usage of shell to be shown*/
    unsigned int   showVersion: 1;      /**< this flag requires version to be shown */
    unsigned int   debugMode: 1;        /**< If shell is running in debug mode, this is 1 */
    unsigned int   interactiveShell: 1; /**< 1 if shell is running in interactive mode */
    unsigned int   loginShell: 1;       /**< 1 if shell is used as login shell */
    phlib::IStream *inputStream;        /**< istream where shell script is located*/
    phlib::OStream *outputStream;       /**< ostream for shell output*/
} PshArguments;

/**
 * @brief Describes shell exit codes
 */
typedef enum ShellExitCode {
    EXIT_NORMAL              = 0x00, /**< Everything was correct, shell did its work*/
    EXIT_INVALID_ARGUMENTS   = 0xa0, /**< Shell was unable to parse startup arguments*/
    EXIT_DUPLICATED_ARGUMENT = 0xa1  /**< Shell found correct argument repeated twice*/
} ShellExitCode;

//@formatter:on

/**
 * @brief Root level class in shell structure
 */
class PhantomShell {

private:

    PshArguments *pshArguments;
    Lexer *lexer;
    Parser *parser;

public:

    explicit PhantomShell(PshArguments *args);

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
UInt32 ParseShellArgs(PshArguments *args, int argc, const char **argv);

/**
 * @brief Can parse shell option, starts with "-"
 * @param args to write into
 * @param option to parse
 * @return 0 if succeed, @ref ShellExitCode otherwise
 */
UInt32 ParseShellShortOptions(PshArguments *args, const char *option);

/**
 * @brief Can parse shell option, starts with "--"
 * @param args to write into
 * @param option to parse
 * @return 0 if succeed, @ref ShellExitCode otherwise
 */
UInt32 ParseShellLongOptions(PshArguments *args, const char *option);

/**
 *
 * @param args
 * @return
 */
UInt32 CleanupShellArgs(PshArguments *args);

/**
 * @brief Shows shell usage
 * @param args from which the output stream wil be taken
 * @return exit code value
 */
ShellExitCode ShellShowUsage(PshArguments *args);

/**
 * @brief Shows shell version
 * @param args where output stream to write into is stored
 * @return exit code value
 */
ShellExitCode ShellShowVersion(PshArguments *args);

/**
 * @brief Shell entry point
 * @param args with flags data
 * @return psh::shell_exit_code
 */
ShellExitCode ShellMain(PshArguments *args);

} //namespace psh

/**
 * @brief Program entry point
 * @param argc argv count
 * @param argv arguments array
 * @return psh::shell_exit_code from shell_main()
 */
int main(int argc, const char **argv);

#endif //PHANTOMSHELL_PHANTOM_SHELL_H
