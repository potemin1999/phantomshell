package phantom.support.util;

import java.util.Iterator;

public abstract class List<T extends Object> implements Iterable<T>{

    public List() {
    }

    public final void add(Object obj) {
        addLast(obj);
    }

    public final T remove(){
        return removeLast();
    }

    public abstract void addFirst(Object obj);

    public abstract void addLast(Object obj);

    public abstract T removeFirst();

    public abstract T removeLast();

    public abstract T getFirst();

    public abstract T getLast();

    public abstract int size();

    public abstract boolean isEmpty();

    public abstract Iterator<T> iterator();

}
