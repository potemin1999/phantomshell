package phantom.shell.values;

public class BoolValue extends Value {
    private boolean val;

    public BoolValue(Boolean newVal) {
        this.val = newVal;
    }

    public Boolean getValue() {
        return val;
    }

    public void setValue(Boolean x) {
        val = x;
    }

    public BoolValue operatorEqual(BoolValue other) {
        return new BoolValue(this.val == other.val);
    }

    public BoolValue operatorNotEqual(BoolValue other) {
        return new BoolValue(this.val != other.val);
    }

    public BoolValue operatorLogicalNot() {
        return new BoolValue(!this.val);
    }

    public BoolValue operatorLogicalAnd(BoolValue other) {
        return new BoolValue(this.val && other.val);
    }

    public BoolValue operatorLogicalOr(BoolValue other) {
        return new BoolValue(this.val || other.val);
    }

    public BoolValue operatorLogicalXor(BoolValue other) {
        return new BoolValue((this.val || !other.val) && (!this.val || other.val));
    }

    public BoolValue operatorLogicalImplication(BoolValue other) {
        return new BoolValue(this.val && !other.val ? false : true);
    }
}
