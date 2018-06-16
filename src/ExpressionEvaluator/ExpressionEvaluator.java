package ExpressionEvaluator;

import javafx.util.Pair;
import java.util.ArrayList;
import java.util.HashMap;

public class ExpressionEvaluator {
    HashMap<String, Integer> opPriority;

    public ExpressionEvaluator() {
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
    }

    public Object evaluateExpression(ArrayList<Pair<Object, Integer>> tokens, int startingIndex, int finishingIndex) {
        ArrayList<Object> objectStack = new ArrayList<>();
        ArrayList<Object> opStack = new ArrayList<>();

        for (int i = startingIndex; i <= finishingIndex; ++i) {
            var token = tokens.get(i);

            switch (token.getValue()) {
                case 0:
                    objectStack.add(token.getKey());
                    break;
                case 1:
                    //TODO: Check that variable is defined in the environment
                    objectStack.add(token.getKey());
                    break;
                case 2:
                    var op = token.getKey();
                    if (op.equals("(")) {
                        opStack.add(op);
                    } else if (op.equals(")")) {
                        op = opStack.get(opStack.size() - 1);
                        opStack.remove(opStack.size() - 1);

                        while (!op.equals("(")) {
                            var a = objectStack.get(objectStack.size() - 2);
                            var b = objectStack.get(objectStack.size() - 1);

                            objectStack.remove(objectStack.size() - 2);
                            objectStack.remove(objectStack.size() - 1);

                            objectStack.add(evaluate((String) op, a, b));

                            op = opStack.get(opStack.size() - 1);
                            opStack.remove(opStack.size() - 1);
                        }
                    } else {
                        Object prevOp;

                        if (!opStack.isEmpty()) {
                            prevOp = opStack.get(opStack.size() - 1);
                        } else {
                            prevOp = null;
                        }

                        while (prevOp != null && !prevOp.equals("(") && opPriority.get(prevOp) < opPriority.get(op)){
                            if (i < finishingIndex) {
                                var nextToken = tokens.get(i + 1);

                                if (nextToken.getKey() == "(") {
                                    int j = finishingIndex;
                                    while (tokens.get(j).getKey() != ")") {
                                        --j;
                                    }

                                    var a = objectStack.get(objectStack.size() - 1);
                                    objectStack.remove(objectStack.size() - 1);

                                    var b = evaluateExpression(tokens, i + 1, j);
                                    objectStack.add(evaluate((String) prevOp, a, b));

                                    opStack.remove(opStack.size() - 1);
                                } else {
                                    var a = objectStack.get(objectStack.size() - 2);
                                    var b = objectStack.get(objectStack.size() - 1);

                                    objectStack.remove(objectStack.size() - 2);
                                    objectStack.remove(objectStack.size() - 1);

                                    objectStack.add(evaluate((String) prevOp, a, b));

                                    opStack.remove(opStack.size() - 1);

                                    if (!opStack.isEmpty()) {
                                        prevOp = opStack.get(opStack.size() - 1);
                                        opStack.remove(opStack.size() - 1);
                                    } else {
                                        prevOp = null;
                                    }
                                }
                            } else {
                                System.out.println("Something is wrong here");
                                return null;
                            }
                        }

                        opStack.add(op);
                    }
            }
        }

        while (!opStack.isEmpty()) {
            var op = opStack.get(opStack.size() - 1);
            opStack.remove(opStack.size() - 1);

            var a = objectStack.get(objectStack.size() - 2);
            var b = objectStack.get(objectStack.size() - 1);

            objectStack.remove(objectStack.size() - 2);
            objectStack.remove(objectStack.size() - 1);

            objectStack.add(evaluate((String) op, a, b));
        }

        if (objectStack.size() == 1) {
            return objectStack.get(0);
        } else {
            System.out.printf("Object stack has a wrong size! It is %d\n", objectStack.size());
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
