package phantom.support.util;

import phantom.support.lang.ArrayOutOfBoundsException;
import phantom.support.lang.StringBuilder;

import java.util.Iterator;

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
            buffer[i + 1] = buffer[i];
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
    public T remove(int index) {
        if (size == 0)
            return null;
        var ret = (T) buffer[index];
        for (int i = index; i < size; i++) {
            buffer[i] = buffer[i + 1];
        }
        buffer[size--] = null;
        return ret;
    }

    public T removeFirst() {
        return remove(0);
    }

    @SuppressWarnings("unchecked")
    public T get(int index) {
        if (index < 0 || index >= size)
            throw new ArrayOutOfBoundsException(index + " is out of bounds [0;" + size + ")");
        return (T) buffer[index];
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

    @Override
    public boolean isEmpty() {
        return size == 0;
    }

    @Override
    public Iterator<T> iterator() {
        return new Iterator<>() {

            int current = 0;

            @Override
            public boolean hasNext() {
                return current < size;
            }

            @Override
            @SuppressWarnings("unchecked")
            public T next() {
                return (T) buffer[current++];
            }
        };
    }

    @Override
    public String toString() {
        StringBuilder builder = new StringBuilder();
        builder.append('[');
        for (int i = 0; i < size; i++) {
            builder.append(buffer[i].toString());
            if (i != size - 1) builder.append(',');
        }
        builder.append(']');
        return String.valueOf(builder.build());
    }
}
