package phantom.shell.executor;

import phantom.support.lang.RuntimeException;

public class ExecutionFaultException extends RuntimeException {

    private int code;

    public ExecutionFaultException(Throwable origin) {
        super(origin);
    }

    public ExecutionFaultException(int code) {
        super(code+"");
        this.code = code;
    }

    public int getCode(){
        return code;
    }

    public ExecutionFault makeFault(){
        return new ExecutionFault(code);
    }

}
