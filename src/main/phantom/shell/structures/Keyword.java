package phantom.shell.structures;

public class Keyword {

    public boolean isKeyword(char[] chars) {
        return compare(chars, new char[]{'d', 'e', 'f'}) == 0 ||
                compare(chars, new char[]{'i','f'}) == 0 ||
                compare(chars, new char[]{'e','l','i','f'}) == 0 ||
                compare(chars, new char[]{'e','l','s','e'}) == 0;
    }

    private int compare(char[] chars1, char[] chars2) {
        if (chars1.length != chars2.length)
            return chars1.length > chars2.length ? -1 : 1;
        for (int i = 0; i < chars1.length; i++) {
            if (chars1[i] != chars2[i])
                return chars1[i] < chars2[i] ? -1 : 1;
        }
        return 0;
    }

}
