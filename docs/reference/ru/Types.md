### Типы данных

Пользователю предлагается использовать пять основных типа данных:
 
 * Базовые типы
    * `bool   ` - булево значение, примитив 
    * `int    ` - целочисленное значение со знаком размером 32 бита, примитив 
    * `float  ` - число с плавающей запятой одинарной точности размером 32 бита, примитив 
    * `char   ` - символ, примитив 
    * `string ` - последовательность символов 

 * Производные типы 
    * [Массивы][1]
    * [Объекты][2]

---
#### Неявное приведение типов

    * int     + float   = float  
    * int     + string  = string 
    * float   + string  = string 
    * int     + char    = string     
    * float   + char    = string 
    * сhar    + string  = string 
    
[1]: https://github.com/potemin1999/phantomshell/tree/master/docs/reference/ru/Arrays.md
[2]: https://github.com/potemin1999/phantomshell/tree/master/docs/reference/ru/Objects.md
