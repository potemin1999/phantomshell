package phantom.support.io;

import phantom.support.lang.RuntimeException;

public class PrintStream extends OutputStream {

    private OutputStream stream;

    public PrintStream(OutputStream stream) {
        super();
        this.stream = stream;
    }

    public void print() {
        System.out.print("");
    }

    public void write(byte[] bytes, int offset, int length) {
        stream.write(bytes, offset, length);
    }

    public void write(byte[] bytes) {
        stream.write(bytes);
    }

    public void write(int value) {
        stream.write(value);
    }

    public void flush() {
        stream.flush();
    }

    public void close() {
        stream.close();
    }

    private void newLine() {
        write((int) ('\n'));
        flush();
    }

    public void print(String str) {
        write(str.getBytes());
    }

    public void println(String str) {
        print(str);
        newLine();
    }

    public void println(){
        newLine();
    }

    public void print(RuntimeException exception) {
        StackTraceElement[] stackTraceElements = exception.getStackTrace();
        print(exception.getClass().getName());
        if (exception.getMessage() != null) {
            println(" : " + exception.getMessage());
        }else{
            println();
        }
        if (exception.getCause() != null)
            println("caused by " + exception.getCause());
        for (StackTraceElement ste : stackTraceElements) {
            println("   at " + ste.getClassName() + "." + ste.getMethodName() + "(" + ste.getFileName() + ":" + ste.getLineNumber() + ")");
        }
    }


}
