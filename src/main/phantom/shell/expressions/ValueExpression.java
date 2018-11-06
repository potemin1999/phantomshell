package phantom.shell.expressions;

import phantom.shell.parser.Token;
import phantom.support.util.List;

public class ValueExpression extends Expression {

    public static final int TYPE = ExpressionTypes.VALUE_EXPRESSION_TYPE;

    private List<Token> tokens;

    public ValueExpression(List<Token> tokens) {
        this.tokens = tokens;
    }

    public List<Token> getTokens() {
        return this.tokens;
    }

    @Override
    public int getType() {
        return TYPE;
    }

    @Override
    public String toString() {
        return "ValExp: "+tokens.toString();
    }
}
