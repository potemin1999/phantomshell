package phantom.shell.expressions;

import phantom.shell.parser.Token;
import phantom.support.util.List;
import phantom.support.lang.StringBuilder;

public class IfExpression extends Expression {

    public static final int TYPE = 3;

    private ValueExpression condition;
    private IfExpression next;
    private BlockExpression trueBlock;
    private BlockExpression falseBlock;

    public IfExpression(ValueExpression condition, BlockExpression trueBlock) {
        this(condition, trueBlock, null);
    }

    public IfExpression(ValueExpression condition, BlockExpression trueBlock, IfExpression next) {
        this(condition, trueBlock, next, null);
    }

    public IfExpression(ValueExpression condition, BlockExpression trueBlock, IfExpression next, BlockExpression falseBlock) {
        this.condition = condition;
        this.trueBlock = trueBlock;
        this.next = next;
        this.falseBlock = falseBlock;
    }

    public ValueExpression getCondition() {
        return condition;
    }

    public BlockExpression getTrueBlock() {
        return this.trueBlock;
    }

    public BlockExpression getFalseBlock() {
        return falseBlock;
    }

    public IfExpression getNext() {
        return this.next;
    }

    public List<Token> getTokens() {
        return null;
    }

    @Override
    public int getType() {
        return TYPE;
    }

    @Override
    public String toString() {
        var builder = new StringBuilder();
        builder.append("IfExp: \n");
        builder.append("-condition: "+getCondition()+"\n");
        builder.append("-true block: "+getTrueBlock()+"\n");
        builder.append("-false block: "+getFalseBlock()+"\n");
        builder.append("-next IfExp: "+getNext()+"\n");
        return String.valueOf(builder.build());
    }
}
