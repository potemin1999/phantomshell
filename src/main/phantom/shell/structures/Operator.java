package phantom.shell.structures;

import java.util.HashMap;
import java.util.HashSet;

public class Operator {
    HashSet<Object> unaryOperators, incrementDecrementOperators, binaryOperators, comparisonOperators, logicalOperators, priorityOperators;

    HashMap<Object, Integer> opPriority;

    public Operator() {
        unaryOperators = new HashSet<>();
        incrementDecrementOperators = new HashSet<>();
        binaryOperators = new HashSet<>();
        priorityOperators = new HashSet<>();
        comparisonOperators = new HashSet<>();
        logicalOperators = new HashSet<>();

        incrementDecrementOperators.add("++");      // increment
        incrementDecrementOperators.add("--");      // decrement

        binaryOperators.add("=");      // assigning

        binaryOperators.add("+");      // addition
        binaryOperators.add("-");      // subtraction
        binaryOperators.add("*");      // multiplication
        binaryOperators.add("/");      // division

        unaryOperators.add("~");       // bitwise not
        binaryOperators.add("/\\");    // bitwise and
        binaryOperators.add("\\/");    // bitwise or
        binaryOperators.add("\\\'/");  // bitwise xor
        binaryOperators.add("<<");     // bitwise arithmetical shift left
        binaryOperators.add(">>");     // bitwise arithmetical shift right

        comparisonOperators.add("=="); // equality
        comparisonOperators.add("!="); // inequality
        comparisonOperators.add(">");  // greater than
        comparisonOperators.add(">="); // not less than
        comparisonOperators.add("<");  // less than
        comparisonOperators.add("<="); // not greater than

        unaryOperators.add("!");     // logical not
        unaryOperators.add("not");       // logical not (these two are equivalent)
        logicalOperators.add("and");   // logical conjunction
        logicalOperators.add("or");    // logical inclusive disjunction
        logicalOperators.add("xor");   // logical exclusive disjunction
        logicalOperators.add("->");    // logical implication

        priorityOperators.add("(");    // opening parenthesis
        priorityOperators.add(")");    // closing parenthesis

        opPriority = new HashMap<>();

        opPriority.put("++", 0);
        opPriority.put("--", 0);

        opPriority.put("<<", 1);
        opPriority.put(">>", 1);
        opPriority.put("~", 2);
        opPriority.put("/\\", 3);
        opPriority.put("\\/", 4);
        opPriority.put("\\\'/", 4);

        opPriority.put("*", 5);
        opPriority.put("/", 5);
        opPriority.put("+", 6);
        opPriority.put("-", 6);

        opPriority.put("!", 10);
        opPriority.put("not", 10);
        opPriority.put("and", 11);
        opPriority.put("or", 12);
        opPriority.put("xor", 12);
        opPriority.put("->", 13);

        opPriority.put("=", 20);
    }

    public boolean isOperator(Object str) {
        return isUnaryOperator(str) || isBinaryOperator(str) || isComparisonOperator(str) || isLogicalOperator(str)
                || isPriorityOperator(str) || isIncrementDecrementOperator(str);
    }

    public boolean isUnaryOperator(Object str) {
        return unaryOperators.contains(str);
    }

    public boolean isBinaryOperator(Object str) {
        return binaryOperators.contains(str);
    }

    public boolean isComparisonOperator(Object str) {
        return comparisonOperators.contains(str);
    }

    public boolean isLogicalOperator(Object str) {
        return logicalOperators.contains(str);
    }

    public boolean isPriorityOperator(Object str) {
        return priorityOperators.contains(str);
    }

    public boolean isIncrementDecrementOperator(Object str) {
        return incrementDecrementOperators.contains(str);
    }

    public int getPriority(Object op) {
        return opPriority.get(op);
    }
}
