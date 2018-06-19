package phantom.support.io;

public class InputStream {

    private java.io.InputStream stream;

    public InputStream(){
        stream = null;
        System.out.print("Input stream creating without source, inheritance is expected");
    }

    public InputStream(java.io.InputStream wrap){
        if (wrap==null)
            throw new IOException("Input stream can not wrap nothing");
        wrap(wrap);
    }

    protected final void wrap(java.io.InputStream wrap){
        stream = wrap;
    }

    java.io.InputStream getStream(){
        return stream;
    }

    public int available(){
        try {
            return stream.available();
        }catch (java.io.IOException e){
            throw new IOException(e);
        }
    }

    public int read(){
        try {
            return stream.read();
        } catch (java.io.IOException e) {
            throw new IOException(e);
        }
    }

    public int read(byte[] buffer){
        try {
            return stream.read(buffer);
        } catch (java.io.IOException e) {
            throw new IOException(e);
        }
    }

    public int read(byte[] buffer,int offset,int length){
        try {
            return stream.read(buffer, offset, length);
        } catch (java.io.IOException e) {
            throw new IOException(e);
        }
    }

    public void close(){
        try {
            stream.close();
        } catch (java.io.IOException e) {
            throw new IOException(e);
        }
    }

}
