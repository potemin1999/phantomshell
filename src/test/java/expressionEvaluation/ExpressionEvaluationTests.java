package expressionEvaluation;

import phantom.shell.environment.Environment;
import phantom.shell.expressionHandler.ExpressionHandler;
import phantom.shell.expressionParser.ExpressionParser;

import javafx.util.Pair;

import java.util.ArrayList;

public class ExpressionEvaluationTests {
    public static void print(ArrayList<Pair<Object, Integer>> tokens, int counter) {
        System.out.printf("expressionParser Test #%d\n", counter);
        for (var i = 0; i < tokens.size(); ++i) {
            try {
                System.out.printf("%8s %d\n", tokens.get(i).getKey(), tokens.get(i).getValue());
            } catch (NullPointerException e) {
                System.out.println(e);
            }

        }
        System.out.println();
    }

    public static void check(Object result, Object correctResult, int counter) {
        System.out.printf("expressionEvaluator Test #%d\n", counter);
        System.out.print(result + " ");

        if (result.equals(correctResult)) {
            System.out.println("pass");
        } else {
            System.out.println("fail");
        }

        System.out.println();
    }

    public static void main(String[] args) {
        /*
        ExpressionParser parser = new ExpressionParser();
        ExpressionHandler handler = new ExpressionHandler();

        Environment environment = new Environment();

        ArrayList<Pair<Object, Integer>> tokens;
        var counter = 0;

        // expressionParser.ExpressionEvaluationTests #1
        tokens = parser.parseInTokens("123 + 200a22");
        print(tokens, ++counter);

        // expressionParser.ExpressionEvaluationTests #2
        tokens = parser.parseInTokens("Luke I am your father");
        print(tokens, ++counter);

        // expressionParser.ExpressionEvaluationTests #3
        tokens = parser.parseInTokens("NO0O_O0O");
        print(tokens, ++counter);

        // expressionParser.ExpressionEvaluationTests #4
        tokens = parser.parseInTokens("lattice -> \\/\\/\\/\\/\\/\\/");
        print(tokens, ++counter);

        // expressionParser.ExpressionEvaluationTests #5
        tokens = parser.parseInTokens("cat /\\ dog == peace");
        print(tokens, ++counter);

        // expressionParser.ExpressionEvaluationTests #6
        tokens = parser.parseInTokens("This is xor \\\'/");
        print(tokens, ++counter);

        // expressionParser.ExpressionEvaluationTests #7
        tokens = parser.parseInTokens("-> +|");
        print(tokens, ++counter);

        // expressionParser.ExpressionEvaluationTests #8
        tokens = parser.parseInTokens("100 + 250 / 10 == 125");
        print(tokens, ++counter);

        // expressionParser.ExpressionEvaluationTests #9
        tokens = parser.parseInTokens("3 << 2 \\/ 1 == 13");
        print(tokens, ++counter);

        // expressionParser.ExpressionEvaluationTests #10
        tokens = parser.parseInTokens("3 << (2 \\/ 1) == 24");
        print(tokens, ++counter);

        // expressionParser.ExpressionEvaluationTests #11
        tokens = parser.parseInTokens("++x+++++x++");
        print(tokens, ++counter);


        Object result;
        counter = 0;

        // expressionEvaluator.ExpressionEvaluationTests #1
        print(parser.parseInTokens("100 + 250 / 10"), 1);
        result = handler.evaluate(environment, "100 + 250 / 10");
        check(result, 125, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #2
        result = handler.evaluate(environment, "(245 - 250)");
        check(result, -5, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #3
        result = handler.evaluate(environment, "10/\\2");
        check(result, 2, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #4
        result = handler.evaluate(environment, "100.0 / 3 << 1");
        check(result, 16.666666666666668, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #5
        result = handler.evaluate(environment, "100.0 + 3 / 2");
        check(result, 101.0, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #6
        result = handler.evaluate(environment, "100.0 / 2 + 3");
        check(result, 53.0, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #7
        result = handler.evaluate(environment, "(100.0 + 3) / 2");
        check(result, 51.5, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #8
        environment.defineVariable("abaca", 150);
        environment.defineVariable("baka", 3);
        result = handler.evaluate(environment, "abaca / baka");
        check(result, 50, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #9
        result = handler.evaluate(environment, "100 > 10");
        check(result, 1, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #10
        result = handler.evaluate(environment, "2 * 2 != 4");
        check(result, 0, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #11
        result = handler.evaluate(environment, "abaca == 150 and baka == 3");
        check(result, 1, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #12
        result = handler.evaluate(environment, "abaca == 150.0 xor baka == 3");
        check(result, 0.0, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #13
        result = handler.evaluate(environment, "abaca == 150.1 and baka == 3 -> 60 >> 2 == 14");
        check(result, 1.0, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #14
        result = handler.evaluate(environment, "abaca == 150.1 -> baka == 3 and 60 >> 2 == 14");
        check(result, 1.0, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #15
        result = handler.evaluate(environment, "abaca == 150.0 and baka == 3 -> 60 >> 2 == 14");
        check(result, 0.0, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #16
        result = handler.evaluate(environment, "100 + (20 - 10 - (20 + 2000)) * 2");
        check(result, -3920, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #17
        result = handler.evaluate(environment, "100 + 20 - 10 - 20 + 2000");
        check(result, 2090, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #18
        result = handler.evaluate(environment, "10 and 12");
        check(result, 1, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #19
        result = handler.evaluate(environment, "1 and (1 xor 0 -> (30 != 30))");
        check(result, 0, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #20
        result = handler.evaluate(environment, "1 and (1 xor 0 and (30 != 30))");
        check(result, 1, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #21
        result = handler.evaluate(environment, "1 and (1 xor 0 and (30 != 30)) -> 0");
        check(result, 0, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #22
        result = handler.evaluate(environment, "abaca = baka = 100");
        check(result, 100, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #23
        result = handler.evaluate(environment, "abaca == 100 and baka == 100");
        check(result, 1, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #24
        result = handler.evaluate(environment, "--baka == 99");
        check(result, 1, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #25
        result = handler.evaluate(environment, "++baka == 100");
        check(result, 1, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #26
        result = handler.evaluate(environment, "++baka");
        check(result, 101, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #27
        result = handler.evaluate(environment, "baka++");
        check(result, 101, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #28
        result = handler.evaluate(environment, "baka == 102");
        check(result, 1, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #29
        result = handler.evaluate(environment, "not 1");
        check(result, 0, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #30
        result = handler.evaluate(environment, "!true");
        check(result, 0, ++counter);

        // expressionEvaluator.ExpressionEvaluationTests #30
        result = handler.evaluate(environment, "!(false) xor false");
        check(result, 1, ++counter);

        //int a = 1, b = 2;
        //System.out.println(++((a)));
        */
    }
}
