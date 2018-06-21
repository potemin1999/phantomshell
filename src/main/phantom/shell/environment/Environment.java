package phantom.shell.environment;

import java.util.HashMap;

public class Environment {

    HashMap<Object, Object> definedVariables;
    Environment parent;

    public Environment() {
        parent = null;
        definedVariables = new HashMap<>();
    }

    public Environment(Environment par) {
        parent = par;
        definedVariables = new HashMap<>();
    }

    public void defineVariable(char[] name) {
        defineVariable(name, null);
    }

    public void defineVariable(char[] name, Object value) {
        defineVariable(String.valueOf(name), value);
    }

    public void defineVariable(Object name) {
        defineVariable(name, null);
    }

    public void defineVariable(Object name, Object value) {
        if (!definedVariables.containsKey(name)) {
            definedVariables.put(name, value);
        } else {
            // Throw some exception that you can not define the same variable twice
        }
    }

    public void setVariable(char[] name, Object value) {
        setVariable(String.valueOf(name), value);
    }

    public void setVariable(Object name, Object value) {
        if (definedVariables.containsKey(name)) {
            definedVariables.replace(name, value);
        } else {
            Environment current = parent;

            while (current != null) {
                if (current.definedVariables.containsKey(name)) {
                    definedVariables.replace(name, value);
                    break;
                }

                current = current.parent;
            }
        }
    }

    public Object getVariable(char[] name){
        return getVariable(String.valueOf(name));
    }

    public Object getVariable(Object name) {
        if (definedVariables.containsKey(name)) {
            return definedVariables.get(name);
        } else {
            Environment current = parent;

            while (current != null) {
                if (current.definedVariables.containsKey(name)) {
                    return current.definedVariables.get(name);
                }

                current = current.parent;
            }
        }

        return null;
    }

    public Environment deleteEnvironment() {
        Environment env = this;

        if (env.parent == null) {
            // Throw some exception that you can not delete the global environment

            return env;
        }

        Environment parent = env.parent;

        env.definedVariables = null;
        env.parent = null;

        return parent;
    }
}
