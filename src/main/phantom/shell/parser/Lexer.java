package phantom.shell.parser;

import phantom.shell.structures.Character;
import phantom.support.lang.RuntimeException;
import phantom.support.lang.StringBuilder;

public class Lexer {

    private Scanner scanner;
    private StringBuilder tokenBuffer;
    private boolean[] letterMap;

    public Lexer(Scanner sourceScanner) {
        scanner = sourceScanner;
        tokenBuffer = new StringBuilder();
        letterMap = new boolean[255];
    }

    private void fillLetterMap() {
        var i = 0;
        for (; i < 48; i++) letterMap[i] = false;
        for (; i < 58; i++) letterMap[i] = true;
        for (; i < 65; i++) letterMap[i] = false;
        for (; i < 91; i++) letterMap[i] = true;
        for (; i < 97; i++) letterMap[i] = false;
        for (; i < 123; i++) letterMap[i] = true;
        for (; i < 255; i++) letterMap[i] = false;
    }

    private boolean isLetter(char ch) {
        int code = (int) ch;
        if (code > 255)
            throw new RuntimeException("Lexer unable to handle symbols with code > 255 yet");
        return letterMap[code];
    }

    private boolean isDigit(char ch) {
        int code = (int) ch;
        return code > 47 & code < 58;

    }

    private boolean isSymbol(char ch) {
        return !(isDigit(ch) || isLetter(ch));
    }

    /**
     * @param ch
     * @return 0 - for symbol
     * 1 - for letter
     * 2 - for digit
     */
    private int getSymbolType(char ch) {
        if (isLetter(ch))
            return 1;
        if (isDigit(ch))
            return 2;
        return 0;
    }

    public Token next() {
        var tokenFinished = false;
        var token = new Token();
        var previousSymbolType = -1;
        var currentSymbolType = -1;
        var currentSymbol = ' ';
        var currentSymbolCode = -1;
        var isInsideComment = false;
        var isInsideText = false;
        while (!tokenFinished) {
            currentSymbol = scanner.next();
            currentSymbolCode = (int) currentSymbol;
            currentSymbolType = getSymbolType(currentSymbol);
            if (previousSymbolType == -1) { //start new sequence of symbol
                if (currentSymbolType == 1) { //variable or keyword
                    tokenBuffer.append(currentSymbol);
                } else if (currentSymbolType == 2) { //digits

                } else { // other symbols
                    if (currentSymbol == ' ') {
                        continue; //just a space
                    }
                    if (currentSymbol == ';' ||
                            currentSymbol == '\n') {
                        token.setType(TokenType.EOL);
                        token.setValue(new char[]{currentSymbol});
                        tokenFinished = true;
                    }
                    if (currentSymbol == '=' ||
                            currentSymbol == '>' ||
                            currentSymbol == '<' ||
                            currentSymbol == "!") {

                    }
                }
            } else {

            }
            previousSymbolType = currentSymbolType;
        }
        return null;
    }

}
