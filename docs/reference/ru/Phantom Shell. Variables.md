### Переменные

#### Создание переменных:

Переменные создаются с помощью ключевого слова `def`. Поскольку мы разрабатываем командный язык,
то было решено выбрать именно `def`, а не `var` из-заболее отчетливых ассоциаций со словом "команда" 

```
def intVar = 16           // создание переменной типа Integer
def floatVar = 16.16      // создание переменной типа Float
def strVar = "some text"  // создание переменной типа String
```

Получение значения переменной:
Рассматривая данный вопрос, мы выбирали между классическим и простым способом, используемом в различных языках
программирования и чуть более замысловатым, используемом в командном языке bash. По результатам голосования было
решено сделать акцент на активном использовании в ОС Phantom объектов и поддержать аккуратность и лаконичность кода.
Таким образом, следующая последовательность комманд создаст переменную `a` типа `Integer`, а затем создаст переменную
`b` того же типа с начальным значением, равным значению первой переменной.

```
def a = 32
def b = a
```
