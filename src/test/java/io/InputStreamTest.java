package io;

import phantom.support.io.InputStream;

public class InputStreamTest {

    public static void main(String[] args) {
        InputStream inputStream = new InputStream(System.in);
        for (byte b : "а".getBytes()) {
            System.out.println(Integer.toHexString(b));
        }

        int read = 0;
        do {
            read = inputStream.read();
//            System.out.println("read char code "+read+"bits:"+Integer.toBinaryString(read));
            System.out.println("read char '" + (char) read + "' with code " + read + "; hex: " + Integer.toHexString(read));
        } while (read != 32);

    }

}
