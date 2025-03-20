# Hello World!

Setting up a hello world program in maven is simple, one thing needs to be considered first:
Typing: Typing can be either 'static' or 'dynamic'
- Static typing is where once a variable is provided a type, declared or otherwise, that type is a constant
- Dynamic typing is the opposite of static typing meaning the variable can have any type



```maven
// HelloWorld.mvn
#ruleset(typing="static")

// Things that won't work with static typing:

// Decleration of unmapped functions
fn unmapped(argone,argtwo){
    argone + argtwo
}

// Changing type of variables
mut x = 1 // i32
x = "hello world" // String, String is different from i32 therefore an error is thrown

// hello world:
println("Hello World")
```

The ruleset for typing must be declared within the file anywhere, however it must not contain variables and must be a string

For example:

```maven
x = 1
#ruleset(typing="static")
y = 2
#ruleset(typing="dynamic")
// the last ruleset declared will be the ruleset in effect for the content of the file thus the file has a dynamic ruleset
```
as of version beta (any version before 1.0), dynamic compilations will have more precise behavior but slower than static compilations which are transpiled into C++.


*parts with [S] refer to static only and similarly [D] refer to dynamic only*


# The Basics

### Variables
When defining a variable you have to consider what type of variable it is. This is useful in accurately predicting the behavior of a variable and how you want to use it.

```maven
// Defining variables does not require an annotation, since is inferred but an annotation can be used for simple type conversions eg.

my_u64: u64 = 2 // 2 is automatically converted from i32 to u64

// alternatively you can also:

my_i64 = 2i64 // you could annotate this as well but it would just be an i64 to an i64 since 2 is already an i64

// if you're a fan of type function converting:

my_f32 = f32(1.2)

// incorrect annotations
// correctly done:
var my_anno: Character = Character(102) // 102 ASCII is f
var my_anno: Character = none // none cannot be converted to Character automatically
// As none can't be a Character, the type checker will throw a type mismatch as:
// my_anno:
// Expected: Character
// Got: none


// If you're familiar with programming concepts, skip the below
// Knowing variable types are important to use them

Strong_String = "Hi" // inferred as type String

Num = 1 // inferred as type i32

// if you didn't know the inferred types and say you wanted to add two strings together to make the string "Hi1"

result = Strong_String + Num // Error: Invalid operation String+i32
// It is important to know your variable's type, to do it properly:
One_String = "1" // inferred as type String
result = Strong_String + One_String // result is now "Hi1"
```

### Strings

For those just starting programming, Strings are essentially a collection of characters to form text
For example:
```maven
My_text = "Hello world"
```