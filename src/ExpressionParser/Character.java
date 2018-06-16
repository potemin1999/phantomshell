package ExpressionParser;

public class Character {
    boolean isLetter(char c) {
        return java.lang.Character.isLetter(c);
    }

    boolean isDigit(char c) {
        return java.lang.Character.isDigit(c);
    }

    boolean isLetterOrDigit(char c) {
        return isLetter(c) || isDigit(c);
    }

    boolean isSymbol(char c) {
        return !isLetterOrDigit(c) && c != ' ' && c != '\n' && c != '\b' && c != '\f' && c != '\r' && c != '\t' && c != '\0';

        //return c == '~' || c == '!' || c == '%' || c == '+' || c == '-' || c == '*' || c == '/' || c == '\\'
        //        || c == '\'' || c == '(' || c == ')' || c == '=' || c == '<' || c == '>';
    }
}
