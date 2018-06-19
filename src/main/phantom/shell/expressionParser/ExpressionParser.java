package phantom.shell.expressionParser;

import phantom.shell.environment.Environment;
import phantom.shell.structures.Character;
import phantom.shell.structures.Operator;

import javafx.util.Pair;

import java.util.ArrayList;

public class ExpressionParser {

    Character character;
    Operator operator;

    public ExpressionParser() {
        character = new Character();
        operator = new Operator();
    }

    public ArrayList<Pair<Object, Integer>> parseInTokens(Environment environment, String expression) {
        ArrayList<Pair<Object, Integer>> tokens = new ArrayList<>();

        tokens.add(new Pair("(", 5));

        for (var currentIndex = 0; currentIndex < expression.length(); ++currentIndex) {
            var c = expression.charAt(currentIndex);

            if (character.isDigit(c)) {
                var result = readNumber(expression, currentIndex);
                var number = result.getKey();
                currentIndex = result.getValue() - 1;

                tokens.add(new Pair(number, 0));
            } else if (character.isLetter(c) || c == '_' || c == '$') {
                var word = readWord(expression, currentIndex);
                currentIndex += word.length() - 1;

                var variableValue = environment.getVariable(word);

                if (variableValue != null) {
                    tokens.add(new Pair(variableValue, 0));
                } else if (operator.isLogicalOperator(word)) {
                    tokens.add(new Pair(word, 4));
                } else {
                    // Throw some exception that variable is not defined

                    tokens.add(null);
                }
            } else if (character.isSymbol(c)) {
                var op = readOperator(expression, currentIndex);

                if (op != null) {
                    currentIndex += op.length() - 1;

                    if (operator.isUnaryOperator(op)) {
                        tokens.add(new Pair(op, 1));
                    } else if (operator.isBinaryOperator(op)) {
                        tokens.add(new Pair(op, 2));
                    } else if (operator.isComparisonOperator(op)) {
                        tokens.add(new Pair(op, 3));
                    } else if (operator.isLogicalOperator(op)) {
                        tokens.add(new Pair(op, 4));
                    } else { // Priority operator
                        tokens.add(new Pair(op, 5));
                    }
                } else {
                    tokens.add(new Pair(null, -1));
                }
            }
        }

        tokens.add(new Pair(")", 5));

        return tokens;
    }

    public Pair<Number, Integer> readNumber(String expression, int currentIndex) {
        var number = 0;
        var c = expression.charAt(currentIndex);

        boolean isInteger = true;

        while (currentIndex < expression.length() && character.isDigit(c)) {
            number = number * 10 + (int) (c - '0');

            ++currentIndex;
            if (currentIndex < expression.length()) {
                c = expression.charAt(currentIndex);
            }
        }

        if (currentIndex < expression.length() && c == '.' || c == ',') {
            isInteger = false;

            ++currentIndex;
            if (currentIndex < expression.length()) {
                c = expression.charAt(currentIndex);
            }
        }

        var fractional = 0.0;
        var divider = 10;

        while (currentIndex < expression.length() && character.isDigit(c)) {
            fractional = (fractional + (int) (c - '0')) / divider;
            divider *= 10;

            ++currentIndex;
            if (currentIndex < expression.length()) {
                c = expression.charAt(currentIndex);
            }
        }

        if (isInteger) {
            return new Pair(number, currentIndex);
        } else {
            return new Pair(number + fractional, currentIndex);
        }
    }

    public String readWord(String expression, int currentIndex) {
        var word = "";
        var c = expression.charAt(currentIndex);

        while (currentIndex < expression.length() && (character.isLetterOrDigit(c) || c == '_' || c == '$')) {
            word += c;

            ++currentIndex;
            if (currentIndex < expression.length()) {
                c = expression.charAt(currentIndex);
            }
        }

        return word;
    }

    public String readOperator(String expression, int currentIndex) {
        var op = "";
        var c = expression.charAt(currentIndex);

        while (currentIndex < expression.length() && (character.isSymbol(c))) {
            if ((operator.isOperator(op) || op.length() == 2) && !operator.isOperator(op + c)) {
                break;
            }

            op += c;

            ++currentIndex;
            if (currentIndex < expression.length()) {
                c = expression.charAt(currentIndex);
            }
        }

        if (operator.isOperator(op)) {
            return op;
        } else {
            return null;
        }

    }
}
