package phantom.shell.expressionEvaluator;

import phantom.support.lang.RuntimeException;

public class ExpressionEvaluationException extends RuntimeException {

    public ExpressionEvaluationException(Throwable origin) {
        super(origin);
    }

    public ExpressionEvaluationException(String cause) {
        super(cause);
    }

}
