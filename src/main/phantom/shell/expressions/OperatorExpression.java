package phantom.shell.expressions;

import phantom.shell.parser.Token;
import phantom.support.lang.StringBuilder;
import phantom.support.util.List;

public class OperatorExpression extends Expression {

    public static final int TYPE = 5;

    private List<Token> tokens;

    public OperatorExpression(List<Token> tokens) {
        this.tokens = tokens;
    }

    public List<Token> getTokens() {
        return this.tokens;
    }

    @Override
    public int getType() {
        return TYPE;
    }
}
