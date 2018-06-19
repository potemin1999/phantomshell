package phantom.shell.expressionEvaluator;

import phantom.shell.structures.Operator;

import javafx.util.Pair;

import java.util.ArrayList;

public class ExpressionEvaluator {

    Operator operator;

    public ExpressionEvaluator() {
        operator = new Operator();
    }

    public Object evaluateExpression(ArrayList<Pair<Object, Integer>> tokens, int startingIndex, int finishingIndex) {
        ArrayList<Object> objectStack = new ArrayList<>();
        ArrayList<String> opStack = new ArrayList<>();
        ArrayList<String> logicalOpStack = new ArrayList<>();
        //ArrayList<Object> booleanStack = new ArrayList<>();

        String comparisonOp = null;
        Object objectToCompare = null;

        for (int i = startingIndex; i <= finishingIndex; ++i) {
            var token = tokens.get(i);

            switch (token.getValue()) {
                case 0: // Object
                    objectStack.add(token.getKey());
                    break;

                case 1: // Unary operator
                    // TODO: Evaluating unary operators
                    break;

                case 2: // Binary operator
                    var op = (String) token.getKey();

                    Object prevOp;

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

                        objectStack.add(evaluate((String) prevOp, a, b));

                        opStack.remove(opStack.size() - 1);

                        if (!opStack.isEmpty()) {
                            prevOp = (String) opStack.get(opStack.size() - 1);
                        } else {
                            prevOp = null;
                        }
                    }

                    var nextToken = tokens.get(i + 1);

                    if (nextToken.getKey() == "(") {
                        int j = finishingIndex;
                        while (tokens.get(j).getKey() != ")") {
                            --j;
                        }

                        var a = objectStack.get(objectStack.size() - 1);
                        objectStack.remove(objectStack.size() - 1);

                        var b = evaluateExpression(tokens, i + 1, j);
                        objectStack.add(evaluate((String) op, a, b));

                        i = j + 1;

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
                        op = (String) opStack.get(opStack.size() - 1);

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

                            comparisonOp = (String) op;

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

                        objectStack.add(evaluate((String) op, a, b));
                    }

                    if (comparisonOp != null) {
                        if (!objectStack.isEmpty()) {
                            var a = objectStack.get(objectStack.size() - 1);
                            var b = objectToCompare;

                            objectStack.remove(objectStack.size() - 1);

                            objectStack.add(evaluate(comparisonOp, a, b));

                            comparisonOp = null;
                            objectToCompare = null;
                        } else {
                            System.out.println("8 Invalid expression!");
                            return null;
                        }
                    }

                    op = (String) token.getKey();

                    if (!logicalOpStack.isEmpty()) {
                        prevOp = (String) logicalOpStack.get(logicalOpStack.size() - 1);
                    } else {
                        prevOp = null;
                    }

                    while (prevOp != null && !prevOp.equals("(") && operator.getPriority(prevOp) <= operator.getPriority(op)) {
                        var a = objectStack.get(objectStack.size() - 2);
                        var b = objectStack.get(objectStack.size() - 1);

                        objectStack.remove(objectStack.size() - 2);
                        objectStack.remove(objectStack.size() - 1);

                        objectStack.add(evaluate((String) prevOp, a, b));

                        logicalOpStack.remove(logicalOpStack.size() - 1);

                        if (!logicalOpStack.isEmpty()) {
                            prevOp = (String) logicalOpStack.get(logicalOpStack.size() - 1);
                        } else {
                            prevOp = null;
                        }
                    }


                    nextToken = tokens.get(i + 1);

                    if (nextToken.getKey() == "(") {
                        int j = finishingIndex;
                        while (tokens.get(j).getKey() != ")") {
                            --j;
                        }

                        var a = objectStack.get(objectStack.size() - 1);
                        objectStack.remove(objectStack.size() - 1);

                        var b = evaluateExpression(tokens, i + 1, j);
                        objectStack.add(evaluate((String) op, a, b));

                        i = j + 1;

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
                            op = (String) opStack.get(opStack.size() - 1);
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

                                comparisonOp = (String) op;

                                if (!objectStack.isEmpty()) {
                                    objectToCompare = objectStack.get(objectStack.size() - 1);
                                    objectStack.remove(objectStack.size() - 1);
                                } else {
                                    System.out.println("4 Invalid expression!");
                                    return null;
                                }

                                op = (String) opStack.get(opStack.size() - 1);
                                opStack.remove(opStack.size() - 1);
                                continue;
                            }

                            var a = objectStack.get(objectStack.size() - 2);
                            var b = objectStack.get(objectStack.size() - 1);

                            objectStack.remove(objectStack.size() - 2);
                            objectStack.remove(objectStack.size() - 1);

                            objectStack.add(evaluate((String) op, a, b));

                            op = (String) opStack.get(opStack.size() - 1);
                            opStack.remove(opStack.size() - 1);
                        }

                        if (comparisonOp != null) {
                            if (!objectStack.isEmpty()) {
                                var a = objectStack.get(objectStack.size() - 1);
                                var b = objectToCompare;

                                objectStack.remove(objectStack.size() - 1);

                                objectStack.add(evaluate(comparisonOp, a, b));

                                comparisonOp = null;
                                objectToCompare = null;
                            } else {
                                System.out.println("5 Invalid expression!");
                                return null;
                            }
                        }

                        if (!logicalOpStack.isEmpty()) {
                            op = (String) logicalOpStack.get(logicalOpStack.size() - 1);
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

                            objectStack.add(evaluate((String) op, a, b));

                            op = (String) logicalOpStack.get(logicalOpStack.size() - 1);
                            logicalOpStack.remove(logicalOpStack.size() - 1);
                        }
                    }
                    break;
            }
        }

        while (!opStack.isEmpty()) {
            var op = (String) opStack.get(opStack.size() - 1);
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

                comparisonOp = (String) op;

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

            objectStack.add(evaluate((String) op, a, b));
        }

        if (comparisonOp != null) {
            if (!objectStack.isEmpty()) {
                var a = objectStack.get(objectStack.size() - 1);
                var b = objectToCompare;

                objectStack.remove(objectStack.size() - 1);

                objectStack.add(evaluate(comparisonOp, a, b));

                comparisonOp = null;
                objectToCompare = null;
            } else {
                System.out.println("8 Invalid expression!");
                return null;
            }
        }

        while (!logicalOpStack.isEmpty()) {
            var op = (String) logicalOpStack.get(logicalOpStack.size() - 1);
            logicalOpStack.remove(logicalOpStack.size() - 1);

            if (op.equals("(")) {
                System.out.println("10 Invalid expression!");
                return null;
            }

            var a = objectStack.get(objectStack.size() - 2);
            var b = objectStack.get(objectStack.size() - 1);

            objectStack.remove(objectStack.size() - 2);
            objectStack.remove(objectStack.size() - 1);

            objectStack.add(evaluate((String) op, a, b));
        }

        if (objectStack.size() == 1) {
            return objectStack.get(0);
        } else {
            System.out.printf("Object stack has wrong size! It is %d\n",
                    objectStack.size());
            return null;
        }
    }

    public Object evaluateExpression(ArrayList<Pair<Object, Integer>> tokens) {
        return evaluateExpression(tokens, 0, tokens.size() - 1);
    }

    public Object evaluate(String op, Object obj1, Object obj2) {
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
}
