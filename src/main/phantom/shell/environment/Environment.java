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

    public void defineVariable(Object name, Object value) {
        if (!definedVariables.containsKey(name)) {
            definedVariables.put(name, value);
        } else {
            // Throw some exception that you can not define the same variable twice
        }
    }

    public void defineVariable(Object name) {
        if (!definedVariables.containsKey(name)) {
            definedVariables.put(name, null);
        } else {
            // Throw some exception that you can not define the same variable twice
        }
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

    public Environment deleteEnvironment(Environment env) {
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
