package phantom.shell.interpreter.printer;

import phantom.support.io.PrintStream;

public class Printer {

    private PrintStream out;

    public Printer(PrintStream out) {
        this.out = out;
    }

    public void print(Object toPrint) {
        out.print(toPrint);
    }

    public void println(Object toPrint) {
        out.println(toPrint);
    }
}
