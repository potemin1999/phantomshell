package io;

import phantom.support.io.OutputStream;
import phantom.support.io.PrintStream;
import phantom.support.lang.NullPointerException;

public class OutputStreamTest {

    public static void main(String[] args){
        PrintStream out = new PrintStream(new OutputStream(System.out));
        out.print("simple string;");
        out.println("simple line, exception next;");
        out.print(new NullPointerException(new ArrayIndexOutOfBoundsException("0_0")));
    }


}
