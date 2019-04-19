### Operators

 * Increment and decrement
    * `++`  - postincrement
    * `--`  - postdecrement
 * Assignment operator
    * `=`   - simple assignment
 * Arithmetic operators
    * `+`   - addition
    * `-`   - subtraction
    * `*`   - multiplication
    * `/`   - division
 * Comparison operators
    * `==`  - equals
    * `!=`  - not equals
    * `>`   - greater than
    * `<`   - less than
    * `<=`  - not greater than
    * `>=`  - not less than
 * Logical operators
    * `!` and `not` - negation
    * `and` - logical and
    * `or`  - logical or
    * `xor` - logical xor
    * `->`  - implication
 * Bitwise operators
    * `~`   - bitwise negation
    * `<<`  - bitwise shift to the left
    * `>>`  - bitwise shift to the right
    * `/\`  - bitwise and _(proposal: to replace by `and`)_
    * `\/`  - bitwise or _(proposal: to replace by `or`)_
    * `\'/` - bitwise xor _(proposal: to replace by `xor`)_
 * Access operators 
    * `a[b]`   - subscription
    * `a[b:c]` - array slice
    * `a.b`    - member access
 * Other operators
    * `a(b1,b2,..,bN)` - function call
    * `(type) a`       - type casting
    * `a ? b : c`      - ternary conditional operator
    
---
##### Precedence

Operators with a higher precedence are at the top of list
 
| Precedence | Operator       | Associativity   | Description                              
| ---------- | -------------- | --------------- |------------------------------------- 
| 1          |  `a++`,`a--`   | Left-to-Right   | 
|            |  `a(...)`      |                 | 
|            |  `a[b]`        |                 | 
|            |  `a[b:c]`      |                 | 
|            |  `a.b`         |                 | Member access
| 2          |  `!`,`~`       | Right-to-Left   | Logical and bitwise not 
|            |  `(type) a`    |                 | Explicit type cast
| 3          |  `a*b`, `a/b`  | Left-to-Right   | 
| 4          |  `a+b`, `a-b`  |                 |
| 5          |  `<<`,`>>`     |                 |
| 6          |  `>`,`>=`      |                 |
|            |  `<`,`<=`      |                 | 
| 7          |  `==`,`!=`     |                 |
| 8          |  `/\`          |                 | Bitwise and 
| 9          |  `\/`,`\'/`    |                 | Bitwise or and Bitwise xor
| 10         |  `and`         |                 | 
| 11         |  `xor`         |                 |
| 12         |  `or`,`->`     |                 |
| 13         |  `a ? b : c `  | Right-to-Left   |
| 14         |  `=`           |                 |
