package phantom.shell.values;

public class Value {
    public Value() {}

    public Object getValue() {return null;}
    public void setValue() {}

    public Value operatorIncrement() {return null;}
    public Value operatorDecrement() {return null;}

    public Value operatorAssigning(Value other) {return null;}
    public Value operatorAddition(Value other) {return null;}
    public Value operatorSubtraction(Value other) {return null;}
    public Value operatorMultiplication(Value other) {return null;}
    public Value operatorDivision(Value other) {return null;}

    public Value operatorBitwiseNot() {return null;}
    public Value operatorBitwiseAnd(Value other) {return null;}
    public Value operatorBitwiseOr(Value other) {return null;}
    public Value operatorBitwiseXor(Value other) {return null;}
    public Value operatorBitwiseShiftLeft(Value other) {return null;}
    public Value operatorBitwiseShiftRight(Value other) {return null;}

    public BoolValue operatorEqual(Value other) {return null;}
    public BoolValue operatorNotEqual(Value other) {return null;}
    public BoolValue operatorGreaterThan(Value other) {return null;}
    public BoolValue operatorLessThan(Value other) {return null;}
    public BoolValue operatorNotGreaterThan(Value other) {return null;}
    public BoolValue operatorNotLessThan(Value other) {return null;}
    public BoolValue operatorLogicalNot() {return null;}
    public BoolValue operatorLogicalAnd(Value other) {return null;}
    public BoolValue operatorLogicalOr(Value other) {return null;}
    public BoolValue operatorLogicalXor(Value other) {return null;}
    public BoolValue operatorLogicalImplication(Value other) {return null;}
}
