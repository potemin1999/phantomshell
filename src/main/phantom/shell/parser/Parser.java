package phantom.shell.parser;

import phantom.shell.expressions.*;
import phantom.support.util.ArrayList;
import phantom.support.util.List;

import static phantom.shell.parser.TokenType.*;

public class Parser {

    private Lexer lexer;

    public Parser(Lexer lexer) {
        this.lexer = lexer;
    }

    public Expression next() {
        var token = lexer.next();
        while (token.getType() == TokenType.EOL && lexer.hasNotReachedEOF()) {
            token = lexer.next();
        }
        return makeExpression(token);
    }

    public List<Expression> readToClosingBrace() {
        var list = new ArrayList<Expression>();
        readToClosingBrace(list);
        return list;
    }

    public void readToClosingBrace(List<Expression> list) {
        var braceClose = new ArrayList<Token>();

        do {
            var token = lexer.next();
            if (token.getType() == TokenType.BRACE_CLOSE) {
                braceClose.addLast(token);
                list.addLast(new ValueExpression(braceClose));
                break;
            }

            list.addLast(makeExpression(token));
        } while (lexer.hasNotReachedEOF());
    }

    /**
     * @param token first token, used to determine expression type
     * @return expression
     */
    public Expression makeExpression(Token token) {
        switch (token.getType()) {
            case KEYWORD: {
                return makeKeywordExpression(token);
            }
            case IDENTIFIER: {
                return makeValueExpression(token);
            }
            case BRACE_OPEN: {
                return makeValueExpression(token);
            }
            case BRACE_CLOSE: {
                return makeValueExpression(token);
            }
            case EOL: {
                return new EmptyExpression();
            }
            default:
                return null;
        }
    }

    private Expression makeKeywordExpression(Token token) {
        switch (token.getStringValue()) {
            case "def": {
                var identifier = lexer.next();
                assertType(identifier, TokenType.IDENTIFIER);
                var operator = lexer.next();
                assertType(operator, TokenType.OPERATOR);
                var expressionTokens = lexer.readToEOL();

                //reversed order to better performance in array list

                expressionTokens.add(operator);
                expressionTokens.addFirst(identifier);

                return new DefExpression(expressionTokens);
            }

            case "if": {
                //we are waiting for '[' symbol
                var openBracket = lexer.next();
                assertType(openBracket, TokenType.BRACKET_OPEN);
                //read all token till closing bracket
                var ifExpressionTokens = lexer.readToClosingBracket();
                //make a condition value
                ValueExpression valueExpression = new ValueExpression(ifExpressionTokens);
                // "{" expected
                var openBrace = lexer.next();
                assertType(openBrace, TokenType.BRACE_OPEN);

                ArrayList<Token> arrayList = new ArrayList<>();
                arrayList.addLast(openBrace);

                //TODO: read all expressions till closing brace
                var blockExpressions = new ArrayList<Expression>();
                blockExpressions.addLast(new ValueExpression(arrayList));
                readToClosingBrace(blockExpressions);
                var trueBlock = new BlockExpression(blockExpressions);

                token = lexer.next();
                //System.out.println(token.getStringValue());
                if (token.getType() == TokenType.KEYWORD) {
                    if (token.getStringValue().equals("elif")) {
                        token.setValue(new char[]{'i', 'f'});
                        var elifExpression = (IfExpression) makeKeywordExpression(token);
                        return new IfExpression(valueExpression, trueBlock, elifExpression);
                    } else if (token.getStringValue().equals("else")) {
                        //expecting opening brace
                        openBrace = lexer.next();
                        assertType(openBrace, TokenType.BRACE_OPEN);
                        //TODO: read all expressions till closing brace
                        var elseBlockExpressions = readToClosingBrace();
                        elseBlockExpressions.addFirst(new ValueExpression(arrayList));
                        var falseBlock = new BlockExpression(elseBlockExpressions);
                        return new IfExpression(valueExpression, trueBlock, null, falseBlock);
                    } else {
                        throw new InvalidTokenException("Unexpected token : " + token.getStringValue());
                    }
                } else if (token.getType() == TokenType.EOL) {
                    return new IfExpression(valueExpression, trueBlock);
                }

            }

            case "print": {
                var identifier = lexer.next();
                assertType(identifier, TokenType.BRACKET_OPEN);
                var expressionTokens = lexer.readToClosingBracket();

                expressionTokens.removeLast();

                ValueExpression expression = new ValueExpression(expressionTokens);

                return new PrintExpression(expression);
            }

            case "println": {
                var identifier = lexer.next();
                assertType(identifier, TokenType.BRACKET_OPEN);
                var expressionTokens = lexer.readToClosingBracket();

                expressionTokens.removeLast();

                ValueExpression expression = new ValueExpression(expressionTokens);

                return new PrintlnExpression(expression);
            }

            default:
                return null;
        }
    }

    private Expression makeValueExpression(Token token) {
        var identifier = token;
        var expressionTokens = lexer.readToEOL();
        expressionTokens.addFirst(identifier);

        return new ValueExpression(expressionTokens);
    }
}
