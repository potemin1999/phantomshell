package phantom.shell.expressions;

import phantom.shell.environment.Environment;

public class DefExpression extends Expression {

    private char[] name;
    private ValueExpression value;

    public DefExpression(char[] name, ValueExpression value) {
        this.name = name;
        this.value = value;
    }

    public Object exec(Environment env) {
        return null;
    }

}
