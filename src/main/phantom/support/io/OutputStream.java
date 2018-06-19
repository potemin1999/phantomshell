package phantom.support.io;

import phantom.support.lang.NullPointerException;

public class OutputStream {

    private java.io.OutputStream stream;

    public OutputStream() {
        stream = null;
    }

    public OutputStream(java.io.OutputStream wrap) {
        if (wrap == null)
            throw new NullPointerException("Unable to wrap null stream");
        wrap(wrap);
    }

    protected void wrap(java.io.OutputStream wrap) {
        stream = wrap;
    }

    java.io.OutputStream getStream() {
        return stream;
    }

    public void write(int b) {
        try {
            stream.write(b);
        } catch (java.io.IOException e) {
            throw new IOException(e);
        }
    }

    public void write(byte[] buffer) {
        try {
            stream.write(buffer);
        } catch (java.io.IOException e) {
            throw new IOException(e);
        }
    }

    public void write(byte[] buffer, int offset, int length) {
        try {
            stream.write(buffer, offset, length);
        } catch (java.io.IOException e) {
            throw new IOException(e);
        }
    }

    public void flush() {
        try {
            stream.flush();
        } catch (java.io.IOException e) {
            throw new IOException(e);
        }
    }

    public void close() {
        try {
            stream.close();
        } catch (java.io.IOException e) {
            throw new IOException(e);
        }
    }

}
