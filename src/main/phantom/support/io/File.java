package phantom.support.io;

public class File {

    private java.io.File file;

    public File(String fileName) {
        file = new java.io.File(fileName);
    }

    /**
     * package-private
     *
     * @return
     */
    java.io.File getFile() {
        return file;
    }
}
