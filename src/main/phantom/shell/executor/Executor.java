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
import phantom.support.util.Pair;

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
        //if (expression == null)
        // throw new NullPointerException("expression can not be null");
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
                var result = execute(expressionList.removeFirst());
                if (result instanceof ExecutionFault) {
                    return result;
                }
            }

            token = expressionList.removeLast().getTokens().getLast();
            TokenType.assertType(token, TokenType.BRACE_CLOSE);

            env = env.deleteEnvironment();
        } else if (expression instanceof PrintExpression) {
            var valueExpression = ((PrintExpression) expression).valueExpressionToPrint();
            var tokens = valueExpression.getTokens();

            var pair = handler.evaluate(env, tokens);
            var result = ((Pair) pair).getKey();
            env = (Environment) ((Pair) pair).getValue();
            printer.print(result);

            //Log.out.println("defined variable \"" + String.valueOf(name) + "\" with value " + result);
            return result;
        } else if (expression instanceof PrintlnExpression) {
            var valueExpression = ((PrintlnExpression) expression).valueExpressionToPrint();
            var tokens = valueExpression.getTokens();

            var pair = handler.evaluate(env, tokens);
            var result = ((Pair) pair).getKey();
            env = (Environment) ((Pair) pair).getValue();
            printer.println(result);

            //Log.out.println("defined variable \"" + String.valueOf(name) + "\" with value " + result);
            return result;
        }

        return 0;
    }
}
