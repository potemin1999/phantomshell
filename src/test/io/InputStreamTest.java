package io;

import phantom.support.io.InputStream;

public class InputStreamTest {

    public static void main(String[] args){
        InputStream inputStream = new InputStream(System.in);
        int read = 0;
        do {
            read = inputStream.read();
            System.out.println("read char code "+read);
        }while (read!=32);

    }

}
