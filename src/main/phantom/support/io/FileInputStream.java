package phantom.support.io;


public class FileInputStream extends InputStream{

    public FileInputStream(File file){
        super();
        try {
            var stream = new java.io.FileInputStream(file.getFile());
            wrap(stream);
        } catch (java.io.FileNotFoundException e) {
            throw new FileNotFoundException(e);
        }
    }

    protected FileInputStream(java.io.InputStream wrap) {
        super(wrap);
    }

}
