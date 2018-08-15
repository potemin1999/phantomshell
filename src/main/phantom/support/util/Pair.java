package phantom.support.util;

/**
 * @author Ilya Potemin
 *
 * Simple pair for expression's evaluations
 * @see phantom.shell.calculator.ExpressionEvaluator
 * @see phantom.shell.calculator.ExpressionHandler
 * @see phantom.shell.calculator.ExpressionParser
 *
 * @param <K> key type
 * @param <V> value type
 */
public class Pair<K,V> {

    private K key;
    private V value;

    public Pair(K key, V value){
        this.key = key;
        this.value = value;
    }

    public K getKey(){
        return key;
    }

    public V getValue(){
        return value;
    }

    @Override
    public String toString() {
        return key+"="+value;
    }
}
