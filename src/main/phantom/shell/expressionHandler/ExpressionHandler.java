package phantom.shell.expressionHandler;

import phantom.shell.environment.Environment;
import phantom.shell.expressionEvaluator.ExpressionEvaluator;
import phantom.shell.expressionParser.ExpressionParser;
import phantom.shell.parser.Token;
import phantom.shell.parser.TokenType;
import phantom.support.util.List;

public class ExpressionHandler {
    ExpressionParser parser;
    ExpressionEvaluator evaluator;

    public ExpressionHandler() {
        parser = new ExpressionParser();
        evaluator = new ExpressionEvaluator();
    }

    public Object evaluate(Environment environment, List<Token> tokens) {
        var parsedTokens = parser.parseTokens(tokens);

        //System.out.println(parsedTokens.get(0).getKey());

        var result = evaluator.evaluateExpression(environment, parsedTokens);

        //System.out.println("RESULT " + result);

        return result;
    }
}
