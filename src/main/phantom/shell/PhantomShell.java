package phantom.shell;

import phantom.shell.parser.Lexer;
import phantom.shell.parser.Scanner;
import phantom.support.io.InputStream;
import phantom.support.io.OutputStream;
import phantom.support.io.PrintStream;

public class PhantomShell {

    private InputStream sourceInput;
    private InputStream ttyInput;
    private OutputStream ttyOutput;

    private PrintStream out;
    private Scanner scanner;
    private Lexer lexer;


    public PhantomShell(InputStream sourceInput, InputStream ttyInput, OutputStream ttyOutput) {
        this.sourceInput = sourceInput;
        this.ttyInput = ttyInput;
        this.ttyOutput = ttyOutput;

        out = new PrintStream(ttyOutput);
        scanner = new Scanner(sourceInput);
        lexer = new Lexer(scanner);
        out.println("phantom shell develop version");
        if (sourceInput == ttyInput) {
            out.println("interactive run");
        } else {
            out.println("non-interactive run");
        }
    }


    //TODO: DO DO DO
    public void run() {
        while (scanner.isAvailable()) {
            char c = scanner.next();
            out.print(":"+c+"");
        }
    }


}
