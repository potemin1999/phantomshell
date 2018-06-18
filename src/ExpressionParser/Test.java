package ExpressionParser;

import ExpressionEvaluator.ExpressionEvaluator;

import javafx.util.Pair;
import java.util.ArrayList;

public class Test {
    public static void print(ArrayList<Pair<Object, Integer>> tokens, int counter) {
        System.out.printf("ExpressionParser.Test #%d\n", counter);
        for (var i = 0; i < tokens.size(); ++i) {
            System.out.printf("%8s %d\n", tokens.get(i).getKey(), tokens.get(i).getValue());
        }
        System.out.println();
    }

    public static void print2(Object result, int counter) {
        System.out.printf("ExpressionEvaluator.Test #%d\n", counter);
        System.out.println(result);
        System.out.println();
    }

    public static void main(String[] args) {
        ExpressionParser parser = new ExpressionParser();
        ExpressionEvaluator evaluator = new ExpressionEvaluator();

        ArrayList<Pair<Object, Integer>> tokens;
        var counter = 0;

        //ExpressionParser.Test #1
        tokens = parser.parseInTokens("123 + 200a22");
        print(tokens, ++counter);

        //ExpressionParser.Test #2
        tokens = parser.parseInTokens("Luke I am your father");
        print(tokens, ++counter);

        //ExpressionParser.Test #3
        tokens = parser.parseInTokens("NO0O_O0O");
        print(tokens, ++counter);

        //ExpressionParser.Test #4
        tokens = parser.parseInTokens("lattice -> \\/\\/\\/\\/\\/\\/");
        print(tokens, ++counter);

        //ExpressionParser.Test #5
        tokens = parser.parseInTokens("cat /\\ dog == peace");
        print(tokens, ++counter);

        //ExpressionParser.Test #6
        tokens = parser.parseInTokens("This is xor \\\'/");
        print(tokens, ++counter);

        //ExpressionParser.Test #7
        tokens = parser.parseInTokens("-> +|");
        print(tokens, ++counter);

        //ExpressionParser.Test #8
        tokens = parser.parseInTokens("100 + 250 / 10 == 125");
        print(tokens, ++counter);

        //ExpressionParser.Test #9
        tokens = parser.parseInTokens("3 << 2 \\/ 1 == 13");
        print(tokens, ++counter);

        //ExpressionParser.Test #10
        tokens = parser.parseInTokens("3 << (2 \\/ 1) == 24");
        print(tokens, ++counter);

        //ExpressionParser.Test #11
        tokens = parser.parseInTokens("++x+++++x++");
        print(tokens, ++counter);


        Object result;
        counter = 0;

        //ExpressionEvaluator.Test #1
        result = evaluator.evaluateExpression(parser.parseInTokens("100 + 250 / 10"));
        print2(result, ++counter);

        //ExpressionEvaluator.Test #2
        result = evaluator.evaluateExpression(parser.parseInTokens("(245 - 250)"));
        print2(result, ++counter);

        //ExpressionEvaluator.Test #3
        result = evaluator.evaluateExpression(parser.parseInTokens("10/\\2"));
        print2(result, ++counter);

        //ExpressionEvaluator.Test #4
        result = evaluator.evaluateExpression(parser.parseInTokens("100.0 / 3 << 1"));
        print2(result, ++counter);

        //ExpressionEvaluator.Test #5
        result = evaluator.evaluateExpression(parser.parseInTokens("100.0 + 3 / 2"));
        print2(result, ++counter);

        //ExpressionEvaluator.Test #5
        result = evaluator.evaluateExpression(parser.parseInTokens("(100.0 + 3) / 2"));
        print2(result, ++counter);
    }
}
