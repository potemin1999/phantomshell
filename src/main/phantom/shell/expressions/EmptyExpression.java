package phantom.shell.expressions;

import phantom.shell.parser.Token;
import phantom.support.util.List;

public class EmptyExpression extends Expression {

    public static int TYPE = ExpressionTypes.EMPTY_EXPRESSION_TYPE;

    private List<Token> tokens;

    public EmptyExpression() {
        this.tokens = null;
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
        return "EmptyExp";
    }

}
