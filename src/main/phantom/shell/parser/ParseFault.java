package phantom.shell.parser;

public class ParseFault {

    private static final int BRACKET_OPEN_EXPECTED = 1000;
    private static final int BRACKET_CLOSE_EXPECTED = 1001;

    private int code;

    public ParseFault(int code) {
        this.code = code;
    }

    public String getDescription() {
        switch (code) {
            case BRACKET_OPEN_EXPECTED: return "'[' token was expected";
            case BRACKET_CLOSE_EXPECTED: return "']' token was expected";
            default: return "???";
        }
    }

    public String toString() {
        return "PARSE_FAULT " + code + " : " + getDescription();
    }

}
