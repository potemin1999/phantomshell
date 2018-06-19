package phantom.support.lang;

public class RuntimeException extends java.lang.NullPointerException {

    private Throwable throwable;
    private String cause;
    private StackTraceElement[] stackTrace;

    public RuntimeException(Throwable origin){
        throwable = origin;
        stackTrace = origin.getStackTrace();
    }

    public RuntimeException(String cause){
        this.cause = cause;
        stackTrace = Thread.currentThread().getStackTrace();
    }

    public String getMessage(){
        if (throwable!=null){
            return throwable.getMessage();
        }else{
            return cause;
        }
    }
    public Throwable getCause(){
        return throwable;
    }

    public StackTraceElement[] getStackTrace(){
        return stackTrace;
    }
}
