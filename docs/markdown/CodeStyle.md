### Code Style

C++ is fast as old C, but also have large amount of OOP features, like Java.
C is using snake_script in most cases, Java and other similar languages prefers CamelCase
So which style of code should we choose?
 
....

Oh, really, why should we choose? We can use both styles!

````
 int64         //type name for some primitive type
 simplestring  //type name for class, which should be like primitive
 OStream       //name for regular class
 some_func     //this is function in class, namespace or somewhere else
 OFlags        //also name for structures and unions
 any_var       //variable names are written in snake script
 phlib         //namespace names are always one short word
 ANY_CONSTANT  //element of enum, global constant or #defined value
 all_files.h   //header and source file names in snake script only
````
