package phantom.shell.parser;

import phantom.support.lang.StringBuilder;

public class Lexer {

    Scanner scanner;
    StringBuilder tokenBuffer;
    boolean[] letterMap;

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
        for (; i <123; i++) letterMap[i] = true;
    }

    public Token next() {
        var tokenFinished = false;
        var token = new Token();
        var previousSymbolIsLetter = false;
        var currentSymbol = ' ';
        var currentSymbolCode = -1;
        while (tokenFinished) {
            currentSymbol = scanner.next();
            currentSymbolCode = (int) currentSymbol;
        }
        return null;
    }

}
