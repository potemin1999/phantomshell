package phantom.shell.environment;

import phantom.support.lang.RuntimeException;

public class EnvironmentException extends RuntimeException {

    public EnvironmentException(Throwable origin) {
        super(origin);
    }

    public EnvironmentException(String cause) {
        super(cause);
    }

}
