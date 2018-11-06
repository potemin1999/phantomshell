package phantom.shell.expressions;

import phantom.shell.parser.Token;
import phantom.support.lang.StringBuilder;
import phantom.support.util.List;

public class BlockExpression extends Expression {

    public static final int TYPE = ExpressionTypes.BLOCK_EXPRESSION_TYPE;

    private List<Expression> expressionList;

    public BlockExpression(List<Expression> expressionList) {
        this.expressionList = expressionList;
    }

    public List<Expression> getExpressionList() {
        return this.expressionList;
    }

    public List<Token> getTokens() {
        return null;
    }

    @Override
    public int getType() {
        return TYPE;
    }

    public String toString() {
        StringBuilder builder = new StringBuilder();
        builder.append("[\n");
        for (var expression : expressionList) {
            builder.append(expression.toString()+"\n");
        }
        builder.append("]\n");
        return String.valueOf(builder.build());
    }

}
