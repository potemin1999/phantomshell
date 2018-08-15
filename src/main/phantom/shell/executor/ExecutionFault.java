package phantom.shell.executor;

public class ExecutionFault {

    public static final int DEF_TWICE = 2000;
    public static final int EVAL_OPSTACK_FAULT = 2001;

    private int code = -1;
    private Throwable throwable;

    protected ExecutionFault(int code) {
        this.code = code;
    }

    protected ExecutionFault(int code, Throwable throwable) {
        this(code);
        this.throwable = throwable;
    }

    public String getDescription() {
        switch (code) {
            case DEF_TWICE:
                return "Variable with the same name is already defined in this scope";
            case EVAL_OPSTACK_FAULT:
                return "Execution of sequence to evaluate has failed";
            default:
                return "Unknown execution fault";
        }
    }


    public String toString() {
        if (throwable == null) {
            if (code != -1) {
                return "EXEC_FAULT " + code + " : " + getDescription();
            } else {
                return "EXEC_FAULT : " + code;
            }
        } else {
            StackTraceElement element = throwable.getStackTrace()[0];
            return "EXEC_FAULT : " + element.getClassName() + "." + element.getMethodName()
                    + "(line:" + element.getLineNumber() + ") : " + throwable.getMessage();
        }
    }
}

