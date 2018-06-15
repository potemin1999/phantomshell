package ExpressionParser;

import java.util.HashSet;

public class Operator {
    static HashSet<String> list = new HashSet<>();
    static {
        list.add("+");      //addition
        list.add("-");      //subtraction
        list.add("*");      //multiplication
        list.add("/");      //division

        list.add("++");     //increment
        list.add("--");     //decrement

        list.add("!");      //logical not
        list.add("->");     //logical implication

        list.add("~");      //bitwise not
        list.add("/\\");    //bitwise and
        list.add("\\/");    //bitwise or
        list.add("\\\'/");  //bitwise xor
        list.add("<<");     //bitwise arithmetical shift left
        list.add(">>");     //bitwise arithmetical shift right

        list.add("=");      //assigning
        list.add("==");     //equality
        list.add("!=");     //inequality
        list.add(">");      //greater than
        list.add(">=");     //not less than
        list.add("<");      //less than
        list.add("<=");     //not greater than

        list.add("(");      //opening parenthesis
        list.add(")");      //closing parenthesis
    }

    static boolean isOperator(String str) {
        return list.contains(str);
    }
}
