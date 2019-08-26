### Functions

Function in Phantom Shell is a snippet of code which can accept fixed number of 
arguments and can return some value. It has it's own stack and local variables

Local variables are used to:
1. Save function arguments. It is guaranteed, that index of the argument in local variables memory
    will have the same relations > and < as do have index of argument in function definition
2. Save function local variables. They cannot be accessed by code in program root or by other functions
    (except passing them in functions arguments)
    
Stack for function is now allocated in heap (yeah, that is awfully confusing) and used as
usual stack in stack virtual machine: to execute any operations on its top

There are designed several opcodes, which are triggers the creation of function stack:
1.  Opcode call
    Is used to call global function. 
    Takes two bytes as function signature index in current constant pool.
    Before execution requires function arguments to be placed at current stack in ...[arg1,arg2,..., argN] order.
    Copies all arguments to local variables storage of called function.
    After function execution finished, places return value (or none)
    at the caller's stack (root statements' or other function's stack).
1.   Opcode invoke
    Used to call member function of object.
    Also takes two bytes as function signature index in caller constant pool 
    Always requires as first argument object reference, whose function should be called.
    