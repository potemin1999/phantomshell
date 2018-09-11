package phantom.shell.expressions;

import phantom.shell.parser.Token;
import phantom.support.util.List;

public class PrintExpression extends Expression {

    private ValueExpression valueExpressionToPrint;

    public PrintExpression(ValueExpression valueExpressionToPrint) {
        this.valueExpressionToPrint = valueExpressionToPrint;
    }

    public ValueExpression valueExpressionToPrint() {
        return  this.valueExpressionToPrint;
    }

    public List<Token> getTokens() {
        return null;
    }

    @Override
    public int getType() {
        return -1;
    }

}
