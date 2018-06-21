package phantom.shell.executor;

import phantom.shell.environment.Environment;
import phantom.shell.parser.InvalidTokenException;
import phantom.shell.parser.Lexer;
import phantom.shell.parser.Token;
import phantom.shell.parser.TokenType;

public class Executor {
    private Lexer lexer;
    private Environment env;


    public Executor(Lexer lexer, Environment env) {
        this.lexer = lexer;
        this.env = env;
    }


    public void execute() {
        while (lexer.hasNotReachedEOF()) {
            // read token
            var token = lexer.next();

            switch (token.getType()) {
                case TokenType.KEYWORD:
                    // if, for, def
                    break;
                case TokenType.IDENTIFIER:
                    // function call, var assignment
                    break;
            }

            // determine what is this

            // execute some code
        }
    }
}
