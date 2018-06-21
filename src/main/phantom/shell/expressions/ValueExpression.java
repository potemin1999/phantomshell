package phantom.shell.expressions;

import phantom.shell.environment.Environment;
import phantom.shell.values.Value;

public class ValueExpression extends Expression {

    private Value value;
    private OperatorExpression expression;

    public ValueExpression(OperatorExpression expression) {
        this.expression = expression;
    }

    public ValueExpression(Value value) {
        this.value = value;
    }

    public Value exec(Environment env) {
        compute();
        return value;
    }

    private void compute() {
        if (value != null) return;
    }
}
