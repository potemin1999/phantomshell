/**
 * @file
 * @author Ilya Potemin
 * @date 11/6/18.
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "Lexer.h"
#include "Parser.h"
#include "PhantomShell.h"
#include "Alloc.h"
#include "String.h"
#include "Log.h"

using namespace psh;
using namespace phlib;

PhantomShell::PhantomShell(PshArguments *args) {
    pshArguments = args;
    lexer        = new Lexer(args->inputStream);
    parser       = new Parser();
}


PhantomShell::~PhantomShell() {
    delete lexer;
    delete parser;
}


ShellExitCode PhantomShell::run() {
    while (lexer != nullptr) {
        Token *token = lexer->getNextToken();
        if (token == nullptr) break;
#ifdef __debug__
        DEBUG_LOG("%s\n", token->tokenToString());
#endif
        parser->pushToken(token);
    }
    return ShellExitCode::EXIT_NORMAL;
}


UInt32 psh::parseShellShortOptions(psh::PshArguments *args, const char *option) {
    if (option[0] != '-') return 1;
    switch (option[1]) {
        case 'd': {
#ifndef __debug__
            if (args->debugMode == 1)
                    return ShellExitCode::EXIT_DUPLICATED_ARGUMENT;
#endif //__debug__
            args->debugMode = 1;
            DEBUG_LOG("debug mode activated\n");
            break;
        }
        case 'i': {
            if (args->interactiveShell == 1)
                return ShellExitCode::EXIT_DUPLICATED_ARGUMENT;
            args->interactiveShell = 1;
            DEBUG_LOG("starting as interactive shell\n");
            break;
        }
        case 'l': {
            if (args->loginShell == 1)
                return ShellExitCode::EXIT_DUPLICATED_ARGUMENT;
            args->loginShell       = 1;
            args->interactiveShell = 1;
            DEBUG_LOG("used as login shell\n");
            break;
        }
        case 'h': {
            if (args->showUsage == 1)
                return ShellExitCode::EXIT_DUPLICATED_ARGUMENT;
            args->showUsage = 1;
            break;
        }
        case 'v': {
            if (args->showVersion == 1)
                return ShellExitCode::EXIT_DUPLICATED_ARGUMENT;
            args->showVersion = 1;
            break;
        }
        default: {
            DEBUG_LOG("flag cannot be recognized\n");
            return ShellExitCode::EXIT_INVALID_ARGUMENTS;
        }
    }
    return 0;
}


UInt32 psh::parseShellLongOptions(psh::PshArguments *args, const char *option) {
    //TODO: implement
    return 0;
}


UInt32 psh::parseShellArgs(PshArguments *args, int argc, const char **argv) {
#ifndef __debug__
    args->debugMode = 0;
#else //__debug__
    args->debugMode = 1;
#endif //__debug__
    args->interactiveShell = 0;
    args->loginShell       = 0;
    args->showVersion      = 0;
    args->showUsage        = 0;
    args->outputStream     = nullptr;
    args->inputStream      = nullptr;
    for (int i = 1; i < argc; i++) {
        String argStr = argv[i];
        DEBUG_LOG("string %s\n", argStr.charValue());
        if (argStr.startsWith("--")) {
            parseShellLongOptions(args, argv[i]);
        } else if (argStr.startsWith("-")) {
            parseShellShortOptions(args, argv[i]);
        } else {
            if (args->interactiveShell || args->loginShell) {
                DEBUG_LOG("interactive and login shell does not accept other parameters\n");
                return ShellExitCode::EXIT_INVALID_ARGUMENTS;
            }
#ifdef __simbuild__
            if (argStr.endsWith(".psh")) {
                DEBUG_LOG("found script location : %s\n", argStr.charValue());
                args->inputStream = new IStream(argStr);
                break;
            } else {
                DEBUG_LOG("invalid script file : %s\n", argStr.charValue());
                return ShellExitCode::EXIT_INVALID_ARGUMENTS;
            }
#endif //__simbuild__
        }
    }
    if (args->inputStream == nullptr) {
        args->inputStream = new IStream();
    }
    if (args->outputStream == nullptr) {
        args->outputStream = new OStream();
    }
    return 0;
}


UInt32 psh::cleanupShellArgs(psh::PshArguments *args) {
    delete args->inputStream;
    delete args->outputStream;
}


psh::ShellExitCode psh::shellShowUsage(PshArguments *args) {
    psh::shellShowVersion(args);
    const char *usage = "Usage: psh [options]\n"\
                        "       psh [options] script_file\n"\
                        "options:\n"\
                        "   -h: show this usage tutorial\n"\
                        "   -v: show shell version only\n"\
                        "   -i: start shell in interactive mode\n"\
                        "   -d: start shell in debug mode\n"\
                        "";
    String     usageStr(usage);
    args->outputStream->write(usageStr.value(), usageStr.length());
    return ShellExitCode::EXIT_NORMAL;
}


psh::ShellExitCode psh::shellShowVersion(PshArguments *args) {
    String versionStr = PHANTOM_SHELL_VERSION "\n";
    args->outputStream->write(versionStr.value(), versionStr.length());
    return ShellExitCode::EXIT_NORMAL;
}


psh::ShellExitCode psh::shellMain(PshArguments *args) {
    PhantomShell  shell(args);
    ShellExitCode exitCode = shell.run();
    cleanupShellArgs(args);
    return exitCode;
}


int main(int argc, const char **argv) {
    psh::PshArguments args;
    int               res = psh::parseShellArgs(&args, argc, argv);
    if (res != 0) return res;
    if (args.showUsage) {
        return psh::shellShowUsage(&args);
    }
    if (args.showVersion) {
        return psh::shellShowVersion(&args);
    }
    return psh::shellMain(&args);
}
