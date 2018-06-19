package phantom.shell.parser;

import phantom.support.io.File;
import phantom.support.io.FileInputStream;
import phantom.support.io.FileNotFoundException;
import phantom.support.io.InputStream;
import phantom.support.log.Log;

public class Scanner {

    private InputStream stdIn;
    private int bufferedValue = -1;

    public Scanner(String fullFileName) {
        try {
            var file = new File(fullFileName);
            stdIn = new FileInputStream(file);
        } catch (FileNotFoundException ex) {
            Log.debug.print(ex);
        }
    }

    public Scanner(InputStream inputStream) {
        stdIn = inputStream;
    }

    public boolean isAvailable() {
        return stdIn.available() > 0;
    }

    private char readNextChar() {
        int res;
        do {
            res = stdIn.read();
        } while (res == '\r');
        if (res < 192)
            return (char) (res);
        if (res < 224) {
            int b2 = stdIn.read() & 0b00111111;
            res = res & 0b00011111;
            return (char) ((res << 6) + b2);
        }
        if (res < 240) {
            int b2 = stdIn.read() & 0b00111111;
            int b3 = stdIn.read() & 0b00111111;
            res = res & 0b00001111;
            return (char) ((res << 12) + (b2 << 6) + b3);
        }
        int b2 = stdIn.read() & 0b00111111;
        int b3 = stdIn.read() & 0b00111111;
        int b4 = stdIn.read() & 0b00111111;
        res = res & 0b00000111;
        return (char) ((res << 18) + (b2 << 12) + (b3 << 6) + b4);

    }

    public char next() {
        if (bufferedValue == -1) {
            if (stdIn.available() < 0)
                return (char) (-1);
            return readNextChar();
        } else {
            var ret = (char) bufferedValue;
            bufferedValue = -1;
            return ret;
        }
    }

    public char peek() {
        if (stdIn.available() < 0)
            return (char) (-1);
        if (bufferedValue == -1)
            bufferedValue = readNextChar();
        return (char) bufferedValue;
    }

    public void skip() {
        readNextChar();
    }

}
