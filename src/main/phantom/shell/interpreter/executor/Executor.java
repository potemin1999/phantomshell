package phantom.shell.interpreter.executor;

import phantom.shell.interpreter.environment.Environment;
import phantom.shell.interpreter.calculator.ExpressionHandler;
import phantom.shell.expressions.*;
import phantom.shell.parser.Lexer;
import phantom.shell.parser.Parser;
import phantom.shell.parser.Token;
import phantom.shell.parser.TokenType;
import phantom.shell.interpreter.printer.Printer;
import phantom.shell.values.StringValue;
import phantom.support.io.PrintStream;

/**
 * @author Ilya Potemin
 * @author Bodgan Fedotov
 *
 * Executor takes and expressions from Parser and turns them into actions
 *
 * @see Parser
 * @see Expression
 */

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

    private Object assertExecution(int result) {
        if (result == 0) {
            return 0;
        } else {
            return new ExecutionFault(result);
        }
    }

    public Object executeUnsafe(Expression expression) {
        return execute(expression);
    }

    public Object executeSafe(Expression expression) {
        try {
            return execute(expression);
        } catch (ExecutionFaultException fault) {
            fault.printStackTrace();
            if (fault.getCode() == 0 || fault.getCode() == -1)
                throw fault;
            return fault.makeFault();
        } catch (Throwable throwable) {
            return new ExecutionFault(-1, throwable);
        }
    }

    /**
     * @param expression
     * @return Any object if execution was succeed
     * ExecutionFault object if not
     */
    protected Object execute(Expression expression) {
        if (expression == null)
         throw new NullPointerException("expression can not be null");
        switch (expression.getType()){
            case DefExpression.TYPE:{
                return executeDefExpression(((DefExpression) expression));
            }
            case ValueExpression.TYPE:{
                return executeValueExpression(((ValueExpression) expression));
            }
            case IfExpression.TYPE:{
                return executeIfExpression(((IfExpression) expression));
            }
            case BlockExpression.TYPE:{
                return executeBlockExpression(((BlockExpression) expression));
            }
        }
        /*if (expression instanceof PrintExpression) {
            var valueExpression = ((PrintExpression) expression).valueExpressionToPrint();
            var tokens = valueExpression.getTokens();

            var pair = handler.evaluate(env, tokens);
            var result = pair.getKey();
            env = (Environment) ((Pair) pair).getValue();
            printer.print(result);

            //Log.out.println("defined variable \"" + String.valueOf(name) + "\" with value " + result);
            return result;
        } else if (expression instanceof PrintlnExpression) {
            var valueExpression = ((PrintlnExpression) expression).valueExpressionToPrint();
            var tokens = valueExpression.getTokens();

            var pair = handler.evaluate(env, tokens);
            var result = pair.getKey();
            env = (Environment) ((Pair) pair).getValue();
            printer.println(result);

            //Log.out.println("defined variable \"" + String.valueOf(name) + "\" with value " + result);
            return result;
        }*/

        return 0;
    }

    protected Object executeDefExpression(DefExpression expression){
        var tokens = expression.getTokens();
        var name = tokens.getFirst().getValue();

        env.defineVariable(new StringValue(name));

        var result = handler.evaluate(env, tokens);

        return result;
    }

    protected Object executeValueExpression(ValueExpression expression){
        var tokens = expression.getTokens();

        var result = handler.evaluate(env, tokens);

        return result;
    }

    protected Object executeIfExpression(IfExpression expression){
        var result = execute(expression.getCondition());

        if (result.equals(true)) {
            execute(expression.getTrueBlock());
        } else {
            if (expression.getNext() != null) {
                execute(expression.getNext());
            } else {
                execute(expression.getFalseBlock());
            }
        }

        return result;
    }

    protected Object executeBlockExpression(BlockExpression expression){
        var expressionList = expression.getExpressionList();

        Token token = expressionList.removeFirst().getTokens().getFirst();
        TokenType.assertType(token, TokenType.BRACE_OPEN);

        env = new Environment(env);

        while (expressionList.size() > 1) {
            var result = execute(expressionList.removeFirst());
            if (result instanceof ExecutionFault) {
                return result;
            }
        }

        token = expressionList.removeLast().getTokens().getLast();
        TokenType.assertType(token, TokenType.BRACE_CLOSE);

        env = env.deleteEnvironment();
        return 0;
    }
}
