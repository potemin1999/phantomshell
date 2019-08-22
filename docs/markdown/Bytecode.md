### Bytecode

#### Opcodes
They all can be found in file `src/vm/opcodes.h`

#### Functions
Function definition must be explicitly registered in VM by calling vm_register_function method.

Function call begins by unconditional jump over the function signature string to the 
OPCODE_CALL byte, which takes two bytes as constant pool index 