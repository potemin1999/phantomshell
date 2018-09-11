package phantom.shell.calculator;

import phantom.shell.environment.Environment;
import phantom.shell.parser.Token;
import phantom.shell.values.Value;
import phantom.support.util.List;
import phantom.support.util.Pair;

public class ExpressionHandler {

    private ExpressionParser parser;
    private ExpressionEvaluator evaluator;

    public ExpressionHandler() {
        parser = new ExpressionParser();
        evaluator = new ExpressionEvaluator();
    }

    public Value evaluate(Environment environment, List<Token> tokens) {
        var parsedTokens = parser.parseTokens(tokens);

        var result = evaluator.evaluateExpression(environment, parsedTokens);
        //var result = pair.getKey();
        //environment = pair.getValue();

        //System.out.println("RESULT " + result);
        //System.out.println("RESULT VALUE " + result.getValue());

        return result;
    }
}
