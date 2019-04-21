### Backus-Naur Form

[what's this?](https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form)
 
#### Basic
```
<digit>     ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

<non_digit> ::= a | b | c | d | e | f | g | h | j | i | k | l | m | n | o | p | q | r | s | t | u | v | w | x | y | z | A | B | C | D | E | F | G | H | J | I | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z | _

<escape_symbol> ::= \\ | \n | \b | \f | \r | \t | \v | \0

<char_symbol> ::= <digit>
            | <non_digit>
            | <escape_symbol>

<digit_sequence> ::= <digit><digit>
               | <digit_sequence><digit>

<char_sequence> ::= <char_symbol><char_symbol>
                  | <char_sequence><char_symbol>
                  
<identifier_suffix> ::= <non_digit><identifier_suffix>
                      | <digit><identifier_suffix>
                      | <digit>
                      | <non_digit>
                      

<identifier> ::= <non_digit><identifier_suffix>
                 <non_digit>
                 
<type_specifier> ::= bool
                   | int
                   | float
                   | char
                   | string
                   | <identifier>
```

#### Literals
```
<boolean_literal> ::= true
                    | false

<integer_literal> ::= <digit>
                    | <digit_sequence>

<float_literal> ::= <digit>.<digit>
                  | <digit>.<digit_sequence>
                  | <digit_sequence>.<digit>
                  | <digit_sequence>.<digit_sequence>

<char_literal> ::= ‘<char_symbol>’

<string_literal> ::= ‘<char_sequence>’
                   | “<char_symbol>”
                   | “<char_sequence>”
```

#### Operators
```
<bitwise_unary_operator> ::= ~

<logical_unary_operator> ::= not
                           | !

<unary_operator> ::= <bitwise_unary_operator> 
                   | <logical_unary_operator>
                   
<arithmetic_binary_operator> ::= + 
                               | -
                               | *
                               | /
                               | =

<bitwise_binary_operator> ::= <<
                            | >>
                            | /\
                            | \/
                            | \’/

<comparison_binary_operator> ::= !=
                               | ==
                               | >
                               | <
                               | >=
                               | <=

<logical_binary_operator> ::= and
                            | or
                            | xor
                            | ->
                            
<binary_operator> ::= <arithmetic_binary_operator>
                    | <bitwise_binary_operator>
                    | <comparison_binary_operator>
                    | <logical_binary_operator>

<incdec_operator> ::= ++
                    |
                
```

#### Expressions
```
<constant_expression> ::= <boolean_literal>
                        | <integer_literal>
                        | <float_literal>
                        | <char_literal>
                        | <string_literal>

<ternary_expression> ::= <expression> ? <expression> : <expression>

<expression> ::= <identifier>
               | <constant_expression>
               | <ternary_expression>
               | <expression> <incdec_operator>
               | <unary_operator> <expression>
               | <expression> <binary_operator> <expression>
               | (<expression>)
```

#### Statements
```
<statement>::= <expression> 
             | { <statement> }
             | <selection_statement>
             | <iteration_statement>
             | <jump_statement>

<switch_selection_block> ::= case <constant_expression> { <statement> }
                           | case <constant_expression> { <statement> } <switch_selection_block>
                           | other { <statement> }

<elif_selection_block> ::= elif <expression> { <statement> }
                         | elif <expression> { <statement> } <elif_selection_block>

<selection_statement> ::= if <expression> { <statement> }
                        | if <expression> { <statement> } else { <statement> }
                        | if <expression> { <statement> } <elif_selection_block> else { <statement> }
                        | switch <expression> { <switch_selection_block> }

<iteration_statement> ::= while <expression> { <statement> } 
                        | do { <statement> } while <expression>
                        | for <identifier> in <identifier> { <statement> }
                        | for <expression> ; <expression> ; <expression> { <statement> }

<jump_statement> ::= return <expression>
```

#### Structure
```
<func_declaration> ::= func <identifier>() { <statement> }
                     | func <identifier>(<function_args>) { <statement> }
                     | func <identifier>() <type_specifier> { <statement> }
                     | func <identifier>(<function_args>) <type_specifier> { <statement> }

<function_argument> ::= <identifier>
                      | <identifier>  <type_specifier> 

<function_args> ::= <function_argument>
                  | <function_argument> , <function_args>
                 
<declaration> ::= <func_declaration>

<program> ::= <program> <statement>
            | <program> <declaration>
            | <statement>
            | <declaration>
```