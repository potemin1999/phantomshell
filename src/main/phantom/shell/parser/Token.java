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

    public String toString(){
        return "["+getStringType()+"] "+new String(
                getValue()!=null ? getValue() : new char[]{'N','U','L'})+" ";
    }

    private String getStringType(){
        switch (tokenType){
            case TokenType.EOL: return "EOL";
            case TokenType.IDENTIFIER: return "IDF";
            case TokenType.BRACE_CLOSE: return "BC";
            case TokenType.BRACE_OPEN: return "BO";
            case TokenType.COMMA: return "CMM";
            case TokenType.DOT: return "DOT";
            case TokenType.KEYWORD: return "KWD";
            case TokenType.OPERATOR: return "OPR";
            case TokenType.PAREN_CLOSE: return "PC";
            case TokenType.PAREN_OPEN: return "PO";
        }
        return "???";
    }

}
