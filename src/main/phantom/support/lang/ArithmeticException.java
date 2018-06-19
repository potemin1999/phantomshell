package phantom.support.lang;

public class ArithmeticException extends RuntimeException {

    public ArithmeticException(Throwable origin) {
        super(origin);
    }

    public ArithmeticException(String cause) {
        super(cause);
    }

}
