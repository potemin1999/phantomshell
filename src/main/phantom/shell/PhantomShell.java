package phantom.shell;

import phantom.shell.parser.Scanner;
import phantom.support.io.InputStream;
import phantom.support.io.OutputStream;
import phantom.support.io.PrintStream;

public class PhantomShell {

    private PrintStream out;
    private Scanner scanner;

    public PhantomShell(InputStream sourceInput,InputStream ttyInput,OutputStream ttyOutput){
        out = new PrintStream(ttyOutput);
        scanner = new Scanner(sourceInput);
        out.println("phantom shell develop version");

    }

    public void run(){

    }



}
