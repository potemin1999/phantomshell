package phantom.shell.values;

public class StringValue extends Value {
    private String val;

    public StringValue(String newVal) {
        this.val = newVal;
    }

    public String getValue() {
        return this.val;
    }

    public void setValue(String newVal) {
        this.val = newVal;
    }
}
