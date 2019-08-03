/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 7/31/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */
#include "types.h"
#include "y.tab.h"

int main(int argc, const char **argv) {
    yyparse();
}
