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

uint32 psh::parse_shell_args(psh_arguments_t *args, int argc, const char **argv) {
#ifndef __debug__
    args->debug_mode = 0;
#else //__debug__
    args->debug_mode = 1;
#endif //__debug__
    args->interactive_shell = 0;
    args->login_shell = 0;
    args->show_version = 0;
    args->show_usage = 0;
    args->output_stream = nullptr;
    args->input_stream = nullptr;
    for (int i = 0; i < argc; i++) {
        string arg_str = argv[i];
        DEBUG_LOG("string %s\n", arg_str.value());
        if (arg_str.starts_with("-")) {
            if (arg_str.equals("-d")) {
#ifndef __debug__
                if (args->debug_mode == 1)
                    return shell_exit_codes::EXIT_DUPLICATED_ARGUMENT;
#endif //__debug__
                args->debug_mode = 1;
                DEBUG_LOG("debug mode activated\n");
            } else if (arg_str.equals("-i")) {
                if (args->interactive_shell == 1)
                    return shell_exit_codes::EXIT_DUPLICATED_ARGUMENT;
                args->interactive_shell = 1;
                DEBUG_LOG("starting as interactive shell\n");
            } else if (arg_str.equals("-l")) {
                if (args->login_shell == 1)
                    return shell_exit_codes::EXIT_DUPLICATED_ARGUMENT;
                args->login_shell = 1;
                args->interactive_shell = 1;
                DEBUG_LOG("used as login shell\n");
            } else if (arg_str.equals("-h")) {
                if (args->show_usage == 1)
                    return shell_exit_codes::EXIT_DUPLICATED_ARGUMENT;
                args->show_usage = 1;
            } else if (arg_str.equals("-v")) {
                if (args->show_version == 1)
                    return shell_exit_codes::EXIT_DUPLICATED_ARGUMENT;
                args->show_version = 1;
            } else {
                DEBUG_LOG("flag cannot be recognized\n");
                return shell_exit_codes::EXIT_INVALID_ARGUMENTS;
            }
        } else {
            if (args->interactive_shell || args->login_shell) {
                DEBUG_LOG("interactive and login shell does not accept other parameters\n");
                return shell_exit_codes::EXIT_INVALID_ARGUMENTS;
            }
#ifdef __simbuild__
            if (arg_str.ends_with(".psh")) {
                DEBUG_LOG("found script location : %s\n", arg_str.value());
                args->input_stream = new istream(arg_str);
            }
#endif //__simbuild__
        }
    }
    if (args->output_stream == nullptr) {
        args->output_stream = new ostream();
    }
    return 0;
}

psh::shell_exit_codes psh::shell_show_usage(psh::psh_arguments_t *args) {
    psh::shell_show_version(args);
    const char *usage = "Usage: psh [options]\n"\
                        "       psh [options] script_file\n"\
                        "options:\n"\
                        "   -h: show this usage tutorial\n"\
                        "   -v: show shell version only\n"\
                        "   -i: start shell in interactive mode\n"\
                        "   -d: start shell in debug mode\n"\
                        "";
    phlib::string usage_str(usage);
    args->output_stream->write(usage_str.value(), usage_str.length());
    return shell_exit_codes::EXIT_NORMAL;
}

psh::shell_exit_codes psh::shell_show_version(psh::psh_arguments_t *args) {
    const char *version = PHANTOM_SHELL_VERSION "\n";
    phlib::string version_str(version);
    args->output_stream->write(version_str.value(), version_str.length());
    return shell_exit_codes::EXIT_NORMAL;
}

psh::shell_exit_codes psh::shell_main(psh_arguments_t *args) {
    return shell_exit_codes::EXIT_NORMAL;
}

int main(int argc, const char **argv) {
    psh::psh_arguments_t args;
    int res = psh::parse_shell_args(&args, argc, argv);
    if (res != 0) return res;
    if (args.show_usage){
        return psh::shell_show_usage(&args);
    }
    if (args.show_version){
        return psh::shell_show_version(&args);
    }
    return psh::shell_main(&args);
}
