Pointers in Maven work a little differently from other languages.

Specifically in the way that when you make a reference to a variable it copies the value and holds it within itself.

So first types of pointers:

Pointer - Owned by the scope it's in, mutable always, only one of it can exist

BorrowedPointers - Can be owned by any scope, can be mutable or immutable, multiple can exist, does not contain the actual value referenced


Example:
```
my_value = "Hello!"
my_pointer = &my_value // it doesn't actually refer to my_value but copies the value into itself

// so if we edit this pointer,
my_pointer &= "Hello! World!"

value_of_my_pointer = *my_pointer // "Hello! World!"

// however my_value, the referenced variable is still "Hello!", because the Pointer creates a copy of the value

// So in a C-style usage, say passing a variable to a function through a pointer
mut passed_value = 100

addbyone(&passed_value) // will not changed passed_value at all it'll just free the pointer automatically

// Maven usage:
passed_value = 100

ptr = &passed_value // same as &100

addbyone(ptr)

ptr_value = *ptr // 101
passed_value // 100

// this has been done to emphasize usage of pointers and not
// to use it in a cstyle way:
value = &120

addbyone(value)

*value // 121

// Other notes for Pointers:
// There can only be one variable holding a Pointer to a specific address
// eg.
x = &1
y = x // y is now Pointer(x) and x no longer exists
//x // variable non-existent error


// BorrowedPointers and trading:
// say we pass the pointer to addbyone and it just assigns it, making us lose the Pointer object
// eg.

fn addbyone(ptr){
    x = ptr
    // even if it does not assign it, ptr is implicitly assigned as Pointer so it will still be freed
    x &= *x + 1
    // Pointer is now set to 1
} // pointer is freed at end of scope, we have lost the pointer we passed it is now a nullptr


// Method 1 - BorrowedPointers

value2 = &120

bptr = &b value2 // immutable borrowing of pointer to extract the value and use it however
// &b <variable containing a Pointer or Pointer directly>

// since addbyone modifies the pointer,
addbyone(&b mut value2) // multiple bptrs may exist and this is a mutable version of it

// borrowed pointers will not be freed, only deleted once out of scope, for us this is not an issue
// since the Pointer contains the actual value we point to, thus
now = *value2 // 121

// Method 2 - Trading

// this method involves a modification on the addbyone side mainly

fn addbyone(ptr){
    x = ptr // we receive and assign ptr
    
    x &= *x + 1

    &g x
    // since ptr is no longer a valid reference since it's been assigned
    // x is now the Pointer
    // &g x gives ownership of x as a return value to the receiver
    // &g can only work on Pointers as only Pointers own their values
    // &g also swaps the reference provided with a borrowed pointer
    // so x is now a bptr
}

// now:
mut value3 = &120

value3 = addbyone(value3) // this ensures we receive the owned pointer

*value3 // 121

// Summary:

start = "Hello!"

ptr = &start

bptr = &b mut ptr

bptr &= "Hello from mutable borrowed pointer"

// *ptr is now "Hello from mutable borrowed pointer"

ptr &= "Hey what's up"

// *ptr is now "Hey what's up"

// etc

Iwantthisptr = ptr

// ptr as a variable no longer exists
// however Iwantthisptr contains it's Pointer object

*Iwantthisptr // "Hey what's up"

give = &g Iwantthisptr // since it's within the same scope
// the only effect is to swap Iwanthisptr with an IMMUTABLE bptr
// give is now the Pointer object
// *give is "Hey what's up"


// Generic use cases:
// say we have a block we want to give us a Pointer
x = {
    y = &"100 strings"
    y
}
// this is effectively a dangling pointer, well it gets freed when the scope of the block ends
// so x is a nullptr
// fix:
x = {
    y = &"100 strings"
    &g y
} // ta-da! y is now a bptr and x receives the Pointer object
// TODO: manual free-ing of pointers & .c_ptr() to get the c PtrVal object
```

Fun:
```
// Pointers can be thought off as messengers in a way so:
mut General = "Call the troops"
Msngr = &General
Msngr &= "Yes SIR!"
mut Troop = "We're training in the barracks, we're on strike"
// Msgnr arrives
Msgnr &= "Alright..."
Msgnr &= Troop
// Msgnr goes back
General = "How DARE THEY, tell them to do a thousand pushups" // PE teacher.
Msgnr &= "OK!"
Msgnr &= "Do a thousand pushups"
// Msgnr goes to troop
Troop = "... Bring out the cannons"
Msgnr &= "EEK"
Msgnr &= Troop
// Msgnr goes back
General = "... well- I suppose I could negotiate"
Msgnr &= "I'll only negotiate if you agree to give Msgnr a pay raise"
// Msgnr goes to Troop
Troop = "Deal"
Msgnr &= "Only if Msgnr gets a raise along with us"
// Msgnr goes back to General
General = "Very well."
Msgnr &= ":D"
```