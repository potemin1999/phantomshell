package phantom.shell.expressionEvaluator;

import phantom.shell.environment.Environment;
import phantom.shell.structures.Operator;

import javafx.util.Pair;

import java.util.ArrayList;

public class ExpressionEvaluator {

    Operator operator;

    public ExpressionEvaluator() {
        operator = new Operator();
    }

    public Object evaluateExpression(Environment environment, ArrayList<Pair<Object, Integer>> tokens, int startingIndex, int finishingIndex, ArrayList<Object> postChangingObjectStack, ArrayList<String> postOpStack) {
        ArrayList<Object> objectStack = new ArrayList<>();

        ArrayList<String> opStack = new ArrayList<>();
        ArrayList<String> logicalOpStack = new ArrayList<>();
        ArrayList<String> assigningOpStack = new ArrayList<>();

        String comparisonOp = null;
        Object objectToCompare = null;

        for (int i = startingIndex; i <= finishingIndex; ++i) {
            var token = tokens.get(i);
            //System.out.println(token.getKey() + " " + token.getValue());
            //System.out.println(opStack.size());

            switch (token.getValue()) {
                case 0: // Object
                    objectStack.add(token.getKey());
                    break;

                case 1: // Unary operator
                    var op = (String) token.getKey();
                    Pair<Object, Integer> nextToken = null;

                    if (i < tokens.size() - 1) {
                        nextToken = tokens.get(i + 1);
                    }

                    if (nextToken != null) {
                        if (nextToken.getValue() == 0) {
                            objectStack.add(evaluate(environment, op, nextToken.getKey()));
                            ++i;
                        } else if (nextToken.getKey().equals("(")) {
                            var j = i + 2;
                            var balance = 1;

                            while (j <= finishingIndex && balance != 0) {
                                if (tokens.get(j).getKey().equals("(")) {
                                    ++balance;
                                } else if (tokens.get(j).getKey().equals(")")) {
                                    --balance;
                                }
                                ++j;
                            }
                            --j;

                            //System.out.println("HERE");
                            //System.out.println(i + " " + j);

                            var a = evaluateExpression(environment, tokens, i + 1, j, postChangingObjectStack, postOpStack);
                            objectStack.add(evaluate(environment, op, a));

                            i = j;
                            //System.out.println(i + " " + objectStack.size() + " " + opStack.size());
                        }
                    }
                    break;

                case 2: // Binary operator
                    op = (String) token.getKey();

                    String prevOp;

                    if (!opStack.isEmpty()) {
                        prevOp = (String) opStack.get(opStack.size() - 1);
                    } else {
                        prevOp = null;
                    }

                    while (prevOp != null && !prevOp.equals("(") && operator.getPriority(prevOp) <= operator.getPriority(op)) {
                        var a = objectStack.get(objectStack.size() - 2);
                        var b = objectStack.get(objectStack.size() - 1);

                        objectStack.remove(objectStack.size() - 2);
                        objectStack.remove(objectStack.size() - 1);

                        objectStack.add(evaluate(environment, prevOp, a, b));

                        opStack.remove(opStack.size() - 1);

                        if (!opStack.isEmpty()) {
                            prevOp = opStack.get(opStack.size() - 1);
                        } else {
                            prevOp = null;
                        }
                    }

                    nextToken = tokens.get(i + 1);

                    if (nextToken.getKey() == "(") {
                        var j = i + 2;
                        var balance = 1;

                        while (j <= finishingIndex && balance != 0) {
                            if (tokens.get(j).getKey().equals("(")) {
                                ++balance;
                            } else if (tokens.get(j).getKey().equals(")")) {
                                --balance;
                            }
                            ++j;
                        }
                        --j;

                        var a = objectStack.get(objectStack.size() - 1);
                        objectStack.remove(objectStack.size() - 1);

                        var b = evaluateExpression(environment, tokens, i + 1, j, postChangingObjectStack, postOpStack);
                        objectStack.add(evaluate(environment, op, a, b));

                        i = j;

                        break;
                    }

                    opStack.add(op);
                    break;

                case 3: // Comparison operator
                    op = (String) token.getKey();
                    opStack.add(op);
                    break;

                case 4: // Logical operator
                    op = null;

                    while (!opStack.isEmpty()) {
                        op = opStack.get(opStack.size() - 1);

                        if (op.equals("(")) {
                            break;
                        }

                        opStack.remove(opStack.size() - 1);

                        /**
                         * If there is need to compare, transfer object to compare from object stack
                         * and comparison operator from operator stack.
                         */
                        if (operator.isComparisonOperator(op)) {
                            if (comparisonOp != null) {
                                System.out.println("3 Invalid expression!");
                                return null;
                            }

                            comparisonOp = op;

                            if (!objectStack.isEmpty()) {
                                objectToCompare = objectStack.get(objectStack.size() - 1);
                                objectStack.remove(objectStack.size() - 1);
                            } else {
                                System.out.println("7 Invalid expression!");
                                return null;
                            }

                            continue;
                        }

                        var a = objectStack.get(objectStack.size() - 2);
                        var b = objectStack.get(objectStack.size() - 1);

                        objectStack.remove(objectStack.size() - 2);
                        objectStack.remove(objectStack.size() - 1);

                        objectStack.add(evaluate(environment, op, a, b));
                    }

                    if (comparisonOp != null) {
                        if (!objectStack.isEmpty()) {
                            var a = objectStack.get(objectStack.size() - 1);
                            var b = objectToCompare;

                            objectStack.remove(objectStack.size() - 1);

                            objectStack.add(evaluate(environment, comparisonOp, a, b));

                            comparisonOp = null;
                            objectToCompare = null;
                        } else {
                            System.out.println("8 Invalid expression!");
                            return null;
                        }
                    }

                    op = (String) token.getKey();

                    if (!logicalOpStack.isEmpty()) {
                        prevOp = logicalOpStack.get(logicalOpStack.size() - 1);
                    } else {
                        prevOp = null;
                    }

                    while (prevOp != null && !prevOp.equals("(") && operator.getPriority(prevOp) <= operator.getPriority(op)) {
                        var a = objectStack.get(objectStack.size() - 2);
                        var b = objectStack.get(objectStack.size() - 1);

                        objectStack.remove(objectStack.size() - 2);
                        objectStack.remove(objectStack.size() - 1);

                        objectStack.add(evaluate(environment, prevOp, a, b));

                        logicalOpStack.remove(logicalOpStack.size() - 1);

                        if (!logicalOpStack.isEmpty()) {
                            prevOp = logicalOpStack.get(logicalOpStack.size() - 1);
                        } else {
                            prevOp = null;
                        }
                    }


                    nextToken = tokens.get(i + 1);

                    if (nextToken.getKey() == "(") {
                        var j = i + 2;
                        var balance = 1;

                        while (j <= finishingIndex && balance != 0) {
                            if (tokens.get(j).getKey().equals("(")) {
                                ++balance;
                            } else if (tokens.get(j).getKey().equals(")")) {
                                --balance;
                            }
                            ++j;
                        }
                        --j;

                        var a = objectStack.get(objectStack.size() - 1);
                        objectStack.remove(objectStack.size() - 1);

                        var b = evaluateExpression(environment, tokens, i + 1, j, postChangingObjectStack, postOpStack);
                        objectStack.add(evaluate(environment, op, a, b));

                        i = j;

                        break;
                    }

                    logicalOpStack.add(op);
                    break;

                case 5: // Priority operator
                    op = (String) token.getKey();
                    if (op.equals("(")) {
                        opStack.add(op);
                        logicalOpStack.add(op);
                    } else if (op.equals(")")) {
                        if (!opStack.isEmpty()) {
                            op = opStack.get(opStack.size() - 1);
                            opStack.remove(opStack.size() - 1);
                        } else {
                            System.out.println("2 Invalid expression!");
                            return null;
                        }

                        while (!op.equals("(")) {
                            /**
                             * If there is need to compare, transfer object to compare from object stack
                             * and comparison operator from operator stack.
                             */
                            if (operator.isComparisonOperator(op)) {
                                if (comparisonOp != null) {
                                    System.out.println("13 Invalid expression!");
                                    return null;
                                }

                                comparisonOp = op;

                                if (!objectStack.isEmpty()) {
                                    objectToCompare = objectStack.get(objectStack.size() - 1);
                                    objectStack.remove(objectStack.size() - 1);
                                } else {
                                    System.out.println("4 Invalid expression!");
                                    return null;
                                }

                                op = opStack.get(opStack.size() - 1);
                                opStack.remove(opStack.size() - 1);
                                continue;
                            }

                            var a = objectStack.get(objectStack.size() - 2);
                            var b = objectStack.get(objectStack.size() - 1);

                            objectStack.remove(objectStack.size() - 2);
                            objectStack.remove(objectStack.size() - 1);

                            objectStack.add(evaluate(environment, op, a, b));

                            op = opStack.get(opStack.size() - 1);
                            opStack.remove(opStack.size() - 1);
                        }

                        if (comparisonOp != null) {
                            if (!objectStack.isEmpty()) {
                                var a = objectStack.get(objectStack.size() - 1);
                                var b = objectToCompare;

                                objectStack.remove(objectStack.size() - 1);

                                objectStack.add(evaluate(environment, comparisonOp, a, b));

                                comparisonOp = null;
                                objectToCompare = null;
                            } else {
                                System.out.println("5 Invalid expression!");
                                return null;
                            }
                        }

                        if (!logicalOpStack.isEmpty()) {
                            op = logicalOpStack.get(logicalOpStack.size() - 1);
                            logicalOpStack.remove(logicalOpStack.size() - 1);
                        } else {
                            System.out.println("6 Invalid expression!");
                            return null;
                        }

                        while (!op.equals("(")) {
                            var a = objectStack.get(objectStack.size() - 2);
                            var b = objectStack.get(objectStack.size() - 1);

                            objectStack.remove(objectStack.size() - 2);
                            objectStack.remove(objectStack.size() - 1);

                            objectStack.add(evaluate(environment, op, a, b));

                            op = logicalOpStack.get(logicalOpStack.size() - 1);
                            logicalOpStack.remove(logicalOpStack.size() - 1);
                        }
                    }
                    break;

                case 6: // Assign operator
                    op = (String) token.getKey();
                    assigningOpStack.add(op);
                    break;

                case 7: // Increment/decrement operator
                    op = (String) token.getKey();
                    Pair<Object, Integer> prevToken = null;
                    nextToken = null;

                    if (i > 0) {
                        prevToken = tokens.get(i - 1);
                    }

                    if (i < tokens.size() - 1) {
                        nextToken = tokens.get(i + 1);
                    }

                    if (nextToken != null && nextToken.getValue() == 0) {        // Preincrement/predecrement
                        objectStack.add(evaluate(environment, op, nextToken.getKey()));
                        ++i;
                    } else if (prevToken != null && prevToken.getValue() == 0) { // Postincrement/postdecrement
                        postOpStack.add(op);
                        postChangingObjectStack.add(prevToken.getKey());
                    } else {
                        System.out.println("Expected variable");
                        return null;
                    }
            }
        }

        while (!opStack.isEmpty()) {
            var op = opStack.get(opStack.size() - 1);
            opStack.remove(opStack.size() - 1);

            if (op.equals("(")) {
                System.out.println("10 Invalid expression!");
                return null;
            }

            /**
             * If there is need to compare, transfer object to compare from object stack
             * and comparison operator from operator stack.
             */
            if (operator.isComparisonOperator(op)) {
                if (comparisonOp != null) {
                    System.out.println("14 Invalid expression!");
                    return null;
                }

                comparisonOp = op;

                if (!objectStack.isEmpty()) {
                    objectToCompare = objectStack.get(objectStack.size() - 1);
                    objectStack.remove(objectStack.size() - 1);
                } else {
                    System.out.println("7 Invalid expression!");
                    return null;
                }

                continue;
            }

            var a = objectStack.get(objectStack.size() - 2);
            var b = objectStack.get(objectStack.size() - 1);

            objectStack.remove(objectStack.size() - 2);
            objectStack.remove(objectStack.size() - 1);

            objectStack.add(evaluate(environment, op, a, b));
        }

        if (comparisonOp != null) {
            if (!objectStack.isEmpty()) {
                var a = objectStack.get(objectStack.size() - 1);
                var b = objectToCompare;

                objectStack.remove(objectStack.size() - 1);

                objectStack.add(evaluate(environment, comparisonOp, a, b));

                comparisonOp = null;
                objectToCompare = null;
            } else {
                System.out.println("8 Invalid expression!");
                return null;
            }
        }

        while (!logicalOpStack.isEmpty()) {
            var op = logicalOpStack.get(logicalOpStack.size() - 1);
            logicalOpStack.remove(logicalOpStack.size() - 1);

            if (op.equals("(")) {
                System.out.println("10 Invalid expression!");
                return null;
            }

            var a = objectStack.get(objectStack.size() - 2);
            var b = objectStack.get(objectStack.size() - 1);

            objectStack.remove(objectStack.size() - 2);
            objectStack.remove(objectStack.size() - 1);

            objectStack.add(evaluate(environment, op, a, b));
        }

        while (!assigningOpStack.isEmpty()) {
            var op = assigningOpStack.get(assigningOpStack.size() - 1);
            assigningOpStack.remove(assigningOpStack.size() - 1);

            var a = objectStack.get(objectStack.size() - 2);
            var b = objectStack.get(objectStack.size() - 1);

            objectStack.remove(objectStack.size() - 2);
            objectStack.remove(objectStack.size() - 1);

            objectStack.add(evaluate(environment, op, a, b));
        }

        if (objectStack.size() == 1) {
            var obj = objectStack.get(0);
            var val = environment.getVariable(obj);

            return val == null ? obj : val;
        } else {
            System.out.printf("Object stack has wrong size! It is %d\n",
                    objectStack.size());
            return null;
        }
    }

