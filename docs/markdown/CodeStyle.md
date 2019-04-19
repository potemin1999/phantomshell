### Code Style

C++ is fast as old C, but also have large amount of OOP features, like Java.
C is using snake_script in most cases, Java and other similar languages prefers сamelCase
So which style of code should we choose?
 
....

Oh, really, why should we choose? We can use both styles!

UPD: snake script sucks, removed it.
Now are are using **PascalCase** + **camelCase**

````
 someVar       //name for variable
 Unt64         //type name for "primitive" type
 OStream       //name for regular class, structure or union
 someFunc      //class member function
 SomeFunc      //namespace function
 phlib         //namespace names are always one short word
 ANY_CONSTANT  //element of enum, global constant or #defined value
 Types.h       //header and source file names
````

Code Style for CLion
[link](https://github.com/potemin1999/phantomshell/tree/master/docs/xml/Project.xml)