package phantom.shell.interpreter.calculator;

import phantom.shell.values.Value;

public class ExpressionToken {

    public static final int OPTYPE_OBJECT = 0;
    public static final int OPTYPE_UNARY_OPERATOR = 1;
    public static final int OPTYPE_BINARY_OPERATOR = 2;
    public static final int OPTYPE_COMPARISON_OPERATOR = 3;
    public static final int OPTYPE_LOGICAL_OPERATOR = 4;
    public static final int OPTYPE_PRIORITY_OPERATOR = 5;
    public static final int OPTYPE_ASSIGN_OPERATOR = 6;
    public static final int OPTYPE_INCDEC_OPERATOR = 7;

    private int opType;
    private int opCode;
    private Value value;

    public ExpressionToken(int opType,int operator){
        this.opType = opType;
        this.opCode = operator;
    }

    public ExpressionToken(int opType, Value value){
        this.opType = opType;
        this.value = value;
    }

    public int getOpType() {
        return opType;
    }

    public Value getValue() {
        return value;
    }

    public int getOpCode() {
        return opCode;
    }

    @Override
    public String toString() {
        if (value==null){
            return "ExpressionToken["+opType+"]("+opCode+")";
        }else{
            return "ExpressionToken["+opType+"]("+value+")";
        }
    }
}
