package phantom.shell.interpreter.calculator;

import phantom.shell.interpreter.environment.Environment;
import phantom.shell.structures.Operator;

import phantom.shell.values.BoolValue;
import phantom.shell.values.FloatValue;
import phantom.shell.values.IntValue;
import phantom.shell.values.Value;
import phantom.support.util.ArrayList;
import phantom.support.util.List;


public class ExpressionEvaluator {

    private Operator operator;

    public ExpressionEvaluator() {
        operator = new Operator();
    }

    private Value evaluateExpression(Environment environment, ArrayList<ExpressionToken> tokens,
                                     int startingIndex, int finishingIndex,
                                     List<Value> postChangingObjectStack, List<Integer> postOpStack) {
        ArrayList<Value> objectStack = new ArrayList<>();

        ArrayList<Integer> opStack = new ArrayList<>();
        ArrayList<Integer> logicalOpStack = new ArrayList<>();
        ArrayList<Integer> assigningOpStack = new ArrayList<>();

        int comparisonOp = 0;
        Value objectToCompare = null;

        for (int i = startingIndex; i <= finishingIndex; ++i) {
            var token = tokens.get(i);
            int fff = 0;
            //System.out.println(token.getKey().getValue() + " " + token.getValue());

            switch (token.getOpType()) {
                case ExpressionToken.OPTYPE_OBJECT: { // 0 - Object
                    objectStack.add(token.getValue());
                    break;
                }
                case ExpressionToken.OPTYPE_UNARY_OPERATOR: { // 1 - Unary operator

                    int op = token.getOpCode();
                    //IntValue op = (IntValue) token.getValue();
                    ExpressionToken nextToken = null;
                    //Pair<Value, Integer> nextToken = null;

                    if (i < tokens.size() - 1) {
                        nextToken = tokens.get(i + 1);
                    }

                    if (nextToken != null) {

                        if (nextToken.getOpType() == 0) {
                            objectStack.add(evaluate(environment, op, nextToken.getValue()));
                            ++i;
                        } else if (nextToken.getOpCode() == Operator.PAREN_OPEN) {
                            var j = getBalance(i, tokens, finishingIndex);

                            var a = evaluateExpression(environment, tokens, i + 1, j, postChangingObjectStack, postOpStack);

                            objectStack.add(evaluate(environment, op, a));

                            i = j;
                        }
                    }
                    break;
                }
                case ExpressionToken.OPTYPE_BINARY_OPERATOR: { // 2 - Binary operator
                    int op = token.getOpCode();
                    int prevOp = 0;

                    if (!opStack.isEmpty()) {
                        prevOp = opStack.get(opStack.size() - 1);
                    }

                    while (prevOp != 0 && prevOp != Operator.PAREN_OPEN
                            && operator.getPriority(prevOp) <= operator.getPriority(op)) {
                        Value a = objectStack.get(objectStack.size() - 2);
                        Value b = objectStack.get(objectStack.size() - 1);

                        objectStack.remove(objectStack.size() - 2);
                        objectStack.remove(objectStack.size() - 1);

                        var resultValue = evaluate(environment, prevOp, a, b);
                        objectStack.add(resultValue);

                        opStack.remove(opStack.size() - 1);

                        if (!opStack.isEmpty()) {
                            prevOp = opStack.get(opStack.size() - 1);
                        } else {
                            prevOp = 0;
                        }
                    }

                    var nextToken = tokens.get(i + 1);

                    if (nextToken.getOpType() == 5 && nextToken.getOpCode() == Operator.PAREN_OPEN) {
                        var j = getBalance(i, tokens, finishingIndex);

                        Value a = objectStack.get(objectStack.size() - 1);
                        objectStack.remove(objectStack.size() - 1);

                        var b = evaluateExpression(environment, tokens, i + 1, j, postChangingObjectStack, postOpStack);

                        var resultValue = evaluate(environment, op, a, b);
                        objectStack.add(resultValue);

                        i = j;

                        break;
                    }

                    opStack.add(op);
                    break;
                }
                case ExpressionToken.OPTYPE_COMPARISON_OPERATOR: { // 3 - Comparison operator
                    opStack.add(token.getOpCode());
                    break;
                }
                case ExpressionToken.OPTYPE_LOGICAL_OPERATOR: { // Logical operator
                    int op;
                    int prevOp;
                    ExpressionToken nextToken;
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
                            if (comparisonOp != 0) {
                                throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION,
                                        "" + ExpressionToken.OPTYPE_COMPARISON_OPERATOR);
                                //return null;
                            }

                            comparisonOp = op;

                            if (!objectStack.isEmpty()) {
                                objectToCompare = objectStack.get(objectStack.size() - 1);
                                objectStack.remove(objectStack.size() - 1);
                            } else {
                                throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION,
                                        "" + ExpressionToken.OPTYPE_INCDEC_OPERATOR);
                            }

                            continue;
                        }

                        var a = objectStack.get(objectStack.size() - 2);
                        var b = objectStack.get(objectStack.size() - 1);

                        objectStack.remove(objectStack.size() - 2);
                        objectStack.remove(objectStack.size() - 1);

                        var resultValue = evaluate(environment, op, a, b);
                        objectStack.add(resultValue);
                        //environment = pair.getValue();
                    }

