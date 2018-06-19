package structures;

public class Character {
    public boolean isLetter(char c) {
        return java.lang.Character.isLetter(c);
    }

    public boolean isDigit(char c) {
        return java.lang.Character.isDigit(c);
    }

    public boolean isLetterOrDigit(char c) {
        return isLetter(c) || isDigit(c);
    }

    public boolean isSymbol(char c) {
        return !isLetterOrDigit(c) && c != ' ' && c != '\n' && c != '\b' && c != '\f' && c != '\r' && c != '\t' && c != '\0';
    }
}
