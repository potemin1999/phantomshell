package phantom.shell.structures;

public class Keyword {

    /**
     * @param chars
     * @return
     */
    public boolean isKeyword(char[] chars) {
        //         def
        return compare(chars, new char[]{'d', 'e', 'f'}) == 0 ||
                // if
                compare(chars, new char[]{'i', 'f'}) == 0 ||
                // elif
                compare(chars, new char[]{'e', 'l', 'i', 'f'}) == 0 ||
                // else
                compare(chars, new char[]{'e', 'l', 's', 'e'}) == 0 ||
                // return
                compare(chars, new char[]{'f', 'o', 'r'}) == 0 ||
                // while
                compare(chars, new char[]{'w', 'h', 'i', 'l', 'e'}) == 0 ||
                // do
                compare(chars, new char[]{'d', 'o'}) == 0 ||
                // switch
                compare(chars, new char[]{'s', 'w', 'i', 't', 'c', 'h'}) == 0 ||
                // case
                compare(chars, new char[]{'c', 'a', 's', 'e'}) == 0 ||
                // other
                compare(chars, new char[]{'o', 't', 'h', 'e', 'r'}) == 0 ||
                // return
                compare(chars, new char[]{'r', 'e', 't', 'u', 'r', 'n'}) == 0 ||
                // static
                compare(chars, new char[]{'s','t','a','t','i','c'}) == 0 ||
                // const
                compare(chars, new char[]{'c','o','n','s','t'}) == 0 ||
                // import
                compare(chars, new char[]{'i','m','p','o','r','t'}) == 0 ||
                // class
                compare(chars, new char[]{'c', 'l', 'a', 's', 's'}) == 0 ;
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
