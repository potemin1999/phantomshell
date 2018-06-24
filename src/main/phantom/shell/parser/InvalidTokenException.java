package phantom.shell.parser;

import phantom.support.lang.RuntimeException;

public class InvalidTokenException extends RuntimeException {
    public InvalidTokenException(String message) {
        super(message);
    }
}
