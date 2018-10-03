package phantom.shell.structures;

import phantom.shell.values.Value;

import java.util.HashMap;

/**
 * @author Bogdan Fedotov
 * @author Ilya Potemin
 *
 * New, fast and revorked functionality for operators
 *
 * DO NOT AUTO-FORMAT!
 */
public class Operator {

    /**
     *  0000   0000  00000000
     *  type priority    id;
     *
     *  Types:
     *   0 - increment decrement operators
     *   1 - unary operators
     *   2 - binary operators
     *   3 - priority operators
     *   4 - comparison operators
     *   5 - logical operators
     *   15 - reserved
     */
    public static final int INCREMENT           = 0x0000; // ++
    public static final int DECREMENT           = 0x0001; // --
    public static final int ASSIGNING           = 0x2f10; // =
    public static final int ADDITION            = 0x2620; // +
    public static final int SUBTRACTION         = 0x2621; // -
    public static final int MULTIPLICATION      = 0x2522; // *
    public static final int DIVISION            = 0x2523; // /
    public static final int BITWISE_NOT         = 0x1230; // ~
    public static final int BITWISE_AND         = 0x2331; // /\
    public static final int BITWISE_OR          = 0x2432; // \/
    public static final int BITWISE_XOR         = 0x2433; // \'/
    public static final int BITWISE_SHIFT_L     = 0x2134; // <<
    public static final int BITWISE_SHIFT_R     = 0x2135; // >>
    //TODO: add/ignore priority of comparison operators
    public static final int EQUAL               = 0x4040; // ==
    public static final int NOT_EQUAL           = 0x4041; // !=
    public static final int GREATER_THAN        = 0x4042; // >
    public static final int LESS_THAN           = 0x4043; // <
    public static final int NOT_GREATER_THAN    = 0x4044; // <=
    public static final int NOT_LESS_THAN       = 0x4045; // >=
    public static final int LOGICAL_NOT         = 0x1a50; // ! / not
    public static final int LOGICAL_AND         = 0x5b51; // and
    public static final int LOGICAL_OR          = 0x5c52; // or
    public static final int LOGICAL_XOR         = 0x5c53; // xor
    public static final int LOGICAL_IMPLICATION = 0x5d54; // ->
    //TODO: add/ignore priority of priority operators
    public static final int PAREN_OPEN          = 0x3060; // (
    public static final int PAREN_CLOSE         = 0x3061; // )
    // maximum operator id - 0x61 or 97
    private static final int MAX_OPERATOR_ID = 97;

    private HashMap<Object, Integer> opCodeMap;
    private OperatorExecutor[] operators;

