package parser;

import phantom.shell.parser.*;
import phantom.support.log.Log;

public class ParserPerformanceTest {

    public static void main(String[] args){
        var scanner = new Scanner("src/test/scripts/lexer_performance_test.psh");
        var lexer = new Lexer(scanner);
        var parser = new Parser(lexer);
        var startTime = System.currentTimeMillis();
        var counter = 0;
        while (lexer.hasNotReachedEOF()) {
            parser.next();
        }
        var endTime = System.currentTimeMillis();
        var time = endTime - startTime;
        Log.out.println(counter + " token(s) in " + time + "ms : " + ((float) counter) / time + " t/ms");
    }


}