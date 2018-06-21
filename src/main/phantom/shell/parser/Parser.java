package phantom.shell.parser;

import phantom.shell.expressions.DefExpression;
import phantom.shell.expressions.Expression;
import phantom.shell.expressions.ValueExpression;

import static phantom.shell.parser.TokenType.KEYWORD;

public class Parser {

    private Lexer lexer;

    public Parser(Lexer lexer) {
        this.lexer = lexer;
    }

    private void assertType(Token token, int tokenType) {
        assertType(token, tokenType, Token.typeToString(tokenType) + " expected");
    }

    private void assertType(Token token, int tokenType, String errorMsg) {
        if (token.getType() != tokenType)
            throw new InvalidTokenException(errorMsg);
    }

    public Expression next() {
        var token = lexer.next();
        while (token.getType() == TokenType.EOL) {
            token = lexer.next();
        }
        return makeExpression(token);
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
        }
        return null;
    }

    private Expression makeKeywordExpression(Token token) {
        switch (token.getStringValue()) {
            case "def": {
                var tokenName = lexer.next();
                assertType(tokenName, TokenType.IDENTIFIER);
                var tokenAssert = lexer.next();
                assertType(tokenAssert, TokenType.OPERATOR);
                var expressionTokens = lexer.readToEOL();
                ValueExpression valueExp = null;
                if (expressionTokens.size() == 1) {
                    //TODO: value or const
                } else {
                    //TODO: operator
                }
                return new DefExpression(tokenName.getValue(), valueExp);
            }
            default:
                return null;
        }
    }
}