    public Operator() {
        opCodeMap = new HashMap<>();
        opCodeMap.put("++", INCREMENT);
        opCodeMap.put("--", DECREMENT);
        opCodeMap.put("=", ASSIGNING);
        opCodeMap.put("+", ADDITION);
        opCodeMap.put("-", SUBTRACTION);
        opCodeMap.put("*", MULTIPLICATION);
        opCodeMap.put("/", DIVISION);
        opCodeMap.put("~", BITWISE_NOT);
        opCodeMap.put("/\\", BITWISE_AND);
        opCodeMap.put("\\/", BITWISE_OR);
        opCodeMap.put("\\'/", BITWISE_XOR);
        opCodeMap.put("<<", BITWISE_SHIFT_L);
        opCodeMap.put(">>", BITWISE_SHIFT_R);
        opCodeMap.put("==", EQUAL);
        opCodeMap.put("!=", NOT_EQUAL);
        opCodeMap.put(">", GREATER_THAN);
        opCodeMap.put("<", LESS_THAN);
        opCodeMap.put("<=", NOT_GREATER_THAN);
        opCodeMap.put(">=", NOT_LESS_THAN);
        opCodeMap.put("!", LOGICAL_NOT);
        opCodeMap.put("not", LOGICAL_NOT);
        opCodeMap.put("and", LOGICAL_AND);
        opCodeMap.put("or", LOGICAL_OR);
        opCodeMap.put("xor", LOGICAL_XOR);
        opCodeMap.put("->", LOGICAL_IMPLICATION);
        opCodeMap.put("(", PAREN_OPEN);
        opCodeMap.put(")", PAREN_CLOSE);
        //next code may be represented x6 shorter, using lambdas
        //operator executors allow us to execute operator without switch-cases in O(1)
        operators = new OperatorExecutor[MAX_OPERATOR_ID+1];
        operators[getId(INCREMENT)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorIncrement();
            }
        };
        operators[getId(DECREMENT)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorDecrement();
            }
        };
        operators[getId(ASSIGNING)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorAssigning(value2);
            }
        };
        operators[getId(ADDITION)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorAddition(value2);
            }
        };
        operators[getId(SUBTRACTION)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorSubtraction(value2);
            }
        };
        operators[getId(MULTIPLICATION)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorMultiplication(value2);
            }
        };
        operators[getId(DIVISION)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorDivision(value2);
            }
        };
        operators[getId(BITWISE_NOT)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorBitwiseNot();
            }
        };
        operators[getId(BITWISE_SHIFT_L)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorBitwiseShiftLeft(value2);
            }
        };
        operators[getId(BITWISE_SHIFT_R)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorBitwiseShiftRight(value2);
            }
        };
        operators[getId(BITWISE_AND)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorBitwiseAnd(value2);
            }
        };
        operators[getId(BITWISE_OR)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorBitwiseOr(value2);
            }
        };
        operators[getId(BITWISE_XOR)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorBitwiseXor(value2);
            }
        };
        operators[getId(EQUAL)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorEqual(value2);
            }
        };
        operators[getId(NOT_EQUAL)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorNotEqual(value2);
            }
        };
        operators[getId(GREATER_THAN)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorGreaterThan(value2);
            }
        };
        operators[getId(LESS_THAN)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorLessThan(value2);
            }
        };
        operators[getId(NOT_GREATER_THAN)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorNotGreaterThan(value2);
            }
        };
        operators[getId(NOT_LESS_THAN)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorNotLessThan(value2);
            }
        };
        operators[getId(LOGICAL_NOT)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorLogicalNot();
            }
        };
        operators[getId(LOGICAL_AND)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorLogicalAnd(value2);
            }
        };
        operators[getId(LOGICAL_OR)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorLogicalOr(value2);
            }
        };
        operators[getId(LOGICAL_XOR)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorLogicalXor(value2);
            }
        };
        operators[getId(LOGICAL_IMPLICATION)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return value1.operatorLogicalImplication(value2);
            }
        };
        operators[getId(PAREN_OPEN)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return null;
            }
        };
        operators[getId(PAREN_CLOSE)] = new OperatorExecutor() {
            @Override
            public Value execute(Value value1, Value value2) {
                return null;
            }
        };
    }

    /**
     * maps operator's string value to its code
     * @param op
     * @return
     */
    public int map(Object op) {
        return opCodeMap.getOrDefault(op, 0xffff);
    }

    public boolean isOperator(Object str) {
        return map(str) == 0xffff;
    }

    public boolean isUnaryOperator(Object str) {
        return isUnaryOperator(map(str));
    }

    public boolean isBinaryOperator(Object str) {
        return isBinaryOperator(map(str));
    }

    public boolean isComparisonOperator(Object str) {
        return isComparisonOperator(map(str));
    }

    public boolean isLogicalOperator(Object str) {
        return isLogicalOperator(map(str));
    }

    public boolean isPriorityOperator(Object str) {
        return isPriorityOperator(map(str));
    }

    public boolean isIncrementDecrementOperator(Object str) {
        return isIncrementDecrementOperator(map(str));
    }

    public int getPriority(Object op) {
        return getPriority(map(op));
    }

    public boolean isUnaryOperator(int code) {
        return (code & 0xf000) == 0x1000;
    }

    public boolean isBinaryOperator(int code) {
        return (code & 0xf000) == 0x2000;
    }

    public boolean isComparisonOperator(int code) {
        return (code & 0xf000) == 0x4000;
    }

    public boolean isLogicalOperator(int code) {
        return (code & 0xf000) == 0x5000;
    }

    public boolean isPriorityOperator(int code) {
        return (code & 0xf000) == 0x3000;
    }

    public boolean isIncrementDecrementOperator(int code) {
        return (code & 0xf000) == 0x0000;
    }

    /**
     * @param code of operator
     * @return priority, extracted via bitwise operator
     */
    public int getPriority(int code) {
        return (code & 0x0f00) >> 8;
    }

    public int getId(int code){
        return code&0x00ff;
    }

    public Value executeBinaryOperator(int operator,Value value1,Value value2){
        return operators[operator & 0x00ff].execute(value1, value2);
    }

    public Value executeUnaryOperator(int operator,Value value){
        return operators[operator & 0x00ff].execute(value,null);
    }

    private abstract class OperatorExecutor{
        public abstract Value execute(Value value1,Value value2);
    }

}
