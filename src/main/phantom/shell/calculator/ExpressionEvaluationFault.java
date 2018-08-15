package phantom.shell.calculator;

import phantom.shell.executor.ExecutionFault;

public class ExpressionEvaluationFault extends ExecutionFault {

    public static final int INVALID_EXPRESSION = 2100;
    public static final int UNKNOWN_OBJ_TYPE = 2101;
    public static final int UNKNOWN_NUMBER_TYPE = 2102;
    public static final int UNDEFINED_INT_OPERATOR = 2103;
    public static final int UNDEFINED_FLOAT_OPERATOR = 2104;
    public static final int OBJ_STACK_WRONG_SIZE = 2105;
    public static final int INTEGER_DIVISION_BY_ZERO = 2110;

    private String msg;
    private int code;

    protected ExpressionEvaluationFault(int code) {
        super(code);
        this.code = code;
    }

    protected ExpressionEvaluationFault(int code, String msg) {
        this(code);
        this.msg = msg;
    }

    public int getCode() {
        return code;
    }

    @Override
    public String getDescription() {
        switch (code) {
            case INVALID_EXPRESSION: return "Invalid expression "+msg;
            case OBJ_STACK_WRONG_SIZE: return "Object stack had wrong size : "+msg;
            case UNKNOWN_OBJ_TYPE: return "Unknown object type";
            case UNKNOWN_NUMBER_TYPE: return "Unknown number type";
            case UNDEFINED_INT_OPERATOR: return "Operator "+msg+" is not defined for integers";
            case UNDEFINED_FLOAT_OPERATOR: return "Operator "+msg+" is not defined for floats";
            case INTEGER_DIVISION_BY_ZERO: return "Division by zero is prohibited";
            default: return "Unknown expression fault";
        }
    }
}
