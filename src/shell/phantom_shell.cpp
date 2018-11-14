/**
 * Created by Ilya Potemin on 11/6/18.
 * Part of Phantom Shell project.
 */

#include <shell/phantom_shell.h>
#include <phlib/alloc.h>

using namespace phlib;

int psh::parse_shell_flags(psh_flags_t *flags, int argc, const char **argv) {
    flags->debug_mode = 0;
    flags->interactive_shell = 1;
    //TODO: parse configuration from arguments
    return 0;
}

int psh::shell_main(psh_flags_t *flags) {
    return 0;
}

int main(int argc, const char **argv) {
    psh::psh_flags_t shell_flags;
    psh::parse_shell_flags(&shell_flags, argc, argv);
    return psh::shell_main(&shell_flags);
}
