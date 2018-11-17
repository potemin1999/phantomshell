/**
 * @file
 * @author Ilya Potemin
 * @date 11/6/18.
 * @section LICENSE
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include <shell/lexer.h>
#include <shell/parser.h>
#include <shell/phantom_shell.h>
#include <phlib/alloc.h>
#include <phlib/string.h>
#include <phlib/log.h>

/**
 * Workaround for linking without -lstdc++ flag
 * It will work until first exception, I'm afraid
 */
void *__gxx_personality_v0;

using namespace phlib;

int psh::parse_shell_flags(psh_arguments_t *flags, int argc, const char **argv) {
#ifndef __debug__
    flags->debug_mode = 0;
#else //__debug__
    flags->debug_mode = 1;
#endif //__debug__
    flags->interactive_shell = 0;
    flags->login_shell = 0;
    for (int i = 0; i < argc; i++) {
        string arg_str = argv[i];
        DEBUG_LOG("string %s\n", arg_str.value());
        if (arg_str.starts_with("-")) {
            if (arg_str.equals("-d")) {
#ifndef __debug__
                if (flags->debug_mode == 1)
                    return shell_exit_codes::EXIT_DUPLICATED_ARGUMENT;
#endif //__debug__
                flags->debug_mode = 1;
                DEBUG_LOG("debug mode activated\n");
            } else if (arg_str.equals("-i")) {
                if (flags->interactive_shell == 1)
                    return shell_exit_codes::EXIT_DUPLICATED_ARGUMENT;
                flags->interactive_shell = 1;
                DEBUG_LOG("starting as interactive shell\n");
            } else if (arg_str.equals("-l")) {
                if (flags->login_shell == 1)
                    return shell_exit_codes::EXIT_DUPLICATED_ARGUMENT;
                flags->login_shell = 1;
                flags->interactive_shell = 1;
                DEBUG_LOG("used as login shell\n");
            } else {
                DEBUG_LOG("flag cannot be recognized\n");
                return shell_exit_codes::EXIT_INVALID_ARGUMENTS;
            }
        } else {
            if (flags->interactive_shell || flags->login_shell) {
                DEBUG_LOG("interactive and login shell does not accept other parameters\n");
                return shell_exit_codes::EXIT_INVALID_ARGUMENTS;
            }
#ifdef __simbuild__
            if (arg_str.ends_with(".psh")) {
                DEBUG_LOG("found script location : %s\n", arg_str.value());
                flags->input_stream = new istream(arg_str);
            }
#endif //__simbuild__
        }
    }
    return 0;
}

int psh::shell_main(psh_arguments_t *flags) {
    return shell_exit_codes::EXIT_NORMAL;
}

int main(int argc, const char **argv) {
    psh::psh_arguments_t shell_flags;
    int res = psh::parse_shell_flags(&shell_flags, argc, argv);
    if (res != 0) return res;
    return psh::shell_main(&shell_flags);
}
