package phantom.shell.calculator;

import phantom.shell.environment.Environment;
import phantom.shell.structures.Operator;

import phantom.shell.values.BoolValue;
import phantom.shell.values.FloatValue;
import phantom.shell.values.IntValue;
import phantom.shell.values.Value;
import phantom.support.util.Pair;
import phantom.support.util.ArrayList;
import phantom.support.util.List;


public class ExpressionEvaluator {

    private Operator operator;

    public ExpressionEvaluator() {
        operator = new Operator();
    }

    private Pair<Value, Environment> evaluateExpression(Environment environment, ArrayList<Pair<Value, Integer>> tokens,
                                                        int startingIndex, int finishingIndex,
                                                        List<Value> postChangingObjectStack, List<Value> postOpStack) {
        ArrayList<Value> objectStack = new ArrayList<>();

        ArrayList<IntValue> opStack = new ArrayList<>();
        ArrayList<IntValue> logicalOpStack = new ArrayList<>();
        ArrayList<IntValue> assigningOpStack = new ArrayList<>();

        IntValue comparisonOp = null;
        Value objectToCompare = null;

        for (int i = startingIndex; i <= finishingIndex; ++i) {
            var token = tokens.get(i);
            //System.out.println(token.getKey().getValue() + " " + token.getValue());

            switch (token.getValue()) {
                case 0: // Object
                    objectStack.add(token.getKey());
                    break;

                case 1: // Unary operator

                    IntValue op = (IntValue) token.getKey();
                    Pair<Value, Integer> nextToken = null;

                    if (i < tokens.size() - 1) {
                        nextToken = tokens.get(i + 1);
                    }

                    if (nextToken != null) {

                        if (nextToken.getValue() == 0) {
                            objectStack.add(evaluate(environment, op, nextToken.getKey()));
                            ++i;
                        } else if (nextToken.getKey().operatorEqual(new IntValue(Operator.PAREN_OPEN)).getValue()) {

                            var j = i + 2;
                            var balance = 1;

                            while (j <= finishingIndex && balance != 0) {
                                if (tokens.get(j).getKey().getValue().equals("(")) {
                                    ++balance;
                                } else if (tokens.get(j).getKey().getValue().equals(")")) {
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
                    op = (IntValue) token.getKey();

                    IntValue prevOp;

                    if (!opStack.isEmpty()) {
                        prevOp = opStack.get(opStack.size() - 1);
                    } else {
                        prevOp = null;
                    }

                    while (prevOp != null && prevOp.getValue() != Operator.PAREN_OPEN && operator.getPriority(prevOp) <= operator.getPriority(op)) {
                        Value a = objectStack.get(objectStack.size() - 2);
                        Value b = objectStack.get(objectStack.size() - 1);

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

                    if (nextToken.getValue() == 5 && nextToken.getKey().operatorEqual(new IntValue(Operator.PAREN_OPEN)).getValue()) {
                        var j = i + 2;
                        var balance = 1;

                        while (j <= finishingIndex && balance != 0) {
                            if (tokens.get(j).getKey().operatorEqual(new IntValue(Operator.PAREN_OPEN)).getValue()) {
                                ++balance;
                            } else if (tokens.get(j).getKey().operatorEqual(new IntValue(Operator.PAREN_CLOSE)).getValue()) {
                                --balance;
                            }
                            ++j;
                        }
                        --j;

                        Value a = objectStack.get(objectStack.size() - 1);
                        objectStack.remove(objectStack.size() - 1);

                        var pair = evaluateExpression(environment, tokens, i + 1, j, postChangingObjectStack, postOpStack);
                        Value b = pair.getKey();
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
                    op = (IntValue) token.getKey();
                    opStack.add(op);
                    break;

                case 4: // Logical operator
                    while (!opStack.isEmpty()) {
                        op = opStack.get(opStack.size() - 1);

                        if (op.getValue() == Operator.PAREN_OPEN) {
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
                            Value a = objectStack.get(objectStack.size() - 1);
                            Value b = objectToCompare;

                            objectStack.remove(objectStack.size() - 1);

                            var pair = evaluate(environment, comparisonOp, a, b);
                            objectStack.add(pair.getKey());
                            environment = pair.getValue();

                            comparisonOp = null;
                            objectToCompare = null;
                        } else {
                            throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION,"8");
                        }
                    }

                    op = (IntValue) token.getKey();

                    if (!logicalOpStack.isEmpty()) {
                        prevOp = logicalOpStack.get(logicalOpStack.size() - 1);
                    } else {
                        prevOp = null;
                    }

                    while (prevOp != null && prevOp.getValue() != Operator.PAREN_OPEN && operator.getPriority(prevOp) <= operator.getPriority(op)) {
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

                    if (((IntValue) nextToken.getKey()).getValue() == Operator.PAREN_OPEN) {
                        var j = i + 2;
                        var balance = 1;

                        while (j <= finishingIndex && balance != 0) {
                            if (tokens.get(j).getKey().getValue().equals("(")) {
                                ++balance;
                            } else if (tokens.get(j).getKey().getValue().equals(")")) {
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
                    op = (IntValue) token.getKey();
                    if (op.getValue() == Operator.PAREN_OPEN) {
                        opStack.add(op);
                        logicalOpStack.add(op);
                    } else if (op.getValue() == Operator.PAREN_CLOSE) {
                        if (!opStack.isEmpty()) {
                            op = opStack.get(opStack.size() - 1);
                            opStack.remove(opStack.size() - 1);
                        } else {
                            throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "2");
                            //return null;
                        }

                        while (op.getValue() != Operator.PAREN_OPEN) {
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
                            }
                        }

                        if (!logicalOpStack.isEmpty()) {
                            op = logicalOpStack.get(logicalOpStack.size() - 1);
                            logicalOpStack.remove(logicalOpStack.size() - 1);
                        } else {
                            throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "6");
                        }

                        while (op.getValue() != Operator.PAREN_OPEN) {
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
                    op = (IntValue) token.getKey();
                    assigningOpStack.add(op);
                    break;

                case 7: // Increment/decrement operator
                    op = (IntValue) token.getKey();
                    Pair<Value, Integer> prevToken = null;
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
                    }
            }
        }

        while (!opStack.isEmpty()) {
            var op = opStack.get(opStack.size() - 1);
            opStack.remove(opStack.size() - 1);

            if (op.getValue() == Operator.PAREN_OPEN) {
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
                }

                comparisonOp = op;

                if (!objectStack.isEmpty()) {
                    objectToCompare = objectStack.get(objectStack.size() - 1);
                    objectStack.remove(objectStack.size() - 1);
                } else {
                    throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "7");
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
            }
        }

        while (!logicalOpStack.isEmpty()) {
            var op = logicalOpStack.get(logicalOpStack.size() - 1);
            logicalOpStack.remove(logicalOpStack.size() - 1);

            if (op.getValue() == Operator.PAREN_OPEN) {
                throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "10");
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
        }
    }

    public Pair<Value, Environment> evaluateExpression(Environment environment, ArrayList<Pair<Value, Integer>> tokens) {
        ArrayList<Value> postChangingObjectStack = new ArrayList<>();
        ArrayList<Value> postOpStack = new ArrayList<>();

        var pair = evaluateExpression(environment, tokens, 0, tokens.size() - 1, postChangingObjectStack, postOpStack);
        var result = pair.getKey();
        environment = pair.getValue();

        while (!postOpStack.isEmpty()) {
            var op = (IntValue) postOpStack.get(postOpStack.size() - 1);
            postOpStack.remove(postOpStack.size() - 1);

            var a = postChangingObjectStack.get(postChangingObjectStack.size() - 1);

            postChangingObjectStack.remove(postChangingObjectStack.size() - 1);

            evaluate(environment, op, a);
        }

        return new Pair<>(result, environment);
    }

    private Pair<Value, Environment> evaluate(Environment environment, IntValue op, Value obj1, Value obj2) {
        Value val2 = environment.getVariable(obj2);

        if (op.getValue() == Operator.ASSIGNING) {
            environment.setVariable(obj1, obj2);

            return new Pair(val2, environment);
        }

        var val1 = environment.getVariable(obj1);

        Value result;

        //System.out.println(val1);
        //System.out.println(val2);

        switch (op.getValue()) {
            case Operator.ADDITION:
                result = val1.operatorAddition(val2);
                break;
            case Operator.SUBTRACTION:
                result = val1.operatorSubtraction(val2);
                break;
            case Operator.MULTIPLICATION:
                result = val1.operatorMultiplication(val2);
                break;
            case Operator.DIVISION:
                result = val1.operatorDivision(val2);
                break;

            case Operator.BITWISE_AND:
                result = val1.operatorBitwiseAnd(val2);
                break;
            case Operator.BITWISE_OR:
                result = val1.operatorBitwiseOr(val2);
                break;
            case Operator.BITWISE_XOR:
                result = val1.operatorBitwiseXor(val2);
                break;
            case Operator.BITWISE_SHIFT_L:
                result = val1.operatorBitwiseShiftLeft(val2);
                break;
            case Operator.BITWISE_SHIFT_R:
                result = val1.operatorBitwiseShiftRight(val2);
                break;

            case Operator.EQUAL:
                result = val1.operatorEqual(val2);
                break;
            case Operator.NOT_EQUAL:
                result = val1.operatorNotEqual(val2);
                break;
            case Operator.GREATER_THAN:
                result = val1.operatorGreaterThan(val2);
                break;
            case Operator.NOT_LESS_THAN:
                result = val1.operatorNotLessThan(val2);
                break;
            case Operator.LESS_THAN:
                result = val1.operatorLessThan(val2);
                break;
            case Operator.NOT_GREATER_THAN:
                result = val1.operatorNotGreaterThan(val2);
                break;

            case Operator.LOGICAL_AND:
                result = val1.operatorLogicalAnd(val2);
                break;
            case Operator.LOGICAL_OR:
                result = val1.operatorLogicalOr(val2);
                break;
            case Operator.LOGICAL_XOR:
                result = val1.operatorLogicalXor(val2);
                break;
            case Operator.LOGICAL_IMPLICATION:
                result = val1.operatorLogicalImplication(val2);
                break;

            default:
                throw new ExpressionEvaluationException(ExpressionEvaluationFault.UNDEFINED_INT_OPERATOR, op.toString());
        }

        return new Pair(result, environment);

    }

    private Object evaluate(Environment environment, IntValue op, Value obj) {
        Value name = obj;

        Value val = environment.getVariable(name);
        if (val != null) {
            obj = val;
        }

        if (obj instanceof IntValue || obj instanceof FloatValue || obj instanceof BoolValue) {
            Value result;

            switch (op.getValue()) {
                case Operator.INCREMENT:
                    result = obj.operatorIncrement();
                    environment.setVariable(name, result);
                    break;
                case Operator.DECREMENT:
                    result = obj.operatorDecrement();
                    environment.setVariable(name, result);
                    break;
                case Operator.BITWISE_NOT:
                    result = obj.operatorBitwiseNot();
                    break;
                case Operator.LOGICAL_NOT:
                    result = obj.operatorLogicalNot();
                    break;

                default:
                    throw new ExpressionEvaluationException(ExpressionEvaluationFault.UNDEFINED_INT_OPERATOR, op.toString());
            }

            return result;

        } else {
            throw new ExpressionEvaluationException(ExpressionEvaluationFault.UNKNOWN_OBJ_TYPE);
        }
    }
}