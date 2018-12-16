/**
 * @file
 * @author Ilya Potemin
 * @date 11/6/18.
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "lexer.h"
#include "parser.h"
#include "phantom_shell.h"
#include "alloc.h"
#include "string.h"
#include "log.h"

using namespace psh;
using namespace phlib;

PhantomShell::PhantomShell(PshArguments *args) {
    psh_arguments = args;
    lexer         = new Lexer(args->input_stream);
}


PhantomShell::~PhantomShell() {
    delete lexer;
}


ShellExitCode PhantomShell::run() {
    while (lexer != nullptr) {
        Token *token = lexer->get_next_token();
        if (token == nullptr) break;
#ifdef __debug__
        DEBUG_LOG("%s\n", token->token_to_string());
#endif
    }
    return ShellExitCode::EXIT_NORMAL;
}


uint32 psh::parse_shell_short_options(psh::PshArguments *args, const char *option) {
    if (option[0] != '-') return 1;
    switch (option[1]) {
        case 'd': {
#ifndef __debug__
            if (args->debug_mode == 1)
                    return ShellExitCode::EXIT_DUPLICATED_ARGUMENT;
#endif //__debug__
            args->debug_mode = 1;
            DEBUG_LOG("debug mode activated\n");
            break;
        }
        case 'i': {
            if (args->interactive_shell == 1)
                return ShellExitCode::EXIT_DUPLICATED_ARGUMENT;
            args->interactive_shell = 1;
            DEBUG_LOG("starting as interactive shell\n");
            break;
        }
        case 'l': {
            if (args->login_shell == 1)
                return ShellExitCode::EXIT_DUPLICATED_ARGUMENT;
            args->login_shell       = 1;
            args->interactive_shell = 1;
            DEBUG_LOG("used as login shell\n");
            break;
        }
        case 'h': {
            if (args->show_usage == 1)
                return ShellExitCode::EXIT_DUPLICATED_ARGUMENT;
            args->show_usage = 1;
            break;
        }
        case 'v': {
            if (args->show_version == 1)
                return ShellExitCode::EXIT_DUPLICATED_ARGUMENT;
            args->show_version = 1;
            break;
        }
        default: {
            DEBUG_LOG("flag cannot be recognized\n");
            return ShellExitCode::EXIT_INVALID_ARGUMENTS;
        }
    }
    return 0;
}


uint32 psh::parse_shell_long_options(psh::PshArguments *args, const char *option) {
    //TODO: implement
    return 0;
}


uint32 psh::parse_shell_args(PshArguments *args, int argc, const char **argv) {
#ifndef __debug__
    args->debug_mode = 0;
#else //__debug__
    args->debug_mode = 1;
#endif //__debug__
    args->interactive_shell = 0;
    args->login_shell       = 0;
    args->show_version      = 0;
    args->show_usage        = 0;
    args->output_stream     = nullptr;
    args->input_stream      = nullptr;
    for (int i = 1; i < argc; i++) {
        String arg_str = argv[i];
        DEBUG_LOG("string %s\n", arg_str.char_value());
        if (arg_str.starts_with("--")) {
            parse_shell_long_options(args, argv[i]);
        } else if (arg_str.starts_with("-")) {
            parse_shell_short_options(args, argv[i]);
        } else {
            if (args->interactive_shell || args->login_shell) {
                DEBUG_LOG("interactive and login shell does not accept other parameters\n");
                return ShellExitCode::EXIT_INVALID_ARGUMENTS;
            }
#ifdef __simbuild__
            if (arg_str.ends_with(".psh")) {
                DEBUG_LOG("found script location : %s\n", arg_str.char_value());
                args->input_stream = new IStream(arg_str);
                break;
            } else {
                DEBUG_LOG("invalid script file : %s\n", arg_str.char_value());
                return ShellExitCode::EXIT_INVALID_ARGUMENTS;
            }
#endif //__simbuild__
        }
    }
    if (args->input_stream == nullptr) {
        args->input_stream = new IStream();
    }
    if (args->output_stream == nullptr) {
        args->output_stream = new OStream();
    }
    return 0;
}


uint32 psh::cleanup_shell_args(psh::PshArguments *args) {
    delete args->input_stream;
    delete args->output_stream;
}


psh::ShellExitCode psh::shell_show_usage(PshArguments *args) {
    psh::shell_show_version(args);
    const char *usage = "Usage: psh [options]\n"\
                        "       psh [options] script_file\n"\
                        "options:\n"\
                        "   -h: show this usage tutorial\n"\
                        "   -v: show shell version only\n"\
                        "   -i: start shell in interactive mode\n"\
                        "   -d: start shell in debug mode\n"\
                        "";
    String     usage_str(usage);
    args->output_stream->write(usage_str.value(), usage_str.length());
    return ShellExitCode::EXIT_NORMAL;
}


psh::ShellExitCode psh::shell_show_version(PshArguments *args) {
    String version_str = PHANTOM_SHELL_VERSION "\n";
    args->output_stream->write(version_str.value(), version_str.length());
    return ShellExitCode::EXIT_NORMAL;
}


psh::ShellExitCode psh::shell_main(PshArguments *args) {
    PhantomShell  shell(args);
    ShellExitCode exit_code = shell.run();
    cleanup_shell_args(args);
    return exit_code;
}


int main(int argc, const char **argv) {
    psh::PshArguments args;
    int               res = psh::parse_shell_args(&args, argc, argv);
    if (res != 0) return res;
    if (args.show_usage) {
        return psh::shell_show_usage(&args);
    }
    if (args.show_version) {
        return psh::shell_show_version(&args);
    }
    return psh::shell_main(&args);
}
