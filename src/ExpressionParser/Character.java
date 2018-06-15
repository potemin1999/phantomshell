package ExpressionParser;

public class Character {
    static boolean isLetter(char c) {
        return java.lang.Character.isLetter(c);
    }

    static boolean isDigit(char c) {
        return java.lang.Character.isDigit(c);
    }

    static boolean isLetterOrDigit(char c) {
        return isLetter(c) || isDigit(c);
    }

    static boolean isSymbol(char c) {
        return !isLetterOrDigit(c) && c != ' ' && c != '\n' && c != '\b' && c != '\f' && c != '\r' && c != '\t' && c != '\0';

        //return c == '~' || c == '!' || c == '%' || c == '+' || c == '-' || c == '*' || c == '/' || c == '\\'
        //        || c == '\'' || c == '(' || c == ')' || c == '=' || c == '<' || c == '>';
    }
}
