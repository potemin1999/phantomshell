package phantom.support.util;

public abstract class List<T extends Object> {

    public List() {
    }

    public abstract void addFirst(Object obj);

    public abstract void addLast(Object obj);

    public abstract T removeFirst();

    public abstract T removeLast();

    public abstract T getFirst();

    public abstract T getLast();

    public abstract int size();
}
