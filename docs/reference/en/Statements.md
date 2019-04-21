### Statements

There is 5 types of statements:
 * compound statement
 * expression statement
 * selection statement
 * iteration statement
 * jump statement
 
---
##### Compound statements
This is a block of statements

    { <statement> }

---
##### Expression statements
Just an expression

    <expression>


---
##### Selection statements
simple if statement

    if <expression> {
        <statement>
    }
    
if statement with else block

    if <expression> {
        <statement> 
    } else { 
        <statement>
    }
    
if statement with elif and else blocks
 
    if <expression> {
        <statement>
    } elif <expression> { 
        <statement>
    } else {
        <statement>
    }
    
switch statement ([BNF reference][1])

    switch <expression> {
        <switch-selection-statement>
    }
   
---
##### Iteration statements
while-do statement
    
    while <expression> {
        <statement>
    }
    
do-while statement

    do { 
        <statement>
    } while <expression>
    
for statement

    for <expression> ; <expression> ; <expression> {
        <statement>
    }
        
foreach statement

    for <identifier> in <expression> {
        <statement>
    }

---     
##### Jump statements
return statement

    return <expression>
    
    
[1]: https://github.com/potemin1999/phantomshell/tree/master/docs/markdown/BNF.md