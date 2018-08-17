package phantom.shell.values;

import phantom.shell.calculator.ExpressionEvaluationException;
import phantom.shell.calculator.ExpressionEvaluationFault;

public class IntValue extends Value {
    private int val;

    public IntValue(Integer newVal) {
        this.val = newVal;
    }

    public Integer getValue() {
        return val;
    }

    public void setValue(Integer x) {
        val = x;
    }

    public IntValue operatorIncrement() {
        return new IntValue(this.val + 1);
    }

    public IntValue operatorDecrement() {
        return new IntValue(this.val - 1);
    }

    public IntValue operatorAssigning(Value other) {
        return new IntValue(this.val = cast(other).val);
    }

    public IntValue operatorAddition(Value other) {
        return new IntValue(this.val + cast(other).val);
    }

    public IntValue operatorSubtraction(Value other) {
        return new IntValue(this.val - cast(other).val);
    }

    public IntValue operatorMultiplication(Value other) {
        return new IntValue(this.val * cast(other).val);
    }

    public IntValue operatorDivision(Value other) {
        if (cast(other).getValue() == 0) {
            throw new ExpressionEvaluationException(ExpressionEvaluationFault.INTEGER_DIVISION_BY_ZERO);
        }

        return new IntValue(this.val / cast(other).val);
    }

    public IntValue operatorBitwiseNot() {
        return new IntValue(~this.val);
    }

    public IntValue operatorBitwiseAnd(Value other) {
        return new IntValue(this.val & cast(other).val);
    }

    public IntValue operatorBitwiseOr(Value other) {
        return new IntValue(this.val | cast(other).val);
    }

    public IntValue operatorBitwiseXor(Value other) {
        return new IntValue(this.val ^ cast(other).val);
    }

    public IntValue operatorBitwiseShiftLeft(Value other) {
        return new IntValue(this.val << cast(other).val);
    }

    public IntValue operatorBitwiseShiftRight(Value other) {
        return new IntValue(this.val >> cast(other).val);
    }

    public BoolValue operatorEqual(Value other) {
        return new BoolValue(this.val == cast(other).val);
    }

    public BoolValue operatorNotEqual(Value other) {
        return new BoolValue(this.val != cast(other).val);
    }

    public BoolValue operatorGreaterThan(Value other) {
        return new BoolValue(this.val > cast(other).val);
    }

    public BoolValue operatorLessThan(Value other) {
        return new BoolValue(this.val < cast(other).val);
    }

    public BoolValue operatorNotGreaterThan(Value other) {
        return new BoolValue(this.val <= cast(other).val);
    }

    public BoolValue operatorNotLessThan(Value other) {
        return new BoolValue(this.val >= cast(other).val);
    }

    public BoolValue operatorLogicalNot() {
        return new BoolValue(this.val == 0 ? true : false);
    }

    public BoolValue operatorLogicalAnd(Value other) {
        return new BoolValue(this.val != 0 && cast(other).val != 0);
    }

    public BoolValue operatorLogicalOr(Value other) {
        return new BoolValue(this.val != 0 || cast(other).val != 0);
    }

    public BoolValue operatorLogicalXor(Value other) {
        return new BoolValue((this.val != 0 || cast(other).val == 0) && (this.val == 0 || cast(other).val != 0));
    }

    public BoolValue operatorLogicalImplication(Value other) {
        return new BoolValue(this.val != 0 && cast(other).val == 0 ? false : true);
    }

    private IntValue cast(Value toCast) {
        if (toCast instanceof IntValue) {
            return (IntValue) toCast;
        } else if (toCast instanceof FloatValue) {
            var val = ((FloatValue) toCast).getValue();
            Integer intVal = val.intValue();
            return new IntValue(intVal);
        } else if (toCast instanceof BoolValue) {
            return new IntValue(((BoolValue) toCast).getValue() ? 1 : 0);
        } else {
            return null;
        }
    }

    @Override
    public String toString() {
        return "IntValue("+val+")";
    }
}
