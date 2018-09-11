package phantom.shell.expressions;

import phantom.shell.parser.Token;
import phantom.support.util.List;

public class EmptyExpression extends Expression {

    public static int TYPE = 0;

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
