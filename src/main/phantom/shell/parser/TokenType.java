package phantom.shell.parser;

public class TokenType {

    /**
     *    name, type    e.t.c.
     */
    public static final int IDENTIFIER = 1;

    /**
     *    =
     *    ==
     *    >
     *    <
     *    >=
     *    <=
     *    !=
     */
    public static final int OPERATOR = 2;

    /**
     *    def
     *    return
     *    if
     *    elif
     *    else
     *    for
     *    while
     *    do
     *    switch
     *    case
     *    other
     *    class
     */
    public static final int KEYWORD = 3;

    /**
     *    ,
     */
    public static final int COMMA = 4;

    /**
     *    \n or semicolon
     */
    public static final int EOL = 5;

    /**
     *    {
     */
    public static final int BRACE_OPEN = 6;

    /**
     *    }
     */
    public static final int BRACE_CLOSE = 7;

    /**
     *    (
     */
    public static final int PAREN_OPEN = 8;

    /**
     *    )
     */
    public static final int PAREN_CLOSE = 9;

    /**
     *    .
     */
    public static final int DOT = 10;

}
