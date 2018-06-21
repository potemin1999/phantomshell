package phantom.shell;

import phantom.shell.parser.Lexer;
import phantom.shell.parser.Scanner;
import phantom.shell.parser.Token;
import phantom.shell.parser.TokenType;
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

    private boolean isInDebugMode = false;


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
            scanner.setInteractiveMode(true);
        } else {
            out.println("non-interactive run");
        }
    }

    public void setDebugMode(boolean debug) {
        isInDebugMode = debug;
    }

    //TODO: DO DO DO
    public void run() {
        while (lexer.hasNotReachedEOF()) {
            Token token = lexer.next();
            if (isInDebugMode) {
                out.print(token.toString());
                if (token.getType() == TokenType.EOL)
                    out.println();
            }
        }
    }


}
