package phantom.shell.environment;

import phantom.shell.executor.ExecutionFault;
import phantom.shell.executor.ExecutionFaultException;
import phantom.shell.values.StringValue;
import phantom.shell.values.Value;

import java.util.HashMap;

public class Environment {

    HashMap<String, Value> definedVariables;
    Environment parent;

    public Environment() {
        parent = null;
        definedVariables = new HashMap<>();
    }

    public Environment(Environment parent) {
        //System.out.println("New env!");
        this.parent = parent;
        this.definedVariables = new HashMap<>();
    }

    public void defineVariable(char[] name) {
        defineVariable(name, null);
    }

    public void defineVariable(char[] name, Value value) {
        defineVariable(new StringValue(String.valueOf(name)), value);
    }

    public void defineVariable(StringValue name) {
        defineVariable(name, null);
    }

    public void defineVariable(StringValue name, Value value) {
        String str = name.getValue();
        if (!definedVariables.containsKey(str)) {
            definedVariables.put(str, value);
        } else {
            throw new ExecutionFaultException(ExecutionFault.DEF_TWICE);
        }
    }

    public void setVariable(char[] name, Value value) {
        setVariable(new StringValue(String.valueOf(name)), value);
    }

    public void setVariable(Value name, Value value) {
        if (name instanceof StringValue) {
            String str = ((StringValue) name).getValue();
            //System.out.println("Trying to find " + str);

            if (definedVariables.containsKey(str)) {
                //System.out.println("Found " + str);

                definedVariables.replace(str, value);
                //System.out.println("ASDASD " + definedVariables.get(str));
            } else {

                Environment current = parent;

                while (current != null) {
                    if (current.definedVariables.containsKey(str)) {
                        //System.out.println("Not Found " + str);
                        current.definedVariables.replace(str, value);
                        //System.out.println(definedVariables.get(str));
                        //System.out.println(value);
                        break;
                    }

                    current = current.parent;
                }
            }
        }
    }

    public Value getVariable(char[] name){
        return getVariable(new StringValue(String.valueOf(name)));
    }

    public Value getVariable(Value name) {
        if (name instanceof StringValue) {
            String str = ((StringValue) name).getValue();
            //System.out.println("Trying to find " + name);

            if (definedVariables.containsKey(str)) {
                //System.out.println("Found " + name);
                //System.out.println("VALUE = " + definedVariables.get(name));
                return definedVariables.get(str);
            } else {
                //System.out.println("Not Found " + name);
                Environment current = parent;

                while (current != null) {

                    if (current.definedVariables.containsKey(str)) {
                        //System.out.println("Found " + name);
                        //System.out.println("VALUE = " + current.definedVariables.get(name));
                        return current.definedVariables.get(str);
                    }

                    current = current.parent;
                }
            }

            return null;
        }

        return name;
    }

    public Environment deleteEnvironment() {
        //System.out.println("Env deleted!");
        Environment env = this;

        if (env.parent == null) {
            throw new EnvironmentException("Deletion of the global environment is prohibited!");
        }

        Environment parent = env.parent;

        env.definedVariables = null;
        env.parent = null;

        return parent;
    }
}
