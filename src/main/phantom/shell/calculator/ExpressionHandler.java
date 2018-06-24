package phantom.shell.calculator;

import javafx.util.Pair;
import phantom.shell.environment.Environment;
import phantom.shell.parser.Token;
import phantom.support.util.List;

public class ExpressionHandler {
    ExpressionParser parser;
    ExpressionEvaluator evaluator;

    public ExpressionHandler() {
        parser = new ExpressionParser();
        evaluator = new ExpressionEvaluator();
    }

    public Pair<Object, Environment> evaluate(Environment environment, List<Token> tokens) {
        var parsedTokens = parser.parseTokens(tokens);

        var pair = evaluator.evaluateExpression(environment, parsedTokens);
        var result = pair.getKey();
        environment = pair.getValue();

        //System.out.println("RESULT " + result);

        return new Pair(result, environment);
    }
}
