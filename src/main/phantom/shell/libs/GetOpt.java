package main.phantom.shell.libs;

public class GetOpt {
    private static boolean finished = false;
    private static int argsind = 0;
    private static int argsends = -1;

    static String optarg = null;
    static int optind = 0;
    static private String current_arg = null;

    public static boolean remainArguments(String[] argv) {
        if (argsends == -1) {
            argsends = 0;
            for (int i = argv.length - 1; i >= 0 && (argsends == 0); i--) {
                if (argv[i].startsWith("-")) {
                    argsends = i + 1;
                }
            }
        }
        return optind < argsends;
    }

    public static int validArg(String opts, char charopt) {
        int at = opts.indexOf(charopt);
        if (at == -1) {
            return -1;
        }
        if (opts.indexOf(':', at) == at+1) {
            return 1;
        }
        return 0;
    }

    public static int findNextArg(String[] args) {
        int next = optind;
        for (; next < args.length; next++) {
            if (args[next].startsWith("-")) {
                return next;
            }
        }
        return 0;
    }

    public static int GetOpt(String[] argv, String opts) {
        if (remainArguments(argv)) {
            if (current_arg == null) {
                current_arg = argv[findNextArg(argv)];
            }
            if (current_arg.equals("--")) {
                current_arg = null;
                optind++;
                return -1;
            }
            current_arg = current_arg.substring(1);
            char optchar = current_arg.charAt(0);
            current_arg = current_arg.substring(1);
            if (current_arg.equals("")){
                current_arg = null;
                optind++;
            }
            switch (validArg(opts, optchar)) {
                case -1: {
                    System.out.printf("Invalid option -- %c\n", optchar);
                    return '?';
                }
                case 1: {
                    if (current_arg != null) {
                        optarg = current_arg;
                        return optchar;
                    }
                    if (optind < argv.length) {
                        optarg = argv[optind++];
                        return optchar;
                    }
                    System.out.printf("Option %c missing argument\n", optchar);
                    return '?';
                }
            }
            return optchar;

        } else {
            return -1;
        }
//        return 0;
    }
}