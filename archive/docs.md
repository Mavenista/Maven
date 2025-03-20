Maven:


Maven is a language meant to be a middle ground between high level and low level with customisable rulesets per file. It also hybrid typed meaning you can opt for strong typing, weak typing along with statically typed or dynamically typed.

Scoping:

Scoping is essentially the life time of variables
```
{
    x = 1
} // x does not exist out of the {}

{
    x = 1
    {
        mut x_copy = x
        x_copy += 1
    } // you can access values from above scopes but it does NOT update them
    // x_copy no longer exists
    y = x + 1
} // x and y no longer exist
```



Typing:
```
// Example 1 - Dynamic typing
// By the way // are comments, annotations for the file


// Dynamic typing example:
mut var = 1
var = "Hello"
// mut declares var as mutable (editable) var is an int32 by default
// var is then edited to a String "Hello", no type check is enforced
```

```
// Example 2 - Static typing

// Static typing example:

[ruleset|static_typing(true)|] // this enables static typing essentially: [<setting category>|<field>(<value>)|]

// Now:
mut value = 1
value = "Hello" // Error type mismatch
// Value can only be an int32 now and changing it's type will cause a type error
```

```
// Example 3 - Weak Typing & Strong Typing


// Weak typing:

f64_value= 3.14
// Maven will take value of the expression provided (in this case decimal)
// and assume it is an f64 (a double), with no input from the user
// this is weak typing

// Strong typing:

f32_value:f32 = 3.14 // The type is annotated as f32, variable : <type> = <expression>
// Therefore Maven will evaluate it as an f32 value

// One more strong typing example

u64_value = 123454321u64 // the type for integers only is stated at the end of the integer <integer><integer type>
// you can reformat this to look better using underscores: 123_454_321_u64

// String strong typing

wchar_value: wchar = "你" // wchar has been specifically annotated so it will receive it as a wchar
// one other thing is a String will be a string regardless of " or ', to use a char annotate it with char
````

Data types:

Tbd

Simple math:
```
// Some simple math with Maven

// Negatives & decimals
nve = -1234

dec = 3.14159

// Operations
pve = (-1 * nve)+32 // multiplication

pve_32 = {nve + 32} * -1 // instead of () for operations {} is used

div = dec / 3.14159 // unless explicitly annotated, the type will remain a f64 as the left expression, dec is a f64

o = div-1 // 0
```

Control Flow / Loops

```
// Control flow
// This is probably the most basic thing ever but using if you can check
// if a condition is fullfiled, if it is not then if another condition is fullfiled
// and if that is not then do this
// eg.
apple = "pear" // meet masquerading apple

mut warden = "Are you an apple?"

warden = if apple == "apple" {"My sincerest apologies for bothering you"}
         elif apple == "grannysmith" {"Why hello Ma'am how's your day?"}
         else {"You're not an apple!"}

// so to break things down if <condition> {<code to execute if true>}
// else if (elif) <condition> {<code to execute if true and if previous condition is NOT true>}
// else {<code to execute if no conditions are true>}

// you can assign it like warden or simply just put it into code
// masquerading apple's response
if warden == "You're not an apple!" {
    apples_response = "Aw shucks you got me"
} else {
    apples_response = "Toodle pip my good man"
}

// Alright so that's control flow

// say we have a generic level 99 regressor (sorry this is a horrible joke)

mut lvl_99_regressor = 0

// their power level is 0 right now

// as they progress throughout the story they gain power levels that increase depending on their current power level
// Power level | increase
// 1           | 1
// 3           | 2
// 7           | 3
// 10          | 100000

// so instead of repeatedly doing if conditions we can use loop!

lvl_99_regressor = loop 10 {
    mut lvl_99_regressor = lvl_99_regressor
    lvl_99_regressor = if lvl_99_regressor < 3 {
        lvl_99_regressor+1
    }
    elif lvl_99_regressor < 7 {
        lvl_99_regressor + 2
    }
    elif lvl_99_regressor < 10 {
        lvl_99_regressor + 3
    }
    elif lvl_99_regressor == 10 {
        lvl_99_regressor + 100000
    }

    if lvl_99_regressor > 10 {
        break: lvl_99_regressor
    }


    
} // 100010


// Boss battle time!
// say you want to repeat an action as long as a condition is met
final = while lvl_99_regressor != 1 || lvl_99_regressor < 0 {
    mut lvl_99_regressor = lvl_99_regressor-10
    lvl_99_regressor
}
```

