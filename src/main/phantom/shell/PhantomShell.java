package phantom.shell;

import phantom.shell.executor.ExecutionFault;
import phantom.shell.executor.Executor;
import phantom.shell.expressions.Expression;
import phantom.shell.parser.*;
import phantom.shell.printer.Printer;
import phantom.support.io.InputStream;
import phantom.support.io.OutputStream;
import phantom.support.io.PrintStream;
import phantom.support.log.Log;

/**
 * @author Ilya Potemin
 * @author Bogdan Fedotov
 * @author Michail Bobrov
 * @author Salavat Dinmukhametov
 * @author Gayaz Kamaletdinov
 */
public class PhantomShell {

    private InputStream sourceInput;
    private InputStream ttyInput;
    private OutputStream ttyOutput;

    private PrintStream out;
    private Scanner scanner;

    private Lexer lexer;
    private Parser parser;
    private Executor executor;
    private Printer printer;

    private boolean isInDebugMode = false;


    public PhantomShell(InputStream sourceInput, InputStream ttyInput, OutputStream ttyOutput) {
        this.sourceInput = sourceInput;
        this.ttyInput = ttyInput;
        this.ttyOutput = ttyOutput;

        out = new PrintStream(ttyOutput);
        printer = new Printer(out);
        scanner = new Scanner(sourceInput);
        lexer = new Lexer(scanner);
        parser = new Parser(lexer);
        executor = new Executor(lexer, out);

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
            Expression expression = parser.next();
            if (isInDebugMode) {
                out.println(expression.toString());
            }

            var result = (isInDebugMode ? executor.executeUnsafe(expression)
                                        : executor.executeSafe(expression));
            if (result instanceof ExecutionFault){
                Log.out.println(result.toString());
            }
        }
    }


}
