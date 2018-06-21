package phantom.support.io;

public class BufferedInputStream extends InputStream {

    private InputStream stream;
    private byte[] buffer;
    private int capacity;
    private int currentPointer;
    private int endPointer;
    private int available;

    public BufferedInputStream(InputStream wrap) {
        stream = wrap;
        capacity = 128;
        buffer = new byte[capacity];
    }

    private void cutPointers() {
        currentPointer = currentPointer % capacity;
        endPointer = endPointer % capacity;
        if (endPointer < currentPointer)
            endPointer += capacity;
    }

    private void bufferize() {
        if (stream.available() <= 0)
            return;
        cutPointers();
        var length = capacity - endPointer;
        var size = stream.read(buffer, endPointer, length);
        endPointer += size;
        if (size == length) {
            cutPointers();
            var length2 = currentPointer - endPointer - 1;
            var size2 = stream.read(buffer, endPointer, length2);
            endPointer += size2;
        }
    }

    @Override
    public int available() {
        return available;
    }

    @Override
    public int read() {
        bufferize();
        if (available <= 0)
            return -1;
        return buffer[currentPointer++];
    }

    @Override
    public int read(byte[] buf) {
        return 0;
        //bufferize();
        //System.arraycopy(buffer,currentPointer,buf,0,);
    }

    @Override
    public int read(byte[] buffer, int offset, int length) {
        return 0;
        //bufferize();
        //var size = capacity > length
        //return super.read(buffer, offset, length);
    }

    @Override
    public void close() {
        super.close();
    }
}
