package phantom.shell.expressionParser;

import phantom.shell.environment.Environment;
import expressionEvaluator.ExpressionEvaluator;

import javafx.util.Pair;
import java.util.ArrayList;

public class Test {
    public static Environment remove(Environment env) {
        env = null;
        return env;
    }

    public static void print(ArrayList<Pair<Object, Integer>> tokens, int counter) {
        System.out.printf("expressionParser.Test #%d\n", counter);
        for (var i = 0; i < tokens.size(); ++i) {
            try {
                System.out.printf("%8s %d\n", tokens.get(i).getKey(), tokens.get(i).getValue());
            } catch (NullPointerException e){
                System.out.println(e);
            }

        }
        System.out.println();
    }

    public static void print2(Object result, int counter) {
        System.out.printf("expressionEvaluator.Test #%d\n", counter);
        System.out.println(result);
        System.out.println();
    }

    public static void main(String[] args) {
        ExpressionParser parser = new ExpressionParser();
        ExpressionEvaluator evaluator = new ExpressionEvaluator();

        Environment environment = new Environment();

        ArrayList<Pair<Object, Integer>> tokens;
        var counter = 0;

        // expressionParser.Test #1
        tokens = parser.parseInTokens(environment, "123 + 200a22");
        print(tokens, ++counter);

        // expressionParser.Test #2
        tokens = parser.parseInTokens(environment, "Luke I am your father");
        print(tokens, ++counter);

        // expressionParser.Test #3
        tokens = parser.parseInTokens(environment, "NO0O_O0O");
        print(tokens, ++counter);

        // expressionParser.Test #4
        tokens = parser.parseInTokens(environment, "lattice -> \\/\\/\\/\\/\\/\\/");
        print(tokens, ++counter);

        // expressionParser.Test #5
        tokens = parser.parseInTokens(environment, "cat /\\ dog == peace");
        print(tokens, ++counter);

        // expressionParser.Test #6
        tokens = parser.parseInTokens(environment, "This is xor \\\'/");
        print(tokens, ++counter);

        // expressionParser.Test #7
        tokens = parser.parseInTokens(environment, "-> +|");
        print(tokens, ++counter);

        // expressionParser.Test #8
        tokens = parser.parseInTokens(environment, "100 + 250 / 10 == 125");
        print(tokens, ++counter);

        // expressionParser.Test #9
        tokens = parser.parseInTokens(environment, "3 << 2 \\/ 1 == 13");
        print(tokens, ++counter);

        // expressionParser.Test #10
        tokens = parser.parseInTokens(environment, "3 << (2 \\/ 1) == 24");
        print(tokens, ++counter);

        // expressionParser.Test #11
        tokens = parser.parseInTokens(environment, "++x+++++x++");
        print(tokens, ++counter);


        Object result;
        counter = 0;

        // expressionEvaluator.Test #1
        print(parser.parseInTokens(environment, "100 + 250 / 10"), 1);
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "100 + 250 / 10"));
        print2(result, ++counter);

        // expressionEvaluator.Test #2
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "(245 - 250)"));
        print2(result, ++counter);

        // expressionEvaluator.Test #3
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "10/\\2"));
        print2(result, ++counter);

        // expressionEvaluator.Test #4
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "100.0 / 3 << 1"));
        print2(result, ++counter);

        // expressionEvaluator.Test #5
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "100.0 + 3 / 2"));
        print2(result, ++counter);

        // expressionEvaluator.Test #6
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "100.0 / 2 + 3"));
        print2(result, ++counter);

        // expressionEvaluator.Test #7
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "(100.0 + 3) / 2"));
        print2(result, ++counter);

        // expressionEvaluator.Test #8
        environment.defineVariable("abaca", 150);
        environment.defineVariable("baka", 3);
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "abaca / baka"));
        print2(result, ++counter);

        // expressionEvaluator.Test #9
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "100 > 10"));
        print2(result, ++counter);

        // expressionEvaluator.Test #10
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "2 * 2 != 4"));
        print2(result, ++counter);

        // expressionEvaluator.Test #11
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "abaca == 150 and baka == 3"));
        print2(result, ++counter);

        // expressionEvaluator.Test #12
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "abaca == 150.0 xor baka == 3"));
        print2(result, ++counter);

        // expressionEvaluator.Test #13
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "abaca == 150.1 and baka == 3 -> 60 >> 2 == 14"));
        print2(result, ++counter);

        // expressionEvaluator.Test #14
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "abaca == 150.1 -> baka == 3 and 60 >> 2 == 14"));
        print2(result, ++counter);

        // expressionEvaluator.Test #15
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "abaca == 150.0 and baka == 3 -> 60 >> 2 == 14"));
        print2(result, ++counter);

        // expressionEvaluator.Test #16
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "100 + (20 - 10 - (20 + 2000)) * 2"));
        print2(result, ++counter);

        // expressionEvaluator.Test #17
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "100 + 20 - 10 - 20 + 2000"));
        print2(result, ++counter);

        // expressionEvaluator.Test #18
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "10 and 12"));
        print2(result, ++counter);

        // expressionEvaluator.Test #19
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "1 and (1 xor 0 -> (30 != 30))"));
        print2(result, ++counter);

        // expressionEvaluator.Test #20
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "1 and (1 xor 0 and (30 != 30))"));
        print2(result, ++counter);

        // expressionEvaluator.Test #21
        result = evaluator.evaluateExpression(parser.parseInTokens(environment, "1 and (1 xor 0 and (30 != 30)) -> 0"));
        print2(result, ++counter);
    }
}
