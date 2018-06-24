package phantom.shell.environment;

import java.util.HashMap;

public class Environment {

    HashMap<Object, Object> definedVariables;
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
            throw new EnvironmentException("Can not define two variables with the same name in the same scope!");
        }
    }

    public void setVariable(char[] name, Object value) {
        setVariable(String.valueOf(name), value);
    }

    public void setVariable(Object name, Object value) {
        //System.out.println("Trying to find " + name);

        if (definedVariables.containsKey(name)) {
            //System.out.println("Found " + name);

            definedVariables.replace(name, value);
            //System.out.println("ASDASD " + definedVariables.get(name));
        } else {

            Environment current = parent;

            while (current != null) {
                if (current.definedVariables.containsKey(name)) {
                    //System.out.println("Not Found " + name);
                    current.definedVariables.replace(name, value);
                    //System.out.println(definedVariables.get(name));
                    //System.out.println(value);
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
        if (name instanceof String) {
            //System.out.println("Trying to find " + name);

            if (definedVariables.containsKey(name)) {
                //System.out.println("Found " + name);
                //System.out.println("VALUE = " + definedVariables.get(name));
                return definedVariables.get(name);
            } else {
                //System.out.println("Not Found " + name);
                Environment current = parent;

                while (current != null) {

                    if (current.definedVariables.containsKey(name)) {
                        //System.out.println("Found " + name);
                        //System.out.println("VALUE = " + current.definedVariables.get(name));
                        return current.definedVariables.get(name);
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
