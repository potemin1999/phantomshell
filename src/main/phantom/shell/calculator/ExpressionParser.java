package phantom.shell.calculator;

import phantom.shell.parser.Token;
import phantom.shell.parser.TokenType;
import phantom.shell.structures.Character;
import phantom.shell.structures.Operator;

import phantom.support.io.PrintStream;
import phantom.support.util.ArrayList;
import phantom.support.util.List;
import phantom.support.util.Pair;


public class ExpressionParser {

    Character character;
    Operator operator;

    public ExpressionParser() {
        character = new Character();
        operator = new Operator();
    }

    public ArrayList<Pair<Object, Integer>> parseTokens(List<Token> tokens) {


        /*
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

                if (operator.isLogicalOperator(word)) {
                    tokens.add(new Pair(word, 4));
                } else if (operator.isUnaryOperator(word)) {
                    tokens.add(new Pair(word, 1));
                } else {
                    switch (word) {
                        case "true":
                            tokens.add(new Pair(1, 0));
                            break;
                        case "false":
                            tokens.add(new Pair(0, 0));
                            break;
                        default:
                            tokens.add(new Pair(word, 0));
                    }
                }
            } else if (character.isSymbol(c)) {
                var op = readOperator(expression, currentIndex);

                if (op != null) {
                    currentIndex += op.length() - 1;

                    if (operator.isUnaryOperator(op)) {
                        tokens.add(new Pair(op, 1));
                    } else if (operator.isIncrementDecrementOperator(op)) {
                        tokens.add(new Pair(op, 7));
                    } else if (operator.isBinaryOperator(op) && !op.equals("=")) {
                        tokens.add(new Pair(op, 2));
                    } else if (operator.isComparisonOperator(op)) {
                        tokens.add(new Pair(op, 3));
                    } else if (operator.isLogicalOperator(op)) {
                        tokens.add(new Pair(op, 4));
                    } else if (operator.isPriorityOperator(op)){
                        tokens.add(new Pair(op, 5));
                    } else if (op.equals("=")) {
                        tokens.add(new Pair(op, 6));
                    }
                } else {
                    tokens.add(new Pair(null, -1));
                }
            }
        }

        tokens.add(new Pair(")", 5));

        return tokens;
        */
        ArrayList<Pair<Object, Integer>> parsedTokens = new ArrayList<>();

        parsedTokens.add(new Pair<>("(", 5));

        while (tokens.size() > 0) {
            var token = tokens.getFirst();
            //System.out.println(token.getStringValue());
            tokens.removeFirst();

            switch (token.getType()) {
                case TokenType.IDENTIFIER:
                    var word = token.getStringValue();

                    switch (word) {
                        case "true":
                            parsedTokens.add(new Pair<>(1, 0));
                            break;
                        case "false":
                            parsedTokens.add(new Pair<>(0, 0));
                            break;
                        default:
                            if (character.isDigit(word.charAt(0))) {
                                parsedTokens.add(new Pair<>(readNumber(word), 0));
                            } else {
                                parsedTokens.add(new Pair<>(word, 0));
                            }
                    }

                    break;

                case TokenType.OPERATOR:
                    var op = token.getStringValue();

                    if (operator.isUnaryOperator(op)) {
                        parsedTokens.add(new Pair<>(op, 1));
                    } else if (operator.isIncrementDecrementOperator(op)) {
                        parsedTokens.add(new Pair<>(op, 7));
                    } else if (operator.isBinaryOperator(op) && !op.equals("=")) {
                        parsedTokens.add(new Pair<>(op, 2));
                    } else if (operator.isComparisonOperator(op)) {
                        parsedTokens.add(new Pair<>(op, 3));
                    } else if (operator.isLogicalOperator(op)) {
                        parsedTokens.add(new Pair<>(op, 4));
                    } else if (operator.isPriorityOperator(op)){
                        parsedTokens.add(new Pair<>(op, 5));
                    } else if (op.equals("=")) {
                        parsedTokens.add(new Pair<>(op, 6));
                    } else {
                        parsedTokens.add(new Pair<>(null, -1));
                    }

                    break;

                case TokenType.PAREN_OPEN:
                    parsedTokens.add(new Pair<>("(", 5));
                    break;

                case TokenType.PAREN_CLOSE:
                    parsedTokens.add(new Pair<>(")", 5));
                    break;

                case TokenType.EOL:
                    break;

                case TokenType.BRACKET_CLOSE:
                    break;

                default:
                    System.out.println("Unexpected token");
                    return null;
            }
        }

        parsedTokens.add(new Pair<>(")", 5));

        return parsedTokens;
    }

    public Number readNumber(String expression) {
        var number = 0;
        var currentIndex = 0;
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
            return number;
        } else {
            return number + fractional;
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
