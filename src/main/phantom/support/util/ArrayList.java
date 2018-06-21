package phantom.support.util;

public class ArrayList<T extends Object> extends List<T> {

    private int capacity;
    private Object[] buffer;
    private int size;

    public ArrayList() {
        this(16);
    }

    @SuppressWarnings("unchecked")
    public ArrayList(int initialCapacity) {
        capacity = initialCapacity;
        buffer = new Object[capacity];
        size = 0;
    }

    public void addLast(Object obj) {
        ensureCapacity(size + 1);
        buffer[size] = obj;
        size++;
    }

    public void addFirst(Object obj) {
        ensureCapacity(size + 1);
        for (int i = size - 1; i > -1; i--) {
            buffer[size + 1] = buffer[size];
        }
        buffer[0] = obj;
        size++;
    }

    @SuppressWarnings("unchecked")
    public T removeLast() {
        if (size == 0)
            return null;
        return (T) buffer[--size];
    }

    @SuppressWarnings("unchecked")
    public T removeFirst() {
        if (size == 0)
            return null;
        var ret = (T) buffer[0];
        for (int i = 0; i < size; i++) {
            buffer[i] = buffer[i + 1];
        }
        buffer[size--] = null;
        return ret;
    }

    @SuppressWarnings("unchecked")
    public T getFirst() {
        if (size == 0)
            return null;
        return (T) buffer[0];
    }

    @SuppressWarnings("unchecked")
    public T getLast() {
        if (size == 0)
            return null;
        return (T) buffer[size - 1];
    }

    public int size() {
        return size;
    }

    private void ensureCapacity(int newSize) {
        var toResize = false;
        while (newSize >= capacity) {
            capacity *= 2;
            toResize = true;
        }
        if (toResize) {
            var bufferOld = buffer;
            var bufferNew = new Object[capacity];
            System.arraycopy(bufferNew, 0, bufferOld, 0, size);
            buffer = bufferNew;
        }
    }

}