    public Object evaluateExpression(Environment environment, ArrayList<Pair<Object, Integer>> tokens) {
        ArrayList<Object> postChangingObjectStack = new ArrayList<>();
        ArrayList<String> postOpStack = new ArrayList<>();

        var result = evaluateExpression(environment, tokens, 0, tokens.size() - 1, postChangingObjectStack, postOpStack);

        while (!postOpStack.isEmpty()) {
            var op = postOpStack.get(postOpStack.size() - 1);
            postOpStack.remove(postOpStack.size() - 1);

            var a = postChangingObjectStack.get(postChangingObjectStack.size() - 1);

            postChangingObjectStack.remove(postChangingObjectStack.size() - 1);

            evaluate(environment, op, a);
        }

        return result;
    }

    public Object evaluate(Environment environment, String op, Object obj1, Object obj2) {
        var val2 = environment.getVariable(obj2);
        if (val2 != null) {
            obj2 = val2;
        }

        if (op.equals("=")) {
            environment.setVariable(obj1, obj2);
            return obj2;
        }

        var val1 = environment.getVariable(obj1);
        if (val1 != null) {
            obj1 = val1;
        }

        if (obj1 instanceof Number && obj2 instanceof Number) {
            Number num1 = (Number) obj1;
            Number num2 = (Number) obj2;

            if (num1 instanceof Double || num2 instanceof Double) {
                var a = num1.doubleValue();
                var b = num2.doubleValue();
                var result = 0.0;

                switch (op) {
                    case "+":
                        result = a + b;
                        break;
                    case "-":
                        result = a - b;
                        break;
                    case "*":
                        result = a * b;
                        break;
                    case "/":
                        result = a / b;
                        break;

                    //TODO: compare by EPS
                    case "==":
                        result = (a == b) ? 1 : 0;
                        break;
                    case "!=":
                        result = (a != b) ? 1 : 0;
                        break;
                    case ">":
                        result = (a > b) ? 1 : 0;
                        break;
                    case ">=":
                        result = (a >= b) ? 1 : 0;
                        break;
                    case "<":
                        result = (a < b) ? 1 : 0;
                        break;
                    case "<=":
                        result = (a <= b) ? 1 : 0;
                        break;

                    case "and":
                        result = (a != 0 && b != 0) ? 1 : 0;
                        break;
                    case "or":
                        result = (a != 0 || b != 0) ? 1 : 0;
                        break;
                    case "xor":
                        result = (a == 0 && b != 0 || a != 0 && b == 0) ? 1 : 0;
                        break;
                    case "->":
                        result = (a != 0 && b == 0) ? 0 : 1;
                        break;

                    default:
                        System.out.println("Operator " + op + " is not defined for floats.");
                        result = 0;
                }

                return result;
            } else if (num1 instanceof Integer || num2 instanceof Integer) {
                var a = num1.intValue();
                var b = num2.intValue();
                var result = 0;

                switch (op) {
                    case "+":
                        result = a + b;
                        break;
                    case "-":
                        result = a - b;
                        break;
                    case "*":
                        result = a * b;
                        break;
                    case "/":
                        result = a / b;
                        break;

                    case "/\\":
                        result = a & b;
                        break;
                    case "\\/":
                        result = a | b;
                        break;
                    case "\\\'/":
                        result = a ^ b;
                        break;
                    case "<<":
                        result = a << b;
                        break;
                    case ">>":
                        result = a >> b;
                        break;

                    case "==":
                        result = (a == b) ? 1 : 0;
                        break;
                    case "!=":
                        result = (a != b) ? 1 : 0;
                        break;
                    case ">":
                        result = (a > b) ? 1 : 0;
                        break;
                    case ">=":
                        result = (a >= b) ? 1 : 0;
                        break;
                    case "<":
                        result = (a < b) ? 1 : 0;
                        break;
                    case "<=":
                        result = (a <= b) ? 1 : 0;
                        break;

                    case "and":
                        result = (a != 0 && b != 0) ? 1 : 0;
                        break;
                    case "or":
                        result = (a != 0 || b != 0) ? 1 : 0;
                        break;
                    case "xor":
                        result = (a == 0 && b != 0 || a != 0 && b == 0) ? 1 : 0;
                        break;
                    case "->":
                        result = (a != 0 && b == 0) ? 0 : 1;
                        break;

                    default:
                        System.out.println("Operator " + op + " is not defined for integers.");
                        result = 0;
                }

                return result;
            } else {
                System.out.println("Unknown number type");
                return null;
            }
        } else {
            System.out.println("Unknown object type");
            return null;
        }
    }

