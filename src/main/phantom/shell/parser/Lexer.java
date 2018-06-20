package phantom.shell.parser;

import phantom.shell.structures.Character;
import phantom.shell.structures.Keyword;
import phantom.support.lang.NullPointerException;
import phantom.support.lang.RuntimeException;
import phantom.support.lang.StringBuilder;
import phantom.support.log.Log;

public class Lexer {

    private boolean isEofReached;
    private Keyword keyword;
    private Scanner scanner;
    private StringBuilder tokenBuffer;
    private boolean[] letterMap;

    public Lexer(Scanner sourceScanner) {
        isEofReached = false;
        scanner = sourceScanner;
        tokenBuffer = new StringBuilder();
        keyword = new Keyword();
        letterMap = new boolean[255];
        fillLetterMap();
    }

    public boolean hasReachedEOF() {
        return isEofReached;
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
        if (code == 65535)
            return false;
        if (code > 255)
            throw new RuntimeException("Lexer unable to handle symbols with code 255+ : " + ((int) ch));
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
        var previousSymbol = ' ';
        var currentSymbolType = -1;
        var currentSymbol = ' ';
        var currentSymbolCode = -1;
        var isInsideComment = false;
        var isInsideText = false;
        var tokenType = 0;
        currentSymbol = scanner.peek(); //look up our symbol
        currentSymbolType = getSymbolType(currentSymbol);
        while (currentSymbol == ' ') {
            scanner.skip();
            currentSymbol = scanner.peek();
        }
        if (currentSymbol == '/'){
            scanner.skip();
            if (isNext('/')){
                do{
                    scanner.skip();
                }while (!(scanner.peek()=='\n' || scanner.peek()==';'));
            }
            if (isNext('*')){
                char next;
                char current;
                do{
                    scanner.skip();
                    current = scanner.next();
                    next = scanner.peek();
                }while (!(next=='/' && current=='*'));
                scanner.skip();
            }
            currentSymbol = scanner.peek();
            currentSymbolType = getSymbolType(currentSymbol);
        }
        if (isLetter(currentSymbol)) {
            do {
                scanner.skip();
                tokenBuffer.append(currentSymbol);
                currentSymbol = scanner.peek();
            } while (!isSymbol(currentSymbol));
            write(token, 1);
            return token;
        }
        if (isDigit(currentSymbol)) {
            do {
                scanner.skip();
                tokenBuffer.append(currentSymbol);
                currentSymbol = scanner.peek();
            } while (isDigit(currentSymbol) || currentSymbol == '.');
            write(token, 2);
            return token;
        } else { //if symbol
            if (currentSymbol == ';' || currentSymbol == '\n') {
                scanner.skip();
                token.setType(TokenType.EOL);
                if (currentSymbol == '\n')
                    token.setValue(new char[]{'\\', 'n'});
                else
                    token.setValue(new char[]{';'});
                return token;
            }
            if (currentSymbol == '=' || currentSymbol == '!') {
                commitSymbol(currentSymbol);
                tryNext('=');
                write(token,0);
                return token;
            }
            if (currentSymbol == '+') {
                commitSymbol('+');
                tryNext('+');
                write(token, 0);
            }
            if (currentSymbol == '-'){
                commitSymbol(currentSymbol);
                if (!tryNext('-')) // try --
                    tryNext('>');   // try ->
                write(token, 0);
            }
            if (currentSymbol == '>' || currentSymbol == '<'){
                commitSymbol(currentSymbol);
                tryNext(currentSymbol);
                tryNext('=');
                write(token, 0);
            }
            if (currentSymbol == '-'){

            }
        }

        return token;
    }

    private void commitSymbol(char ch){
        tokenBuffer.append(ch);
        scanner.skip();
    }

    private boolean isNext(char ch){
        return scanner.peek() == ch;
    }

    private boolean tryNext(char ch){
        var nextSymbol = scanner.peek();
        if (nextSymbol == ch) {
            scanner.skip();
            tokenBuffer.append(ch);
            return true;
        }else{
            return false;
        }
    }

    /**
     * writes token type and value
     *
     * @param dst         destination token
     * @param symbolsType symbols type
     *                    0,1,2 - symbols,letters,digits
     *                    -1 - mixed letters and digits
     */
    private void write(Token dst, int symbolsType) {
        dst.setValue(tokenBuffer.build());
        if (dst.getValue().length <= 0)
            throw new NullPointerException("token with zero value occurred");
        if (symbolsType == 0) {
            dst.setType(TokenType.OPERATOR);
        } else {
            if (keyword.isKeyword(dst.getValue())) {
                dst.setType(TokenType.KEYWORD);
            } else {
                dst.setType(TokenType.IDENTIFIER);
            }
        }
        tokenBuffer.clear();
    }

}
