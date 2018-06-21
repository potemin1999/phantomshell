package phantom.shell.expressionHandler;

import phantom.shell.environment.Environment;
import phantom.shell.expressionEvaluator.ExpressionEvaluator;
import phantom.shell.expressionParser.ExpressionParser;

import java.util.ArrayList;

public class ExpressionHandler {
    ExpressionParser parser;
    ExpressionEvaluator evaluator;

    public ExpressionHandler() {
        parser = new ExpressionParser();
        evaluator = new ExpressionEvaluator();
    }

    public Object evaluate(Environment environment, String expression) {
        var tokens = parser.parseInTokens(expression);
        return evaluator.evaluateExpression(environment, tokens);
    }
}
