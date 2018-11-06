package phantom.shell.expressions;

import phantom.shell.parser.Token;
import phantom.support.lang.StringBuilder;
import phantom.support.util.List;

public class WhileExpression extends Expression {

    public static final int TYPE = ExpressionTypes.WHILE_EXPRESSION_TYPE;

    private List<Token> tokens;

    public WhileExpression(List<Token> tokens) {
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