    public Object evaluate(Environment environment, String op, Object obj) {
        var name = obj;

        var val = environment.getVariable(name);
        if (val != null) {
            obj = val;
        }

        if (obj instanceof Number) {
            Number num = (Number) obj;

            if (num instanceof Double) {
                var a = num.doubleValue();
                var result = 0.0;

                switch (op) {
                    case "++":
                        environment.setVariable(name, a + 1);
                        result = ((Number) environment.getVariable(name)).doubleValue();
                        break;
                    case "--":
                        environment.setVariable(name, a - 1);
                        result = ((Number) environment.getVariable(name)).doubleValue();
                        break;
                    case "!":
                        result = a != 0 ? 0 : 1;
                        break;
                    case "not":
                        result = a != 0 ? 0 : 1;
                        break;

                    default:
                        System.out.println("Operator " + op + " is not defined for floats.");
                        result = 0;
                }

                return result;
            } else if (num instanceof Integer) {
                var a = num.intValue();
                var result = 0;

                switch (op) {
                    case "++":
                        environment.setVariable(name, a + 1);
                        result = ((Number) environment.getVariable(name)).intValue();
                        break;
                    case "--":
                        environment.setVariable(name, a - 1);
                        result = ((Number) environment.getVariable(name)).intValue();
                        break;
                    case "~":
                        result = ~a;
                        break;
                    case "!":
                        result = a != 0 ? 0 : 1;
                        break;
                    case "not":
                        result = a != 0 ? 0 : 1;
                        break;

                    default:
                        System.out.println("Operator " + op + " is not defined for integers.");
                        result = 0;
                }

                return result;
            } else {
                System.out.println("Unknown number type");
                return null;
            }
        } else {
            System.out.println("Unknown object type");
            return null;
        }
    }
}