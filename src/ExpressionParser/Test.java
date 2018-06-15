package ExpressionParser;

import java.util.ArrayList;

public class Test {
    public static void print(ArrayList<String> tokens, int counter) {
        System.out.printf("ExpressionParser.Test #%d\n", counter);
        for (var i = 0; i < tokens.size(); ++i) {
            System.out.println(tokens.get(i));
        }
        System.out.print('\n');
    }

    public static void main(String[] args) {
        ExpressionParser parser = new ExpressionParser();
        ArrayList<String> tokens;
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
    }
}
