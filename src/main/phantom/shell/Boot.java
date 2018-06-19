package phantom.shell;

import phantom.support.io.InputStream;
import phantom.support.io.OutputStream;

public class Boot {

    public static void main(String[] args){
        var input = new InputStream(System.in);
        var output = new OutputStream(System.out);

        var phantomShell = new PhantomShell(input,output);
        phantomShell.run();
    }
}
