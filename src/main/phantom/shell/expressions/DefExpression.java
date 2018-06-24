package phantom.shell.expressions;

import phantom.shell.parser.Token;
import phantom.support.util.List;

public class DefExpression extends Expression {

    private List<Token> tokens;

    public DefExpression(List<Token> tokens) {
        this.tokens = tokens;
    }

    public List<Token> getTokens() {
        return this.tokens;
    }
}
