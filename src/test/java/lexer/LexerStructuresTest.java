package lexer;

import phantom.shell.parser.Lexer;
import phantom.shell.parser.Scanner;
import phantom.shell.parser.Token;
import phantom.shell.parser.TokenType;
import phantom.support.log.Log;

public class LexerStructuresTest {

    public static void main(String[] args) {
        var scanner = new Scanner("src/test/scripts/lexer_structures_test.psh");
        var lexer = new Lexer(scanner);
        var startTime = System.currentTimeMillis();
        var counter = 0;
        while (!lexer.hasReachedEOF()) {
            Token token = lexer.next();
            Log.out.print(token);
            if (token.getType() == TokenType.EOL) {
                Log.out.println();
            }
            counter++;
        }
        var endTime = System.currentTimeMillis();
        var time = endTime - startTime;
        System.out.println(counter + " token(s) in " + time + "ms : " + ((float) counter) / time + " t/ms");
    }

}
