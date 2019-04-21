### Functions

function without arguments and with void return value
 
    func functionName1(){
        a = 23
    }

function without arguments and with int result value 

    func functionName2() int{
        return 123
    }
    
function with arguments of unknown type and int result value
 
    // implementations for such functions will differ by arguments' types
    func functionName3(x,y) int{
        a = (int) x
        b = (int) y
        return a+b
    }
   
function with arguments of known type and int result value

    func functionName4(x int,y int) int{
        return x+y
    }