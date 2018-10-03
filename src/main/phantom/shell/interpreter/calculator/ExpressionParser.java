package phantom.shell.interpreter.calculator;

import phantom.shell.parser.Token;
import phantom.shell.parser.TokenType;
import phantom.shell.structures.Character;
import phantom.shell.structures.Operator;
import phantom.shell.values.*;

import phantom.support.util.ArrayList;
import phantom.support.util.List;


public class ExpressionParser {

    Character character;
    Operator operator;

    public ExpressionParser() {
        character = new Character();
        operator = new Operator();
    }

    public ArrayList<ExpressionToken> parseTokens(List<Token> tokens) {

        ArrayList<ExpressionToken> parsedTokens = new ArrayList<>();

        parsedTokens.add(new ExpressionToken(5, Operator.PAREN_OPEN));

        while (tokens.size() > 0) {
            var token = tokens.removeFirst();

            switch (token.getType()) {
                case TokenType.IDENTIFIER:
                    var word = token.getValue();
                    //TODO: get rid of string type
                    var wordStr = new String(word);
                    switch (wordStr) {
                        case "true":
                            parsedTokens.add(new ExpressionToken(0, new BoolValue(true)));
                            break;
                        case "false":
                            parsedTokens.add(new ExpressionToken(0, new BoolValue(false)));
                            break;
                        default:
                            if (character.isDigit(word[0])) {
                                parsedTokens.add(new ExpressionToken(0, readNumber(word)));
                            } else {
                                parsedTokens.add(new ExpressionToken(0, new StringValue(word)));
                            }
                    }

                    break;

                case TokenType.OPERATOR:
                    var op = token.getStringValue();
                    var opcode = operator.map(op);
                    var operatorType = getOperatorType(op);
                    if (operatorType == -1) {
                        parsedTokens.add(new ExpressionToken(-1, null));
                    } else {
                        parsedTokens.add(new ExpressionToken(operatorType, opcode));
                    }

                    break;

                case TokenType.PAREN_OPEN:
                    parsedTokens.add(new ExpressionToken(5, Operator.PAREN_OPEN));
                    break;

                case TokenType.PAREN_CLOSE:
                    parsedTokens.add(new ExpressionToken(5, Operator.PAREN_CLOSE));
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
        parsedTokens.add(new ExpressionToken(5, Operator.PAREN_CLOSE));

        return parsedTokens;
    }

    public int getOperatorType(String op) {
        if (operator.isUnaryOperator(op)) {
            return 1;
        } else if (operator.isIncrementDecrementOperator(op)) {
            return 7;
        } else if (operator.isBinaryOperator(op) && !op.equals("=")) {
            return 2;
        } else if (operator.isComparisonOperator(op)) {
            return 3;
        } else if (operator.isLogicalOperator(op)) {
            return 4;
        } else if (operator.isPriorityOperator(op)) {
            return 5;
        } else if (op.equals("=")) {
            return 6;
        } else {
            return -1;
        }
    }

    public Value readNumber(char[] expression) {
        return readNumber(String.valueOf(expression));
    }

    public Value readNumber(String expression) {
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
            fractional += (double) (c - '0') / divider;
            divider *= 10;

            ++currentIndex;
            if (currentIndex < expression.length()) {
                c = expression.charAt(currentIndex);
            }
        }

        if (isInteger) {
            return new IntValue(number);
        } else {
            return new FloatValue(number + fractional);
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
