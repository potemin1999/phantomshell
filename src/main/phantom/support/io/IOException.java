package phantom.support.io;

import phantom.support.lang.RuntimeException;

public class IOException extends RuntimeException {

    public IOException(Throwable origin) {
        super(origin);
    }

    public IOException(String cause) {
        super(cause);
    }
}
