package phantom.shell;

import phantom.support.io.*;

public class Boot {

    public static void main(String[] args) {
        new Boot(args).run();
    }

    private PhantomShell shell;

    private Boot(String[] args) {
        var ttyInput = new InputStream(System.in);
        var ttyOutput = new OutputStream(System.out);
        InputStream sourceInput;
        if (isInteractiveModeEnabled(args)) {
            sourceInput = ttyInput;
        } else {
            sourceInput = getStreamFromFile(args[0]);
            if (sourceInput == null) {
                ttyOutput.write("file not found, exiting\n".getBytes());
                System.exit(-1);
            }
        }
        shell = new PhantomShell(sourceInput, ttyInput, ttyOutput);
    }

    public InputStream getStreamFromFile(String fileStr) {
        try {
            File file = new File(fileStr);
            return new FileInputStream(file);
        } catch (FileNotFoundException exception) {
            return null;
        }
    }

    private void run() {
        shell.run();
    }

    private boolean isInteractiveModeEnabled(String[] args) {
        return args.length == 0 || containsFlags(args, "-i");
    }

    private boolean isSilentModeEnabled(String[] args) {
        return args.length > 0 && containsFlags(args, "-q");
    }

    private boolean containsFlags(String[] args, String flag) {
        for (String arg : args) {
            if (arg.trim().equalsIgnoreCase(flag))
                return true;
        }
        return false;
    }
}
