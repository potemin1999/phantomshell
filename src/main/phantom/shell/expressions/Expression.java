package phantom.shell.expressions;

import phantom.shell.parser.Token;
import phantom.support.util.List;

public abstract class Expression {

    public Expression() {};

    public abstract List<Token> getTokens();

}
