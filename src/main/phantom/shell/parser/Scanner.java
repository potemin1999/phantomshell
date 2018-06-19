package phantom.shell.parser;

import phantom.support.io.File;
import phantom.support.io.FileInputStream;
import phantom.support.io.FileNotFoundException;
import phantom.support.io.InputStream;
import phantom.support.log.Log;

public class Scanner {

    private InputStream stdIn;
    private int bufferedValue;

    public Scanner(String fullFileName){
        try {
            var file = new File(fullFileName);
            stdIn = new FileInputStream(file);
        }catch(FileNotFoundException ex){
            Log.debug.print(ex);
        }
    }

    public Scanner(InputStream inputStream){
        stdIn = inputStream;
    }

    public char next(){
        if (bufferedValue==-1){
            return (char) stdIn.read();
        }else{
            var ret = (char) bufferedValue;
            bufferedValue = -1;
            return ret;
        }
    }

    public char peek(){
        if (bufferedValue==-1)
            bufferedValue = stdIn.read();
        return (char)  bufferedValue;
    }

    public void skip(){
        stdIn.read();
    }

}
