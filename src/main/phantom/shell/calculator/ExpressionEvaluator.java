package phantom.shell.calculator;

import phantom.shell.environment.Environment;
import phantom.shell.structures.Operator;

import phantom.support.util.Pair;
import phantom.support.util.ArrayList;
import phantom.support.util.List;


public class ExpressionEvaluator {

    private Operator operator;

    public ExpressionEvaluator() {
        operator = new Operator();
    }

    private Pair<Object, Environment> evaluateExpression(Environment environment, ArrayList<Pair<Object, Integer>> tokens, int startingIndex, int finishingIndex, List<Object> postChangingObjectStack, List<Object> postOpStack) {
        ArrayList<Object> objectStack = new ArrayList<>();

        ArrayList<Integer> opStack = new ArrayList<>();
        ArrayList<Integer> logicalOpStack = new ArrayList<>();
        ArrayList<Integer> assigningOpStack = new ArrayList<>();

        Integer comparisonOp = null;
        Object objectToCompare = null;

        for (int i = startingIndex; i <= finishingIndex; ++i) {
            var token = tokens.get(i);

            switch (token.getValue()) {
                case 0: // Object
                    objectStack.add(token.getKey());
                    break;

                case 1: // Unary operator
                    Integer op = (Integer) token.getKey();
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

                            var pair = evaluateExpression(environment, tokens, i + 1, j, postChangingObjectStack, postOpStack);
                            var a = pair.getKey();
                            environment = pair.getValue();

                            objectStack.add(evaluate(environment, op, a));

                            i = j;
                        }
                    }
                    break;

                case 2: // Binary operator
                    op = (Integer) token.getKey();

                    Integer prevOp;

                    if (!opStack.isEmpty()) {
                        prevOp = opStack.get(opStack.size() - 1);
                    } else {
                        prevOp = null;
                    }

                    while (prevOp != null && prevOp != Operator.PAREN_OPEN && operator.getPriority(prevOp) <= operator.getPriority(op)) {
                        var a = objectStack.get(objectStack.size() - 2);
                        var b = objectStack.get(objectStack.size() - 1);

                        objectStack.remove(objectStack.size() - 2);
                        objectStack.remove(objectStack.size() - 1);

                        var pair = evaluate(environment, prevOp, a, b);
                        objectStack.add(pair.getKey());
                        environment = pair.getValue();

                        opStack.remove(opStack.size() - 1);

                        if (!opStack.isEmpty()) {
                            prevOp = opStack.get(opStack.size() - 1);
                        } else {
                            prevOp = null;
                        }
                    }

                    nextToken = tokens.get(i + 1);

                    if (nextToken.getValue() == 5 && (Integer) nextToken.getKey() == Operator.PAREN_OPEN) {
                        var j = i + 2;
                        var balance = 1;

                        while (j <= finishingIndex && balance != 0) {
                            if ((Integer) tokens.get(j).getKey() == Operator.PAREN_OPEN) {
                                ++balance;
                            } else if ((Integer) tokens.get(j).getKey() == Operator.PAREN_CLOSE) {
                                --balance;
                            }
                            ++j;
                        }
                        --j;

                        var a = objectStack.get(objectStack.size() - 1);
                        objectStack.remove(objectStack.size() - 1);

                        var pair = evaluateExpression(environment, tokens, i + 1, j, postChangingObjectStack, postOpStack);
                        var b = pair.getKey();
                        environment = pair.getValue();

                        pair = evaluate(environment, op, a, b);
                        objectStack.add(pair.getKey());
                        environment = pair.getValue();

                        i = j;

                        break;
                    }

                    opStack.add(op);
                    break;

                case 3: // Comparison operator
                    op = (Integer) token.getKey();
                    opStack.add(op);
                    break;

                case 4: // Logical operator
                    while (!opStack.isEmpty()) {
                        op = opStack.get(opStack.size() - 1);

                        if (op == Operator.PAREN_OPEN) {
                            break;
                        }

                        opStack.remove(opStack.size() - 1);

                        /*
                         * If there is need to compare, transfer object to compare from object stack
                         * and comparison operator from operator stack.
                         */
                        if (operator.isComparisonOperator(op)) {
                            if (comparisonOp != null) {
                                throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "3");
                                //return null;
                            }

                            comparisonOp = op;

                            if (!objectStack.isEmpty()) {
                                objectToCompare = objectStack.get(objectStack.size() - 1);
                                objectStack.remove(objectStack.size() - 1);
                            } else {
                                throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "7");
                                //return null;
                            }

                            continue;
                        }

