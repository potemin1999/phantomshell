package phantom.support.log;

import phantom.support.io.OutputStream;
import phantom.support.io.PrintStream;

public class Log {

    public static final PrintStream out =
            new PrintStream(new OutputStream(
                    System.out
            ));

    public static final PrintStream debug =
            new PrintStream(new OutputStream(
                    System.err
            ));

}
