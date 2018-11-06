package phantom.shell.expressions;

import phantom.shell.parser.Token;
import phantom.support.util.List;

public class DefExpression extends Expression {

    public static final int TYPE = ExpressionTypes.DEF_EXPRESSION_TYPE;

    private List<Token> tokens;

    public DefExpression(List<Token> tokens) {
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
        return "DefExp: " + tokens.toString();
    }

}
