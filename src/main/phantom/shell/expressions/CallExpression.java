package phantom.shell.expressions;

import phantom.shell.parser.Token;
import phantom.support.util.List;

public class CallExpression extends Expression {

    public static final int TYPE = ExpressionTypes.CALL_EXPRESSION_TYPE;
    @Override
    public List<Token> getTokens() {
        return null;
    }

    @Override
    public int getType() {
        return 0;
    }

}
