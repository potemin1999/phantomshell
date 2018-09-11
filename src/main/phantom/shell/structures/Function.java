package phantom.shell.structures;

import phantom.shell.values.Value;

public abstract class Function {

    private char[] signature;

    public Function(){}

    public abstract char[][] getTypes();
    public abstract char[] getName();

    //lazy
    public char[] getSignature(){
        if (signature==null){
            char[][] types = getTypes();
        }
        return signature;
    }

    public void execute(Value[] values){

    }

}
