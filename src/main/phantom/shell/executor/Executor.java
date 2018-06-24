package phantom.shell.executor;

import phantom.shell.environment.Environment;
import phantom.shell.calculator.ExpressionHandler;
import phantom.shell.expressions.*;
import phantom.shell.parser.Lexer;
import phantom.shell.parser.Parser;
import phantom.shell.parser.Token;
import phantom.shell.parser.TokenType;
import phantom.shell.printer.Printer;
import phantom.support.io.PrintStream;

public class Executor {

    private Lexer lexer;
    private Environment env;
    private ExpressionHandler handler;
    private Parser parser;
    private Printer printer;

    public Executor(Lexer lexer, PrintStream out) {
        this.lexer = lexer;
        this.env = new Environment();
        this.handler = new ExpressionHandler();
        this.parser = new Parser(lexer);
        this.printer = new Printer(out);
    }


    public Object execute(Expression expression) {
        if (expression instanceof DefExpression) {
            var tokens = expression.getTokens();
            var name = tokens.getFirst().getStringValue();

            env.defineVariable(name);

            var pair = handler.evaluate(env, tokens);
            var result = pair.getKey();
            env = pair.getValue();

            //Variable is already set inside the ExpressionEvaluator
            //env.setVariable(name, result);

            //Log.out.println("defined variable \"" + String.valueOf(name) + "\" with value " + result);
            return result;
        } else if (expression instanceof ValueExpression) {
            var tokens = expression.getTokens();

            var pair = handler.evaluate(env, tokens);
            var result = pair.getKey();
            env = pair.getValue();

            return result;
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
            var expressionList = ((BlockExpression) expression).getExpressionList();

            Token token = expressionList.removeFirst().getTokens().getFirst();
            TokenType.assertType(token, TokenType.BRACE_OPEN);

            env = new Environment(env);

            while (expressionList.size() > 1) {
                execute(expressionList.removeFirst());
            }

            token = expressionList.removeLast().getTokens().getLast();
            TokenType.assertType(token, TokenType.BRACE_CLOSE);

            env = env.deleteEnvironment();
        } else if (expression instanceof PrintExpression) {
            var valueExpression = ((PrintExpression) expression).valueExpressionToPrint();
            var tokens = valueExpression.getTokens();

            var pair = handler.evaluate(env, tokens);
            var result = pair.getKey();
            env = pair.getValue();

            printer.print(result);

            //Log.out.println("defined variable \"" + String.valueOf(name) + "\" with value " + result);
            return result;
        } else if (expression instanceof PrintlnExpression) {
            var valueExpression = ((PrintlnExpression) expression).valueExpressionToPrint();
            var tokens = valueExpression.getTokens();

            var pair = handler.evaluate(env, tokens);
            var result = pair.getKey();
            env = pair.getValue();

            printer.println(result);

            //Log.out.println("defined variable \"" + String.valueOf(name) + "\" with value " + result);
            return result;
        }

        return null;
    }
}
