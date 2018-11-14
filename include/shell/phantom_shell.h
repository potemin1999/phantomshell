/**
 * Created by Ilya Potemin on 11/6/18.
 * Part of Phantom Shell project.
 */

#ifndef PHANTOMSHELL_PHANTOM_SHELL_H
#define PHANTOMSHELL_PHANTOM_SHELL_H

namespace psh {

typedef struct {
    unsigned debug_mode: 1;
    unsigned interactive_shell: 1;
    unsigned login_shell: 1;
} psh_flags_t;

/**
 * parse shell flags from program startup arguments
 * @param flags struct to write into
 * @param argc arguments count, including program name
 * @param argv arguments - const char[][]
 * @return 0 if parsing was succeed
 */
int parse_shell_flags(psh_flags_t *flags, int argc, const char **argv);

/**
 * shell entry point
 * @param flags with flags data
 * @return exit code, usually
 */
int shell_main(psh_flags_t *flags);

} //namespace psh

/**
 * program entry point
 * @param argc argv count
 * @param argv arguments array
 * @return shell_main() value
 */
int main(int argc, const char **argv);

#endif //PHANTOMSHELL_PHANTOM_SHELL_H
