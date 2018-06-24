package phantom.shell.calculator;

import phantom.support.lang.RuntimeException;

public class ExpressionEvaluationException extends RuntimeException {

    public ExpressionEvaluationException(Throwable origin) {
        super(origin);
    }

    public ExpressionEvaluationException(String cause) {
        super(cause);
    }

}
