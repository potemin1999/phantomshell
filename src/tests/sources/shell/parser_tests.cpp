/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 2/24/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include <cstring>
#include "IStream.h"
#include "Lexer.h"
#include "Parser.h"

using namespace phlib;
using namespace psh;

int test1() {
    const char *source = "1+2/4";
    IStream istream = IStream(source, strlen(source), false);
    Lexer   lexer   = Lexer(&istream);
    Parser  parser  = Parser();
    for (Token *token; (token = lexer.getNextToken()) != nullptr;) {
        parser.pushToken(token);
    }
    return 0;
}

int main(int argc, const char **argv) {
    int ret = 0;
    if ((ret = test1()) != 0) return ret;
    return ret;
}