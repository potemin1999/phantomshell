package phantom.support.lang;

public class ArrayOutOfBoundsException extends RuntimeException {

    public ArrayOutOfBoundsException(Throwable origin) {
        super(origin);
    }

    public ArrayOutOfBoundsException(String cause) {
        super(cause);
    }
}
