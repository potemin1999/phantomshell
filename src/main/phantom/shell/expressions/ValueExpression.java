package phantom.shell.expressions;

import phantom.shell.environment.Environment;
import phantom.shell.parser.Token;
import phantom.shell.values.Value;
import phantom.support.lang.StringBuilder;
import phantom.support.util.List;

public class ValueExpression extends Expression {

    public static final int TYPE = 2;

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
