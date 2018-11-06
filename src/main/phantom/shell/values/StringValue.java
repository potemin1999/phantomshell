package phantom.shell.values;

public class StringValue extends Value {

    private char[] val;

    public StringValue(char[] newVal) {
        this.val = newVal;
    }

    public char[] getValue() {
        return this.val;
    }

    public void setValue(char[] newVal) {
        this.val = newVal;
    }

    @Override
    public Value operatorAddition(Value other) {
        char[] addArray = cast(other).val;
        char[] newArray = new char[val.length + addArray.length];
        copy(newArray,0,val,0,val.length);
        copy(newArray,val.length,addArray,0,addArray.length);
        return new StringValue(newArray);
    }

    @Override
    public BoolValue operatorEqual(Value other) {
        return new BoolValue(this.val.equals(cast(other).val));
    }

    private StringValue cast(Value toCast){
        if (toCast instanceof StringValue){
            return (StringValue) toCast;
        }else{
            return new StringValue(toCast.getValue().toString().toCharArray());
        }
    }

    private static void copy(char[] dst,int dstOffset, char[] src,int srcOffset,int length){
        var dstLength = dst.length;
        var srcLength = src.length;
        if (dstOffset<0 || srcOffset<0){
            throw new RuntimeException("invalid offset in copy function");
        }
        if (srcOffset+length>srcLength){
            throw new RuntimeException("invalid use of copy function ");
        }
        for (int i = 0;i<length;++i){
            dst[dstOffset+i] = src[srcOffset+i];
        }
    }

    public int hash(){
        return hash(val);
    }

    public static int hash(char[] val){
        int h = 0;
        int length = val.length >> 1;
        for (int i = 0; i < length; i++) {
            h = 31 * h + val[i];
        }
        return h;
    }

    @Override
    public String toString() {
        return "StringValue("+new String(val)+")";
    }
}
