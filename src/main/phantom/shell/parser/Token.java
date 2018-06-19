package phantom.shell.parser;

public class Token {

    private int tokenType;
    private char[] tokenValue;

    public Token() {
        tokenType = TokenType.IDENTIFIER;
    }

    public void setType(int tokenType) {
        this.tokenType = tokenType;
    }

    public int getType() {
        return this.tokenType;
    }

    public void setValue(char[] tokenValue) {
        this.tokenValue = tokenValue;
    }

    public char[] getValue() {
        return this.tokenValue;
    }

}
