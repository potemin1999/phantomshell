package phantom.shell.structures;

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

    private HashMap<Object, Integer> opCodeMap;

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
}
