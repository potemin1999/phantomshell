package phantom.shell.distributor;

import phantom.shell.environment.Environment;
import phantom.shell.expressionHandler.ExpressionHandler;
import phantom.shell.parser.Token;
import phantom.shell.parser.TokenType;

import java.util.LinkedList;

public class Distributor {
    LinkedList<Token> tokens;
    Environment environment;
    ExpressionHandler handler;

    public Distributor() {
        tokens = new LinkedList<>();
        environment = new Environment();
        handler = new ExpressionHandler();
    }

    public void receiveToken(Token token) {
        tokens.add(token);
        if (token.getType() == TokenType.EOL) {
            work();
        }
    }

    private Object work() {
        Token first = tokens.getFirst();
        tokens.removeFirst();

        Token current = first;
        LinkedList<Token> line = new LinkedList<>();

        while (current.getType() != TokenType.BRACE_OPEN && current.getType() != TokenType.EOL) {
            line.add(current);
            current = tokens.getFirst();
            tokens.removeFirst();
        }



        var moduleToCall = -1;
        String callback = "";

        switch (first.getType()) {
            case TokenType.KEYWORD:
                String val = first.toStringWithoutType();

                switch (val) {
                    case "def":

                        line.removeFirst();
                        current = line.getFirst();

                        if (current.getType() == TokenType.IDENTIFIER) {
                            Token id = current;
                            current = line.get(1);

                            if (current.getType() == TokenType.PAREN_OPEN) {
                                //TODO: deal with function definitions
                            } else if (current.getType() == TokenType.OPERATOR) {
                                environment.defineVariable(id.toStringWithoutType());
                                moduleToCall = 0;
                            }
                        } else {
                            moduleToCall = -1;
                            callback = "Unexpected token: expected identifier, got " + line.getFirst().toStringWithoutType();
                        }
                        break;

                }

                break;
        }

        Object result = new Token();

        switch (moduleToCall) {
            case 0:
                result = handler.evaluate(environment, line);
                break;
            default:
                System.out.println(callback);
                result = null;
        }
        return result;
    }
}
