package phantom.shell.parser;

import phantom.support.io.File;
import phantom.support.io.FileInputStream;
import phantom.support.io.FileNotFoundException;
import phantom.support.io.InputStream;
import phantom.support.log.Log;

public class Scanner {

    private InputStream stdIn;
    private boolean isInteractiveMode = false;
    private char[] buffer = new char[16];
    private int bufferCurrentPointer = 0;
    private int bufferEndPointer = 0;

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

    public void setInteractiveMode(boolean is) {
        isInteractiveMode = is;
    }

    public boolean isAvailable() {
        return (stdIn.available() + (bufferEndPointer - bufferCurrentPointer)) > 0;
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
        if (bufferCurrentPointer < bufferEndPointer) {
            var ret = buffer[bufferCurrentPointer % buffer.length];
            bufferCurrentPointer++;
            return ret;
        } else {
            var ret = readNextChar();
            buffer[bufferEndPointer % buffer.length] = ret;
            bufferEndPointer++;
            bufferCurrentPointer++;
            return ret;
        }
    }

    public char prev() {
        bufferCurrentPointer--;
        return buffer[bufferCurrentPointer % buffer.length];
    }

    public char peek() {
        if (bufferCurrentPointer < bufferEndPointer) {
            var ret = buffer[(bufferCurrentPointer) % buffer.length];
            return ret;
        } else {
            if (stdIn.available() > (isInteractiveMode ? -1 : 0)) {
                var ret = readNextChar();
                buffer[(bufferCurrentPointer) % buffer.length] = ret;
                bufferEndPointer++;
                return ret;
            } else {
                return '\0';
            }
        }
    }

    public void skip() {
        if (bufferCurrentPointer == bufferEndPointer) {
            buffer[bufferEndPointer % buffer.length] = readNextChar();
            bufferEndPointer++;
        }
        bufferCurrentPointer++;
    }

}
