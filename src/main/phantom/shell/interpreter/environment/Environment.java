package phantom.shell.interpreter.environment;

import phantom.shell.interpreter.executor.ExecutionFault;
import phantom.shell.interpreter.executor.ExecutionFaultException;
import phantom.shell.structures.Function;
import phantom.shell.values.StringValue;
import phantom.shell.values.Value;

import java.util.HashMap;

public class Environment {

    HashMap<Integer, Value> definedVariables;
    HashMap<Integer, Function> definedFunctions;
    Environment parent;

    public Environment() {
        parent = null;
        definedVariables = new HashMap<>();
        definedFunctions = new HashMap<>();
    }

    public Environment(Environment parent) {
        this.parent = parent;
        this.definedVariables = new HashMap<>();
        this.definedFunctions = new HashMap<>();
    }

    public void defineVariable(char[] name) {
        defineVariable(name, null);
    }

    public void defineVariable(char[] name, Value value) {
        defineVariable(new StringValue(name), value);
    }

    public void defineVariable(StringValue name) {
        defineVariable(name, null);
    }

    public void defineVariable(StringValue name, Value value) {
        //char[] str = name.getValue();
        if (!definedVariables.containsKey(name.hash())) {
            definedVariables.put(name.hash(), value);
        } else {
            throw new ExecutionFaultException(ExecutionFault.DEF_TWICE);
        }
    }

    public void setVariable(char[] name, Value value) {
        setVariable(new StringValue(name), value);
    }

    public void setVariable(Value name, Value value) {
        if (name instanceof StringValue) {
            StringValue str = ((StringValue) name);

            if (definedVariables.containsKey(str.hash())) {
                definedVariables.replace(str.hash(), value);
            } else {
                Environment current = parent;
                while (current != null) {
                    if (current.definedVariables.containsKey(str.hash())) {
                        current.definedVariables.replace(str.hash(), value);
                        break;
                    }
                    current = current.parent;
                }
            }
        }
    }

    public Value getVariable(char[] name){
        return getVariable(new StringValue(name));
    }

    public Value getVariable(Value name) {
        if (name instanceof StringValue) {
            StringValue str = ((StringValue) name);
            if (definedVariables.containsKey(str.hash())) {
                return definedVariables.get(str.hash());
            } else {
                Environment current = parent;
                while (current != null) {
                    if (current.definedVariables.containsKey(str.hash())) {
                        return current.definedVariables.get(str.hash());
                    }
                    current = current.parent;
                }
            }

            return null;
        }

        return name;
    }

    public void defineFunction(Function function){
        //definedFunctions.put();
    }

    public void callFunction(Function function){

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
