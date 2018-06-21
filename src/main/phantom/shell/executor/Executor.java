package phantom.shell.executor;

import phantom.shell.environment.Environment;
import phantom.shell.expressionHandler.ExpressionHandler;
import phantom.shell.expressions.*;
import phantom.shell.parser.Lexer;
import phantom.support.log.Log;

public class Executor {

    private Lexer lexer;
    private Environment env;
    private ExpressionHandler handler;


    public Executor(Lexer lexer) {
        this.lexer = lexer;
        this.env = new Environment();
        this.handler = new ExpressionHandler();
    }


    public Object execute(Expression expression) {
        if (expression instanceof DefExpression) {
            var tokens = expression.getTokens();
            var name = ((DefExpression) expression).getName();
            env.defineVariable(name);
            var result = handler.evaluate(env, tokens);
            env.setVariable(name, result);
            //Log.out.println("defined variable \"" + String.valueOf(name) + "\" with value " + result);
            return result;
        } else if (expression instanceof ValueExpression) {
            var tokens = expression.getTokens();

            return handler.evaluate(env, tokens);
        } else if (expression instanceof IfExpression) {
            var result = execute(((IfExpression) expression).getCondition());

            if (!result.equals(0)) {
                execute(((IfExpression) expression).getTrueBlock());
            } else {
                if (((IfExpression) expression).getNext() != null) {
                    execute(((IfExpression) expression).getNext());
                } else {
                    execute(((IfExpression) expression).getFalseBlock());
                }
            }
        } else if (expression instanceof BlockExpression) {
            var valueExpressionList = ((BlockExpression) expression).getExpressionList();

            while (valueExpressionList.size() > 0) {
                execute(valueExpressionList.removeFirst());
            }
        }

        return null;
    }
}
