package phantom.shell.expressions;

import phantom.shell.environment.Environment;
import phantom.support.lang.StringBuilder;
import phantom.shell.parser.Token;
import phantom.shell.values.Value;
import phantom.support.util.List;

public class MethodExpression extends Expression {

    private List<Token> tokens;

    public MethodExpression(List<Token> tokens) {
        this.tokens = tokens;
    }

    public List<Token> getTokens() {
        return this.tokens;
    }
}
