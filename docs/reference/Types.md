### Типы данных

Пользователю предлагается использовать пять основных типа данных:
 
 * Базовые типы
    * `Bool   ` - булево значение, примитив 
    * `Integer` - целочисленное значение со знаком размером 32 бита, примитив 
    * `Float  ` - число с плавающей запятой одинарной точности размером 32 бита, примитив 
    * `Char   ` - символ, примитив 
    * `String ` - последовательность символов 

 * Производные типы 
    * [Массивы][1]
    * [Объекты][2]

---
#### Неявное приведение типов

    * `Integer` + `Float  ` = `Float  `
    * `Integer` + `String ` = `String `
    * `Float  ` + `String ` = `String `
    * `Integer` + `Char   ` = `String `    
    * `Float  ` + `Char   ` = `String `
    * `Char   ` + `String ` = `String `
    
[1]: https://github.com/potemin1999/phantomshell/tree/master/docs/reference/Arrays.md
[2]: https://github.com/potemin1999/phantomshell/tree/master/docs/reference/Objects.md
