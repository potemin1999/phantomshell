package phantom.shell.expressions;

import phantom.shell.parser.Token;
import phantom.support.lang.StringBuilder;
import phantom.support.util.List;

public class OperatorExpression extends Expression {
    private List<Token> tokens;

    public OperatorExpression(List<Token> tokens) {
        this.tokens = tokens;
    }

    public List<Token> getTokens() {
        return this.tokens;
    }
}