                        var a = objectStack.get(objectStack.size() - 2);
                        var b = objectStack.get(objectStack.size() - 1);

                        objectStack.remove(objectStack.size() - 2);
                        objectStack.remove(objectStack.size() - 1);

                        var pair = evaluate(environment, op, a, b);
                        objectStack.add(pair.getKey());
                        environment = pair.getValue();
                    }

                    if (comparisonOp != null) {
                        if (!objectStack.isEmpty()) {
                            var a = objectStack.get(objectStack.size() - 1);
                            var b = objectToCompare;

                            objectStack.remove(objectStack.size() - 1);

                            var pair = evaluate(environment, comparisonOp, a, b);
                            objectStack.add(pair.getKey());
                            environment = pair.getValue();

                            comparisonOp = null;
                            objectToCompare = null;
                        } else {
                            throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION,"8");
                            //return null;
                        }
                    }

                    op = (Integer) token.getKey();

                    if (!logicalOpStack.isEmpty()) {
                        prevOp = logicalOpStack.get(logicalOpStack.size() - 1);
                    } else {
                        prevOp = null;
                    }

                    while (prevOp != null && prevOp != Operator.PAREN_OPEN && operator.getPriority(prevOp) <= operator.getPriority(op)) {
                        var a = objectStack.get(objectStack.size() - 2);
                        var b = objectStack.get(objectStack.size() - 1);

                        objectStack.remove(objectStack.size() - 2);
                        objectStack.remove(objectStack.size() - 1);

                        var pair = evaluate(environment, prevOp, a, b);
                        objectStack.add(pair.getKey());
                        environment = pair.getValue();

                        logicalOpStack.remove(logicalOpStack.size() - 1);

                        if (!logicalOpStack.isEmpty()) {
                            prevOp = logicalOpStack.get(logicalOpStack.size() - 1);
                        } else {
                            prevOp = null;
                        }
                    }


                    nextToken = tokens.get(i + 1);

                    if ((Integer) nextToken.getKey() == Operator.PAREN_OPEN) {
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

                        var pair = evaluateExpression(environment, tokens, i + 1, j, postChangingObjectStack, postOpStack);
                        var b = pair.getKey();
                        environment = pair.getValue();

                        pair = evaluate(environment, op, a, b);
                        objectStack.add(pair.getKey());
                        environment = pair.getValue();

                        i = j;

                        break;
                    }

                    logicalOpStack.add(op);
                    break;

                case 5: // Priority operator
                    op = (Integer) token.getKey();
                    if (op == Operator.PAREN_OPEN) {
                        opStack.add(op);
                        logicalOpStack.add(op);
                    } else if (op == Operator.PAREN_CLOSE) {
                        if (!opStack.isEmpty()) {
                            op = opStack.get(opStack.size() - 1);
                            opStack.remove(opStack.size() - 1);
                        } else {
                            throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "2");
                            //return null;
                        }

                        while (op != Operator.PAREN_OPEN) {
                            /*
                             * If there is need to compare, transfer object to compare from object stack
                             * and comparison operator from operator stack.
                             */
                            if (operator.isComparisonOperator(op)) {
                                if (comparisonOp != null) {
                                    throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION,"13");
                                    //return null;
                                }

                                comparisonOp = op;

                                if (!objectStack.isEmpty()) {
                                    objectToCompare = objectStack.get(objectStack.size() - 1);
                                    objectStack.remove(objectStack.size() - 1);
                                } else {
                                    throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION,"4");
                                    //return null;
                                }

                                op = opStack.get(opStack.size() - 1);
                                opStack.remove(opStack.size() - 1);
                                continue;
                            }

                            var a = objectStack.get(objectStack.size() - 2);
                            var b = objectStack.get(objectStack.size() - 1);

                            objectStack.remove(objectStack.size() - 2);
                            objectStack.remove(objectStack.size() - 1);

                            var pair = evaluate(environment, op, a, b);
                            objectStack.add(pair.getKey());
                            environment = pair.getValue();

                            op = opStack.get(opStack.size() - 1);
                            opStack.remove(opStack.size() - 1);
                        }

                        if (comparisonOp != null) {
                            if (!objectStack.isEmpty()) {
                                var a = objectStack.get(objectStack.size() - 1);
                                var b = objectToCompare;

                                objectStack.remove(objectStack.size() - 1);

                                var pair = evaluate(environment, comparisonOp, a, b);
                                objectStack.add(pair.getKey());
                                environment = pair.getValue();

                                comparisonOp = null;
                                objectToCompare = null;
                            } else {
                                throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION,"5");
                                //return null;
                            }
                        }

                        if (!logicalOpStack.isEmpty()) {
                            op = logicalOpStack.get(logicalOpStack.size() - 1);
                            logicalOpStack.remove(logicalOpStack.size() - 1);
                        } else {
                            throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION,"6");
                            //return null;
                        }

                        while (op != Operator.PAREN_OPEN) {
                            var a = objectStack.get(objectStack.size() - 2);
                            var b = objectStack.get(objectStack.size() - 1);

                            objectStack.remove(objectStack.size() - 2);
                            objectStack.remove(objectStack.size() - 1);

                            var pair = evaluate(environment, op, a, b);
                            objectStack.add(pair.getKey());
                            environment = pair.getValue();

                            op = logicalOpStack.get(logicalOpStack.size() - 1);
                            logicalOpStack.remove(logicalOpStack.size() - 1);
                        }
                    }
                    break;

                case 6: // Assign operator
                    op = (Integer) token.getKey();
                    assigningOpStack.add(op);
                    break;

                case 7: // Increment/decrement operator
                    op = (Integer) token.getKey();
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
                        throw new ExpressionEvaluationException("Expected variable");
                        //return null;
                    }
            }
        }

        while (!opStack.isEmpty()) {
            var op = opStack.get(opStack.size() - 1);
            opStack.remove(opStack.size() - 1);

            if (op == Operator.PAREN_OPEN) {
                throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "10");
                //return null;
            }

            /*
             * If there is need to compare, transfer object to compare from object stack
             * and comparison operator from operator stack.
             */
            if (operator.isComparisonOperator(op)) {
                if (comparisonOp != null) {
                    throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "14");
                    //return null;
                }

                comparisonOp = op;

                if (!objectStack.isEmpty()) {
                    objectToCompare = objectStack.get(objectStack.size() - 1);
                    objectStack.remove(objectStack.size() - 1);
                } else {
                    throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "7");
                    //return null;
                }

                continue;
            }

            var a = objectStack.get(objectStack.size() - 2);
            var b = objectStack.get(objectStack.size() - 1);

            objectStack.remove(objectStack.size() - 2);
            objectStack.remove(objectStack.size() - 1);

            var pair = evaluate(environment, op, a, b);
            objectStack.add(pair.getKey());
            environment = pair.getValue();
        }

        if (comparisonOp != null) {
            if (!objectStack.isEmpty()) {
                var a = objectStack.get(objectStack.size() - 1);

                objectStack.remove(objectStack.size() - 1);

                var pair = evaluate(environment, comparisonOp, a, objectToCompare);
                objectStack.add(pair.getKey());
                environment = pair.getValue();
            } else {
                throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "8");
                //return null;
            }
        }

        while (!logicalOpStack.isEmpty()) {
            var op = logicalOpStack.get(logicalOpStack.size() - 1);
            logicalOpStack.remove(logicalOpStack.size() - 1);

            if (op == Operator.PAREN_OPEN) {
                throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "10");
                //return null;
            }

            var a = objectStack.get(objectStack.size() - 2);
            var b = objectStack.get(objectStack.size() - 1);

            objectStack.remove(objectStack.size() - 2);
            objectStack.remove(objectStack.size() - 1);

            var pair = evaluate(environment, op, a, b);
            objectStack.add(pair.getKey());
            environment = pair.getValue();
        }

        while (!assigningOpStack.isEmpty()) {
            var op = assigningOpStack.get(assigningOpStack.size() - 1);
            assigningOpStack.remove(assigningOpStack.size() - 1);

            var a = objectStack.get(objectStack.size() - 2);
            var b = objectStack.get(objectStack.size() - 1);

            objectStack.remove(objectStack.size() - 2);
            objectStack.remove(objectStack.size() - 1);

            var pair = evaluate(environment, op, a, b);
            objectStack.add(pair.getKey());
            environment = pair.getValue();
        }

        if (objectStack.size() == 1) {
            var obj = objectStack.get(0);

            var val = environment.getVariable(obj);

            return val == null ? new Pair(obj, environment) : new Pair(val, environment);
        } else if (objectStack.size() == 0) {
            return new Pair(null, environment);
        } else {
            throw new ExpressionEvaluationException(ExpressionEvaluationFault.OBJ_STACK_WRONG_SIZE,""+objectStack.size());
            //return null;
        }
    }

    public Pair<Object, Environment> evaluateExpression(Environment environment, ArrayList<Pair<Object, Integer>> tokens) {
        ArrayList<Object> postChangingObjectStack = new ArrayList<>();
        ArrayList<Object> postOpStack = new ArrayList<>();

        var pair = evaluateExpression(environment, tokens, 0, tokens.size() - 1, postChangingObjectStack, postOpStack);
        var result = pair.getKey();
        environment = pair.getValue();

        while (!postOpStack.isEmpty()) {
            var op = (Integer) postOpStack.get(postOpStack.size() - 1);
            postOpStack.remove(postOpStack.size() - 1);

            var a = postChangingObjectStack.get(postChangingObjectStack.size() - 1);

            postChangingObjectStack.remove(postChangingObjectStack.size() - 1);

            evaluate(environment, op, a);
        }

        return new Pair<>(result, environment);
    }

    private Pair<Object, Environment> evaluate(Environment environment, Integer op, Object obj1, Object obj2) {
        //System.out.println(obj1 + " " + op + " " + obj2);

        var val2 = environment.getVariable(obj2);
        if (val2 != null) {
            obj2 = val2;
        }

        if (op == Operator.ASSIGNING) {
            environment.setVariable(obj1, obj2);

            return new Pair(obj2, environment);
        }

        var val1 = environment.getVariable(obj1);
        if (val1 != null) {
            obj1 = val1;
        }

        //System.out.println(obj1 + " " + op + " " + obj2);

        if (obj1 instanceof Number && obj2 instanceof Number) {
            Number num1 = (Number) obj1;
            Number num2 = (Number) obj2;

            if (num1 instanceof Double || num2 instanceof Double) {
                var a = num1.doubleValue();
                var b = num2.doubleValue();
                var result = 0.0;
                final var EPS = 1e-9;

                switch (op) {
                    case Operator.ADDITION:
                        result = a + b;
                        break;
                    case Operator.SUBTRACTION:
                        result = a - b;
                        break;
                    case Operator.MULTIPLICATION:
                        result = a * b;
                        break;
                    case Operator.DIVISION:
                        result = a / b;
                        break;

                    //TODO: compare by EPS
                    case Operator.EQUAL:
                        result = Math.abs(a - b) < EPS ? 1 : 0;
                        break;
                    case Operator.NOT_EQUAL:
                        result = Math.abs(a - b) >= EPS ? 1 : 0;
                        break;
                    case Operator.GREATER_THAN:
                        result = a - b > EPS ? 1 : 0;
                        break;
                    case Operator.NOT_LESS_THAN:
                        result = a - b >= EPS ? 1 : 0;
                        break;
                    case Operator.LESS_THAN:
                        result = b - a > EPS ? 1 : 0;
                        break;
                    case Operator.NOT_GREATER_THAN:
                        result = b - a >= EPS ? 1 : 0;
                        break;

                    case Operator.LOGICAL_AND:
                        result = (a != 0 && b != 0) ? 1 : 0;
                        break;
                    case Operator.LOGICAL_OR:
                        result = (a != 0 || b != 0) ? 1 : 0;
                        break;
                    case Operator.LOGICAL_XOR:
                        result = (a == 0 && b != 0 || a != 0 && b == 0) ? 1 : 0;
                        break;
                    case Operator.LOGICAL_IMPLICATION:
                        result = (a != 0 && b == 0) ? 0 : 1;
                        break;

                    default:
                        throw new ExpressionEvaluationException(ExpressionEvaluationFault.UNDEFINED_FLOAT_OPERATOR, op.toString());
                        //result = 0;
                }

                return new Pair(result, environment);
            } else if (num1 instanceof Integer || num2 instanceof Integer) {
                var a = num1.intValue();
                var b = num2.intValue();
                var result = 0;

                switch (op) {
                    case Operator.ADDITION:
                        result = a + b;
                        break;
                    case Operator.SUBTRACTION:
                        result = a - b;
                        break;
                    case Operator.MULTIPLICATION:
                        result = a * b;
                        break;
                    case Operator.DIVISION:
                        if (b == 0)
                            throw new ExpressionEvaluationException(ExpressionEvaluationFault.INTEGER_DIVISION_BY_ZERO);
                        result = a / b;
                        break;

                    case Operator.BITWISE_AND:
                        result = a & b;
                        break;
                    case Operator.BITWISE_OR:
                        result = a | b;
                        break;
                    case Operator.BITWISE_XOR:
                        result = a ^ b;
                        break;
                    case Operator.BITWISE_SHIFT_L:
                        result = a << b;
                        break;
                    case Operator.BITWISE_SHIFT_R:
                        result = a >> b;
                        break;

                    case Operator.EQUAL:
                        result = (a == b) ? 1 : 0;
                        break;
                    case Operator.NOT_EQUAL:
                        result = (a != b) ? 1 : 0;
                        break;
                    case Operator.GREATER_THAN:
                        result = (a > b) ? 1 : 0;
                        break;
                    case Operator.NOT_LESS_THAN:
                        result = (a >= b) ? 1 : 0;
                        break;
                    case Operator.LESS_THAN:
                        result = (a < b) ? 1 : 0;
                        break;
                    case Operator.NOT_GREATER_THAN:
                        result = (a <= b) ? 1 : 0;
                        break;

                    case Operator.LOGICAL_AND:
                        result = (a != 0 && b != 0) ? 1 : 0;
                        break;
                    case Operator.LOGICAL_OR:
                        result = (a != 0 || b != 0) ? 1 : 0;
                        break;
                    case Operator.LOGICAL_XOR:
                        result = (a == 0 && b != 0 || a != 0 && b == 0) ? 1 : 0;
                        break;
                    case Operator.LOGICAL_IMPLICATION:
                        result = (a != 0 && b == 0) ? 0 : 1;
                        break;

                    default:
                        throw new ExpressionEvaluationException(ExpressionEvaluationFault.UNDEFINED_INT_OPERATOR, op.toString());
                        //result = 0;
                }

                return new Pair(result, environment);
            } else {
                throw new ExpressionEvaluationException(ExpressionEvaluationFault.UNKNOWN_NUMBER_TYPE);
                //return null;
            }
        } else {
            throw new ExpressionEvaluationException(ExpressionEvaluationFault.UNKNOWN_OBJ_TYPE);
            //return null;
        }
    }

    private Object evaluate(Environment environment, Integer op, Object obj) {
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
                    case Operator.INCREMENT:
                        environment.setVariable(name, a + 1);
                        result = ((Number) environment.getVariable(name)).doubleValue();
                        break;
                    case Operator.DECREMENT:
                        environment.setVariable(name, a - 1);
                        result = ((Number) environment.getVariable(name)).doubleValue();
                        break;
                    case Operator.LOGICAL_NOT:
                        result = a != 0 ? 0 : 1;
                        break;

                    default:
                        throw new ExpressionEvaluationException(ExpressionEvaluationFault.UNDEFINED_FLOAT_OPERATOR, op.toString());
                        //result = 0;
                }

                return result;
            } else if (num instanceof Integer) {
                var a = num.intValue();
                var result = 0;

                switch (op) {
                    case Operator.INCREMENT:
                        environment.setVariable(name, a + 1);
                        result = ((Number) environment.getVariable(name)).intValue();
                        break;
                    case Operator.DECREMENT:
                        environment.setVariable(name, a - 1);
                        result = ((Number) environment.getVariable(name)).intValue();
                        break;
                    case Operator.BITWISE_NOT:
                        result = ~a;
                        break;
                    case Operator.LOGICAL_NOT:
                        result = a != 0 ? 0 : 1;
                        break;

                    default:
                        throw new ExpressionEvaluationException(ExpressionEvaluationFault.UNDEFINED_INT_OPERATOR, op.toString());
                        //result = 0;
                }

                return result;
            } else {
                throw new ExpressionEvaluationException(ExpressionEvaluationFault.UNKNOWN_NUMBER_TYPE);
                //return null;
            }
        } else {
            throw new ExpressionEvaluationException(ExpressionEvaluationFault.UNKNOWN_OBJ_TYPE);
            //return null;
        }
    }
}