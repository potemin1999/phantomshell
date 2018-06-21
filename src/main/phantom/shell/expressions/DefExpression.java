package phantom.shell.expressions;

import phantom.shell.parser.Token;
import phantom.support.util.List;

public class DefExpression extends Expression {

    private char[] name;
    private List<Token> tokens;

    public DefExpression(char[] name,List<Token> tokens) {
        this.name = name;
        this.tokens = tokens;
    }

    public char[] getName(){
        return name;
    }
    public List<Token> getTokens() {
        return this.tokens;
    }

    @Override
    public String toString() {
        return "DefExp:" + String.valueOf(name) + " = " + tokens.toString();
    }
}
