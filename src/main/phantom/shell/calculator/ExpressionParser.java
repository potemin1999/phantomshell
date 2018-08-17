package phantom.shell.calculator;
import phantom.shell.parser.Token;
import phantom.shell.parser.TokenType;
import phantom.shell.structures.Character;
import phantom.shell.structures.Operator;
import phantom.shell.values.*;

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

    public ArrayList<Pair<Value, Integer>> parseTokens(List<Token> tokens) {

        ArrayList<Pair<Value, Integer>> parsedTokens = new ArrayList<>();

        parsedTokens.add(new Pair<>(new IntValue(Operator.PAREN_OPEN), 5));

        while (tokens.size() > 0) {
            var token = tokens.getFirst();
            tokens.removeFirst();

            switch (token.getType()) {
                case TokenType.IDENTIFIER:
                    var word = token.getStringValue();

                    switch (word) {
                        case "true":
                            parsedTokens.add(new Pair<>(new BoolValue(true), 0));
                            break;
                        case "false":
                            parsedTokens.add(new Pair<>(new BoolValue(false), 0));
                            break;
                        default:
                            if (character.isDigit(word.charAt(0))) {
                                parsedTokens.add(new Pair<>(readNumber(word), 0));
                            } else {
                                parsedTokens.add(new Pair<>(new StringValue(word), 0));
                            }
                    }

                    break;

                case TokenType.OPERATOR:
                    var op = token.getStringValue();
                    var opcode = new IntValue(operator.map(op));

                    if (operator.isUnaryOperator(op)) {
                        parsedTokens.add(new Pair<>(opcode, 1));
                    } else if (operator.isIncrementDecrementOperator(op)) {
                        parsedTokens.add(new Pair<>(opcode, 7));
                    } else if (operator.isBinaryOperator(op) && !op.equals("=")) {
                        parsedTokens.add(new Pair<>(opcode, 2));
                    } else if (operator.isComparisonOperator(op)) {
                        parsedTokens.add(new Pair<>(opcode, 3));
                    } else if (operator.isLogicalOperator(op)) {
                        parsedTokens.add(new Pair<>(opcode, 4));
                    } else if (operator.isPriorityOperator(op)){
                        parsedTokens.add(new Pair<>(opcode, 5));
                    } else if (op.equals("=")) {
                        parsedTokens.add(new Pair<>(opcode, 6));
                    } else {
                        parsedTokens.add(new Pair<>(null, -1));
                    }

                    break;

                case TokenType.PAREN_OPEN:
                    parsedTokens.add(new Pair<>(new IntValue(operator.PAREN_OPEN), 5));
                    break;

                case TokenType.PAREN_CLOSE:
                    parsedTokens.add(new Pair<>(new IntValue(operator.PAREN_CLOSE), 5));
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

        parsedTokens.add(new Pair<>(new IntValue(Operator.PAREN_CLOSE), 5));

        return parsedTokens;
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
