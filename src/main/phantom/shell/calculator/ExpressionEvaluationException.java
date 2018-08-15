package phantom.shell.calculator;

import phantom.shell.executor.ExecutionFault;
import phantom.shell.executor.ExecutionFaultException;

public class ExpressionEvaluationException extends ExecutionFaultException {

    private int code;
    private String msg;

    public ExpressionEvaluationException(int code) {
        super(code);
        this.code = code;
    }

    public ExpressionEvaluationException(int code, String msg) {
        this(code);
        this.msg = msg;
    }

    public ExpressionEvaluationException(Throwable origin) {
        super(origin);
    }

    public ExpressionEvaluationException(String cause) {
        super(-1);
    }

    @Override
    public ExecutionFault makeFault() {
        if (msg == null) {
            return new ExpressionEvaluationFault(code);
        } else {
            return new ExpressionEvaluationFault(code, msg);
        }
    }
}
