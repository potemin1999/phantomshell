package phantom.support.util;

public class LinkedList<T extends Object> extends List<T> {
    private int size = 0;
    private Node head = null;
    private Node tail = null;

    public LinkedList() {

    }

    public void addLast(Object obj) {
        if(size == 0) {
            head = new Node(obj);
            tail = head;
        }
        else {
            Node newNode = new Node(obj);
            newNode.prev = tail;
            tail.next = newNode;
            tail = newNode;
        }
        size++;
    }

    public void addFirst(Object obj) {
        if(size == 0) {
            head = new Node(obj);
            tail = head;
        }
        else {
            Node newNode = new Node(obj);
            newNode.next = head;
            head.prev = newNode;
            head = newNode;
        }
        size++;
    }

    @SuppressWarnings("unchecked")
    public T removeLast() {
        if (size == 0)
            return null;
        T retValue = (T) tail.getData();
        if (size == 1){
            head = null;
            tail = null;
        }
        else
            tail = tail.prev;
        size--;
        return retValue;
    }

    @SuppressWarnings("unchecked")
    public T removeFirst() {
        if (size == 0)
            return null;
        T retValue = (T) head.getData();
        if (size == 1){
            head = null;
            tail = null;
        }
        else
            head = head.next;
        size--;
        return retValue;
    }

    @SuppressWarnings("unchecked")
    public T getFirst() {
        if (size == 0)
            return null;
        return (T) head.getData();
    }

    @SuppressWarnings("unchecked")
    public T getLast() {
        if (size == 0)
            return null;
        return (T) tail.getData();
    }

    public int size() {
        return size;
    }

    class Node
    {
        //Declare class variables
        private Node next;
        private Node prev;
        private Object data;

        public Node(Object dat)
        {
            data = dat;
        }

        public Object getData()
        {
            return data;
        }
    }
}