                    if (comparisonOp != 0) {
                        if (!objectStack.isEmpty()) {
                            Value a = objectStack.get(objectStack.size() - 1);
                            Value b = objectToCompare;

                            objectStack.remove(objectStack.size() - 1);

                            var resultValue = evaluate(environment, comparisonOp, a, b);
                            objectStack.add(resultValue);
                            //environment = pair.getValue();

                            comparisonOp = 0;
                            objectToCompare = null;
                        } else {
                            throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "8");
                        }
                    }

                    op = token.getOpCode();

                    if (!logicalOpStack.isEmpty()) {
                        prevOp = logicalOpStack.get(logicalOpStack.size() - 1);
                    } else {
                        prevOp = 0;
                    }

                    while (prevOp != 0 && prevOp != Operator.PAREN_OPEN
                            && operator.getPriority(prevOp) <= operator.getPriority(op)) {
                        var a = objectStack.get(objectStack.size() - 2);
                        var b = objectStack.get(objectStack.size() - 1);

                        objectStack.remove(objectStack.size() - 2);
                        objectStack.remove(objectStack.size() - 1);

                        var resultValue = evaluate(environment, prevOp, a, b);
                        objectStack.add(resultValue);

                        logicalOpStack.remove(logicalOpStack.size() - 1);

                        if (!logicalOpStack.isEmpty()) {
                            prevOp = logicalOpStack.get(logicalOpStack.size() - 1);
                        } else {
                            prevOp = 0;
                        }
                    }


                    nextToken = tokens.get(i + 1);

                    if (nextToken.getOpCode() == Operator.PAREN_OPEN) {
                        var j = getBalance(i, tokens, finishingIndex);

                        var a = objectStack.get(objectStack.size() - 1);
                        objectStack.remove(objectStack.size() - 1);

                        var b = evaluateExpression(environment, tokens, i + 1, j, postChangingObjectStack, postOpStack);

                        var resultObject = evaluate(environment, op, a, b);
                        objectStack.add(resultObject);
                        //environment = pair.getValue();

                        i = j;

                        break;
                    }

                    logicalOpStack.add(op);
                    break;
                }
                case ExpressionToken.OPTYPE_PRIORITY_OPERATOR: { // Priority operator
                    int op = token.getOpCode();
                    //op = (IntValue) token.getKey();
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
                                if (comparisonOp != 0) {
                                    throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "13");
                                    //return null;
                                }

                                comparisonOp = op;

                                if (!objectStack.isEmpty()) {
                                    objectToCompare = objectStack.get(objectStack.size() - 1);
                                    objectStack.remove(objectStack.size() - 1);
                                } else {
                                    throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "4");
                                }

                                op = opStack.get(opStack.size() - 1);
                                opStack.remove(opStack.size() - 1);
                                continue;
                            }

                            var a = objectStack.get(objectStack.size() - 2);
                            var b = objectStack.get(objectStack.size() - 1);

                            objectStack.remove(objectStack.size() - 2);
                            objectStack.remove(objectStack.size() - 1);

                            var resultValue = evaluate(environment, op, a, b);
                            objectStack.add(resultValue);
                            //environment = pair.getValue();

                            op = opStack.get(opStack.size() - 1);
                            opStack.remove(opStack.size() - 1);
                        }

                        if (comparisonOp != 0) {
                            if (!objectStack.isEmpty()) {
                                var a = objectStack.get(objectStack.size() - 1);
                                var b = objectToCompare;

                                objectStack.remove(objectStack.size() - 1);

                                var resultValue = evaluate(environment, comparisonOp, a, b);
                                objectStack.add(resultValue);
                                //environment = pair.getValue();

                                comparisonOp = 0;
                                objectToCompare = null;
                            } else {
                                throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "5");
                            }
                        }

                        if (!logicalOpStack.isEmpty()) {
                            op = logicalOpStack.get(logicalOpStack.size() - 1);
                            logicalOpStack.remove(logicalOpStack.size() - 1);
                        } else {
                            throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "6");
                        }

                        while (op != Operator.PAREN_OPEN) {
                            var a = objectStack.get(objectStack.size() - 2);
                            var b = objectStack.get(objectStack.size() - 1);

                            objectStack.remove(objectStack.size() - 2);
                            objectStack.remove(objectStack.size() - 1);

                            var resultValue = evaluate(environment, op, a, b);
                            objectStack.add(resultValue);
                            //environment = pair.getValue();

                            op = logicalOpStack.get(logicalOpStack.size() - 1);
                            logicalOpStack.remove(logicalOpStack.size() - 1);
                        }
                    }
                    break;
                }
                case ExpressionToken.OPTYPE_ASSIGN_OPERATOR: { // Assign operator
                    //op = (IntValue) token.getKey();
                    assigningOpStack.add(token.getOpCode());
                    break;
                }
                case ExpressionToken.OPTYPE_INCDEC_OPERATOR: { // Increment/decrement operator
                    int op = token.getOpCode();
                    ExpressionToken prevToken = null;
                    ExpressionToken nextToken = null;

                    if (i > 0) {
                        prevToken = tokens.get(i - 1);
                    }

                    if (i < tokens.size() - 1) {
                        nextToken = tokens.get(i + 1);
                    }

                    if (nextToken != null && nextToken.getOpType() == 0) {        // Preincrement/predecrement
                        objectStack.add(evaluate(environment, op, nextToken.getValue()));
                        ++i;
                    } else if (prevToken != null && prevToken.getOpType() == 0) { // Postincrement/postdecrement
                        postOpStack.add(op);
                        postChangingObjectStack.add(prevToken.getValue());
                    } else {
                        throw new ExpressionEvaluationException("Expected variable");
                    }
                    break;
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
                if (comparisonOp != 0) {
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

            var resultValue = evaluate(environment, op, a, b);
            objectStack.add(resultValue);
            //environment = pair.getValue();
        }

        if (comparisonOp != 0) {
            if (!objectStack.isEmpty()) {
                var a = objectStack.get(objectStack.size() - 1);

                objectStack.remove(objectStack.size() - 1);

                var resultValue = evaluate(environment, comparisonOp, a, objectToCompare);
                objectStack.add(resultValue);
                //environment = pair.getValue();
            } else {
                throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "8");
            }
        }

        while (!logicalOpStack.isEmpty()) {
            var op = logicalOpStack.get(logicalOpStack.size() - 1);
            logicalOpStack.remove(logicalOpStack.size() - 1);

            if (op == Operator.PAREN_OPEN) {
                throw new ExpressionEvaluationException(ExpressionEvaluationFault.INVALID_EXPRESSION, "10");
            }

            var a = objectStack.get(objectStack.size() - 2);
            var b = objectStack.get(objectStack.size() - 1);

            objectStack.remove(objectStack.size() - 2);
            objectStack.remove(objectStack.size() - 1);

            var resultValue = evaluate(environment, op, a, b);
            objectStack.add(resultValue);
            //environment = pair.getValue();
        }

        while (!assigningOpStack.isEmpty()) {
            var op = assigningOpStack.get(assigningOpStack.size() - 1);
            assigningOpStack.remove(assigningOpStack.size() - 1);

            var a = objectStack.get(objectStack.size() - 2);
            var b = objectStack.get(objectStack.size() - 1);

            objectStack.remove(objectStack.size() - 2);
            objectStack.remove(objectStack.size() - 1);

            var resultValue = evaluate(environment, op, a, b);
            objectStack.add(resultValue);
            //environment = pair.getValue();
        }

        if (objectStack.size() == 1) {
            var obj = objectStack.get(0);

            var val = environment.getVariable(obj);
            return val == null ? obj : val;
        } else if (objectStack.size() == 0) {
            return null;
        } else {
            throw new ExpressionEvaluationException(ExpressionEvaluationFault.OBJ_STACK_WRONG_SIZE, "" + objectStack.size());
        }
    }

    public Value evaluateExpression(Environment environment, ArrayList<ExpressionToken> tokens) {
        ArrayList<Value> postChangingObjectStack = new ArrayList<>();
        ArrayList<Integer> postOpStack = new ArrayList<>();

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

    private Value evaluate(Environment environment, int opCode, Value obj1, Value obj2) {
        Value val2 = environment.getVariable(obj2);

        if (opCode == Operator.ASSIGNING) {
            environment.setVariable(obj1, obj2);
            return val2;
        }

        var val1 = environment.getVariable(obj1);
        Value result;

        result = operator.executeBinaryOperator(opCode, val1, val2);

        return result;

    }

    private Value evaluate(Environment environment, int opcode, Value obj) {
        Value name = obj;

        Value val = environment.getVariable(name);
        if (val != null) {
            obj = val;
        }

        if (obj instanceof IntValue || obj instanceof FloatValue || obj instanceof BoolValue) {

            var result = operator.executeUnaryOperator(opcode, obj);
            if (operator.isIncrementDecrementOperator(opcode)) {
                environment.setVariable(name, result);
            }

            return result;

        } else {
            throw new ExpressionEvaluationException(ExpressionEvaluationFault.UNKNOWN_OBJ_TYPE);
        }
    }

    private int getBalance(int i, ArrayList<ExpressionToken> tokens, int finishingIndex) {
        var j = i + 2;
        var balance = 1;

        while (j <= finishingIndex && balance != 0) {
            if (tokens.get(j).getOpCode() == Operator.PAREN_OPEN) {
                ++balance;
            }
            if (tokens.get(j).getOpCode() == Operator.PAREN_CLOSE) {
                --balance;
            }
            ++j;
        }
        --j;
        return j;
    }

}