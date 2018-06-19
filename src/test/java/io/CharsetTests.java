package io;

public class CharsetTests {

    public static void main(String[] args) {
        char a = 'а'; // 00000100 00110000
        dump(a);
        char z = 'я';
        dump(z);
        char A = 'А';
        dump(A);
        char Z = 'Я';
        dump(Z);
    }

    public static void dump(char a) {
        System.out.println("char " + (int) (a) + " in hex: " + Integer.toHexString(a));
    }


}
