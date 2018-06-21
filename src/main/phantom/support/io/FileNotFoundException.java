package phantom.support.io;

import phantom.support.lang.RuntimeException;

public class FileNotFoundException extends RuntimeException {

    public FileNotFoundException(Throwable origin) {
        super(origin);
    }

    public FileNotFoundException(String cause) {
        super(cause);
    }
}
