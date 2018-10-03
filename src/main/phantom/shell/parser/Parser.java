package phantom.shell.parser;

import phantom.shell.expressions.*;
import phantom.support.util.ArrayList;
import phantom.support.util.List;

import static phantom.shell.parser.TokenType.*;

/**
 * @author Ilya Potemin
 * <p>
 * Takes tokens from lexers and makes expressions from them
 * Expressions can be directly executed by Executor
 * @see phantom.shell.interpreter.executor.Executor
 * @see Lexer
 */
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
        int balance = 0;
        do {
            var token = lexer.next();
            if (token.getType() == TokenType.BRACE_OPEN) {
                ++balance;
                list.addLast(new ValueExpression(braceClose));
            }
            if (token.getType() == TokenType.BRACE_CLOSE) {
                if (balance == 0) {
                    braceClose.addLast(token);
                    list.addLast(new ValueExpression(braceClose));
                    break;
                } else {
                    --balance;
                }
            }
            list.addLast(makeExpression(token));
        } while (lexer.hasNotReachedEOF());
    }

    /**
     * @param token first token, used to determine expression type
     * @return expression
     */
    public Expression makeExpression(Token token) {
        return makeExpression(token,false,false);
    }

    private Expression makeExpression(Token token,boolean isInsideClass,boolean isInsideFunction){
        switch (token.getType()) {
            case KEYWORD: {
                return makeKeywordExpression(token,isInsideClass,isInsideFunction);
            }
            case OPERATOR: {
                return makeValueExpression(token);
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

    private Expression makeKeywordExpression(Token token,boolean isInsideClass,boolean isInsideFunction) {
        switch (token.getStringValue()) {
            case "def": {
                var identifier = lexer.next();
                assertType(identifier, TokenType.IDENTIFIER);
                var operator = lexer.next();
                //it may be function or variable
                if (operator.getType() != TokenType.OPERATOR) {
                    //function
                    var functionExpression = new FunctionExpression(identifier.getStringValue());
                    //var openBraceOrParen = lexer.next();
                    //if we have function with args
                    if (isInsideFunction){
                        throw new InvalidTokenException("function declaration inside function is not declared");
                    }
                    if (operator.getType()!=TokenType.PAREN_OPEN
                            && operator.getType()!=TokenType.BRACE_OPEN){
                        //invalid function declaration
                        throw new InvalidTokenException("invalid function declaration");
                    }
                    if (operator.getType() == TokenType.PAREN_OPEN) {
                        do {
                            var argType = lexer.next();
                            assertType(argType, TokenType.IDENTIFIER);
                            var argName = lexer.next();
                            assertType(argName, TokenType.IDENTIFIER);
                            //TODO: add args to function expression
                            functionExpression.addArgs(argType.getValue(),argName.getValue());
                        }while (lexer.next().getType()!=TokenType.PAREN_CLOSE);
                    }
                    var openingBrace = lexer.next();
                    assertType(openingBrace, TokenType.BRACE_OPEN);
                    List<Expression> bodyExpressionList = new ArrayList<>();
                    var bodyToken = lexer.next();
                    while (bodyToken.getType()==TokenType.EOL){
                        bodyToken = lexer.next();
                    }
                    var balance = 1;
                    while (!(bodyToken.getType() == TokenType.BRACE_CLOSE && balance == 0)) {
                        if (bodyToken.getType() == TokenType.BRACE_OPEN) {
                            ++balance;
                            continue;
                        }
                        if (bodyToken.getType() == TokenType.BRACE_CLOSE) {
                            --balance;
                            continue;
                        }
                        var bodyExpression = makeExpression(bodyToken);
                        bodyExpressionList.addLast(bodyExpression);
                        bodyToken = lexer.next();
                    }
                    functionExpression.setBody(bodyExpressionList);
                    int i = 0;
                    return functionExpression;
                } else {
                    //variable
                    var expressionTokens = lexer.readToEOL();
                    //reversed order to better performance in array list
                    expressionTokens.add(operator);
                    expressionTokens.addFirst(identifier);
                    return new DefExpression(expressionTokens);
                }
            }

            case "class":{
                return null;
            }
            case "if": {
                //we are waiting for '[' symbol
                var openBracket = lexer.next();
                assertType(openBracket, TokenType.BRACKET_OPEN);
                //read all token till closing bracket
                var ifExpressionTokens = lexer.readToEqualClosingBracket();
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
                        var elifExpression = (IfExpression) makeKeywordExpression(token,isInsideClass,isInsideFunction);
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
                if (lexer.peek().getType() == TokenType.BRACKET_OPEN) {
                    lexer.next();
                }
                /*var identifier = lexer.next();
                assertType(identifier, TokenType.BRACKET_OPEN);*/
                var expressionTokens = lexer.readTo(new int[]{TokenType.EOL, TokenType.BRACKET_CLOSE});

                expressionTokens.removeLast();

                ValueExpression expression = new ValueExpression(expressionTokens);

                return new PrintExpression(expression);
            }

            case "println": {
                if (lexer.peek().getType() == TokenType.BRACKET_OPEN) {
                    lexer.next();
                }
                /*var identifier = lexer.next();
                assertType(identifier, TokenType.BRACKET_OPEN);*/
                var expressionTokens = lexer.readTo(new int[]{TokenType.EOL, TokenType.BRACKET_CLOSE});

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
