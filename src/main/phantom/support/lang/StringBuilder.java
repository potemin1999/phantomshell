package phantom.support.lang;

public class StringBuilder {

    private int capacity;
    private char[] buffer;
    private int currentSize;

    public StringBuilder() {
        this(16);
    }

    public StringBuilder(int initialCapacity) {
        buffer = new char[initialCapacity];
        capacity = 16;
        currentSize = 0;
    }

    public void append(String str) {
        int addSize = str.length();
        ensureCapacity(currentSize + addSize);
        copy(buffer,str.toCharArray(),currentSize,0,addSize);
        currentSize += addSize;
    }

    public void append(char symbol) {
        ensureCapacity(currentSize + 1);
        buffer[currentSize] = symbol;
        currentSize += 1;
    }

    public void append(char[] chars) {
        int addSize = chars.length;
        ensureCapacity(currentSize + addSize);
        copy(buffer,chars,currentSize,0,addSize);
        currentSize += addSize;
    }

    public void removeLast() {
        currentSize += -1;
    }

    public char[] build() {
        char[] res = new char[currentSize];
        copy(res, buffer, 0, 0, currentSize);
        return res;
    }

    public void clear() {
        buffer = new char[capacity];
        currentSize = 0;
    }

    private void ensureCapacity(int newSize) {
        var toResize = false;
        while (capacity <= newSize) {
            capacity *= 2;
            toResize = true;
        }
        if (toResize) {
            var bufferOld = buffer;
            var bufferNew = new char[capacity];
            copy(bufferNew, bufferOld, 0, 0, currentSize);
            buffer = bufferNew;
        }
    }

    private void copy(char[] dst, char[] src, int dstOffset, int srcOffset, int length) {
        if (length > src.length)
            throw new ArithmeticException("length required to copy is bigger than array size");
        if (dst.length < dstOffset + length)
            throw new ArithmeticException("not enough capacity of dst array");
        System.arraycopy(src, srcOffset, dst, dstOffset, length);
    }

}
