package phantom.support.lang;

public class NullPointerException extends RuntimeException {

    public NullPointerException(Throwable origin) {
        super(origin);
    }

    public NullPointerException(String cause) {
        super(cause);
    }

}
