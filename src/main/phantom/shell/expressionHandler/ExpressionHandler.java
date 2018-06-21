package phantom.shell.expressionHandler;

import phantom.shell.environment.Environment;
import phantom.shell.expressionEvaluator.ExpressionEvaluator;
import phantom.shell.expressionParser.ExpressionParser;
import phantom.shell.parser.Token;
import phantom.shell.parser.TokenType;

import java.util.ArrayList;
import java.util.LinkedList;

public class ExpressionHandler {
    ExpressionParser parser;
    ExpressionEvaluator evaluator;

    public ExpressionHandler() {
        parser = new ExpressionParser();
        evaluator = new ExpressionEvaluator();
    }

    public Object evaluate(Environment environment, LinkedList<Token> tokens) {
        var parsedTokens = parser.parseTokens(tokens);
        var result = evaluator.evaluateExpression(environment, parsedTokens);

        System.out.println("RESULT " + result);

        return result;
    }
}
