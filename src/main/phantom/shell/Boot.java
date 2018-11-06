package phantom.shell;

import phantom.support.io.*;
import phantom.support.lang.RuntimeException;
import phantom.support.log.Log;

/**
 * @author Ilya Potemin
 *
 * Simple connection with outside world for PhantomShell
 *
 * @see PhantomShell
 */
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
        shell.setDebugMode(true);
    }

    public InputStream getStreamFromFile(String fileStr) {
        try {
            var file = new File(fileStr);
            return new FileInputStream(file);
        } catch (FileNotFoundException exception) {
            return null;
        }
    }

    private void run() {
        try {
            var startTime = System.currentTimeMillis();
            shell.run();
            var endTime = System.currentTimeMillis();
            Log.out.println("run time "+(endTime-startTime)+" ms");
        }catch (Throwable throwable){
            Log.out.println("shell run failed due to "+throwable.toString());
            Log.out.print(new RuntimeException(throwable));
        }

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
