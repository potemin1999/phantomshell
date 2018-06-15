package ExpressionParser;

import java.util.ArrayList;

public class ExpressionParser {
    public ArrayList<String> parseInTokens(String expression) {
        ArrayList<String> tokens = new ArrayList<>();

        for (var currentIndex = 0; currentIndex < expression.length(); ++currentIndex) {
            var c = expression.charAt(currentIndex);

            if (Character.isDigit(c)) {
                var number = readNumber(expression, currentIndex);
                currentIndex += number.length() - 1;

                tokens.add(number);
            } else if (Character.isLetter(c) || c == '_' || c == '$') {
                var word = readWord(expression, currentIndex);
                currentIndex += word.length() - 1;

                tokens.add(word);
            } else if (Character.isSymbol(c)) {
                var operator = readOperator(expression, currentIndex);

                if (operator != null) {
                    currentIndex += operator.length() - 1;
                }

                tokens.add(operator);
            }
        }

        return tokens;
    }

    public String readNumber(String expression, int currentIndex) {
        var number = "";
        var c = expression.charAt(currentIndex);

        while (currentIndex < expression.length() && Character.isDigit(c)) {
            number += c;

            ++currentIndex;
            if (currentIndex < expression.length()) {
                c = expression.charAt(currentIndex);
            }
        }

        return number;
    }

    public String readWord(String expression, int currentIndex) {
        var word = "";
        var c = expression.charAt(currentIndex);

        while (currentIndex < expression.length() && (Character.isLetterOrDigit(c) || c == '_' || c == '$')) {
            word += c;

            ++currentIndex;
            if (currentIndex < expression.length()) {
                c = expression.charAt(currentIndex);
            }
        }

        return word;
    }

    public String readOperator(String expression, int currentIndex) {
        var operator = "";
        var c = expression.charAt(currentIndex);

        while (currentIndex < expression.length() && (Character.isSymbol(c))) {
            if ((Operator.isOperator(operator) || operator.length() == 2) && !Operator.isOperator(operator + c)) {
                break;
            }

            operator += c;

            ++currentIndex;
            if (currentIndex < expression.length()) {
                c = expression.charAt(currentIndex);
            }
        }

        if (Operator.isOperator(operator)) {
            return operator;
        } else {
            return null;
        }

    }
}
