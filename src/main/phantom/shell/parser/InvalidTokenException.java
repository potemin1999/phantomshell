package phantom.shell.parser;

import phantom.support.lang.RuntimeException;

public class InvalidTokenException extends RuntimeException {

    private int code = -1;

    public InvalidTokenException(String message) {
        super(message);
    }

    public InvalidTokenException(int code){
        super(code+"");
        this.code = code;
    }

}
