package phantom.shell.expressions;

import phantom.shell.parser.Token;
import phantom.support.util.ArrayList;
import phantom.support.util.List;
import phantom.support.util.Pair;

public class FunctionExpression extends Expression {

    public static final int TYPE = ExpressionTypes.FUNCTION_EXPRESSION_TYPE;

    private List<Token> tokens;
    private List<Pair<char[],char[]>> args;
    private List<Expression> body;
    private String name;

    public FunctionExpression(String name){
        args = new ArrayList<>();
        this.name = name;
    }

    public FunctionExpression(List<Token> tokens) {
        this.tokens = tokens;
    }

    public List<Token> getTokens() {
        return this.tokens;
    }

    public void addArgs(char[] type,char[] name){
        args.addLast(new Pair<>(type,name));
    }
    public void setBody(List<Expression> bodyExpressions){
        body = bodyExpressions;
    }

    public List<Expression> getBody() {
        return body;
    }

    @Override
    public int getType() {
        return TYPE;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("FunctionExpression: ");
        sb.append(name).append("( ");
        for (var pair : args){
            sb.append("[").append(pair.getKey()).append(" ").append(pair.getValue()).append("] ");
        }
        sb.append(")\n").append("body: \n");
        for (var expression : body) {
            sb.append(expression.toString()).append("\n");
        }
        sb.append("end function");
        return sb.toString();
    }
}
