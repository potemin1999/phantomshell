package phantom.shell.values;

import phantom.shell.interpreter.calculator.ExpressionEvaluationException;
import phantom.shell.interpreter.calculator.ExpressionEvaluationFault;

public class FloatValue extends Value {
    private double val;
    private final double EPS = 1e-9;

    public FloatValue(Double newVal) {
        this.val = newVal;
    }

    public Double getValue() {
        return val;
    }

    public void setValue(Double x) {
        val = x;
    }

    public FloatValue operatorIncrement() {
        return new FloatValue(this.val + 1);
    }

    public FloatValue operatorDecrement() {
        return new FloatValue(this.val - 1);
    }

    public FloatValue operatorAssigning(Value other) {
        return new FloatValue(this.val = cast(other).val);
    }

    public FloatValue operatorAddition(Value other) {
        return new FloatValue(this.val + cast(other).val);
    }

    public FloatValue operatorSubtraction(Value other) {
        return new FloatValue(this.val - cast(other).val);
    }

    public FloatValue operatorMultiplication(Value other) {
        return new FloatValue(this.val * cast(other).val);
    }

    public FloatValue operatorDivision(Value other) {
        if (cast(other).getValue() == 0) {
            throw new ExpressionEvaluationException(ExpressionEvaluationFault.INTEGER_DIVISION_BY_ZERO);
        }

        return new FloatValue(this.val / cast(other).val);
    }

    public BoolValue operatorEqual(Value other) {
        return new BoolValue(Math.abs(this.val - cast(other).val) < EPS);
    }

    public BoolValue operatorNotEqual(Value other) {
        return new BoolValue(Math.abs(this.val - cast(other).val) > EPS);
    }

    public BoolValue operatorGreaterThan(Value other) {
        return new BoolValue(this.val - cast(other).val > EPS);
    }

    public BoolValue operatorLessThan(Value other) {
        return new BoolValue(this.val - cast(other).val < -EPS);
    }

    public BoolValue operatorNotGreaterThan(Value other) {
        return new BoolValue(this.val - cast(other).val < EPS);
    }

    public BoolValue operatorNotLessThan(Value other) {
        return new BoolValue(this.val - cast(other).val > -EPS);
    }

    public BoolValue operatorLogicalNot() {
        return new BoolValue(Math.abs(this.val) < EPS);
    }

    public BoolValue operatorLogicalAnd(Value other) {
        return new BoolValue(Math.abs(this.val) > EPS && Math.abs(cast(other).val) > EPS);
    }

    public BoolValue operatorLogicalOr(Value other) {
        return new BoolValue(Math.abs(this.val) > EPS || Math.abs(cast(other).val) > EPS);
    }

    public BoolValue operatorLogicalXor(Value other) {
        return new BoolValue((Math.abs(this.val) > EPS || Math.abs(cast(other).val) < EPS) && ((this.val) < EPS || Math.abs(cast(other).val) > EPS));
    }

    public BoolValue operatorLogicalImplication(Value other) {
        return new BoolValue(!(Math.abs(this.val) > EPS && Math.abs(cast(other).val) < EPS));
    }

    private FloatValue cast(Value toCast) {
        if (toCast instanceof FloatValue) {
            return (FloatValue) toCast;
        } else if (toCast instanceof IntValue) {
            var val = ((IntValue) toCast).getValue();
            Double floatVal = val.doubleValue();
            return new FloatValue(floatVal);
        } else if (toCast instanceof BoolValue) {
            return new FloatValue(((BoolValue) toCast).getValue() ? 1.0 : 0.0);
        } else {
            return null;
        }
    }
}
