# SevLang
First transpiler written in C by a newbie :3

## What is this?
This goes without saying, but this language is not meant for production nor is this a serious attempt at one. It took a little more than 4 days to finish this project with very little research done. However, it was a pretty interesting learning experience and I would like to revisit making my own programming language using something like LLVM and actually doing it correctly. SevLang is my attempt at a transpiler by reading a custom format and transforming it into runnable C code. 

## If you worked on this some more, what would you add?
I would fix some bugs that crash the program. Furthermore, I would implement a custom standard library for SevLang and use that instead of `stdio.h`. Crucial language features like loops, and if-statements. Types like `bool`, a custom `string` type, `arrays`, `dynamic arrays`, etc. Memory management will still be manual since I'm too lazy to program a garbage collector. Lastly, some sort of system to link multiple files together as modules. Perhaps this would be a program that writes custom Makefiles. Anyways, this was just a throw away project because I was bored and it really took a lot of discipline to finish this (I say *finish* very loosely) and not move to a new project. Ignore the fact I only worked on this for less than a week. Also, lot's of bad, unsafe code was written 😄. 

## Why Sev? 
I don't know. I didn't want to think of anything cool and slap it on a throw away project.

## Features *(and lack of thereof)* 
- [x] Declare functions
- [x] Call functions from C. This works by pasting includes at the top of the file before transpiling Sev code lol. 
- [x] Create and assign variables values. Current types are *int* and *float*.
- [x] Mathematical operations. Do it like it's done in C
- [x] Comments. No multiline comments because we don't process spaces, tabs, etc. 
- [ ] Meaningful error messages
- [ ] Conditions and loops
- [ ] Making custom types
- [ ] Everything you would expect in the language except for the aforementioned checkmarked features.

## DOCUMENTATION 
Alright, my favorite part of making this language is teaching you about it!
`NOTE: YOU MIGHT CRASH IF YOU DON'T ADD AT LEAST ONE SPACE AFTER EVERY LINE. ALSO IT'S VERY EASY TO BREAK WHICH IS ACTUALLY A DESIGN FEATURE TO FORCE THE PROGRAMMER TO NEVER MAKE A MISTAKE! BY THAT I MEAN IT MAY CRASH TO PERFECTLY FINE CODE THAT IS MISSING A SPACE AFTER EACH LINE OR IT MAY OUTPUT C CODE THAT IS BIZARRE AND UNINTENDED.` 
```
#Comment with no spaces. Yes spaces are removed unless#
#"You use quotation marks! Just a byproduct of laziness. End comments with a "#
```
Declaring and assigning variables works like most other languages. Don't try `+=` or `-=` for numbers.
```
let x: int = 69;
let y: float = 4.20;
let z: int = x + 0;
let w : float = y * 20.0 / 20.0;

x = 1 - 1;
y = y + (21.0 * 21.0);
```
Functions are similar... Angular brackets specify a return variable or a literal. Foo returns `void`, with an `int` parameter `x` and a `float` parameter `y`. The *greater than/less than* symbols specify a variable or numeric literal. This is what the function will return should it have a return type of anything but void. 
```
fn foo(x: int, y: float) = void: <>
end
```
```
fn add(x: int, y: int) = int: <result>
  result = x + y;
end
```
```
#"This returns funny number. Like: return 69;"#
fn give_me_num() = int: <69>
end
```
Now to call a function just use: `call my_function()`. 

You can use any function provided by `stdio.h` or really any C library so long as it is included in the file, or it is put into the file by the transpiler. But the lack of types such as a crucial `string` or `char*` type will put you off from a lot of functions. 

Lastly, make sure you include this function in your program, which functions as an entry point similar to `int main(void)` in C.
```
fn main() = int: <0>
  call printf("Hello World!");
end
```

Now let's put everything together:
```
#"This is a comment!"# 
#"We put quotation marks here because that is the only way to have spaces!"# 

fn sub(a: float, b: float) = float: <result> 
  result = a - b;
end 

fn print_float(a: float) = void: <> 
  call printf("%.2f and ", a);
end

fn print_int(a: int) = void: <> 
  call printf("%d\n", a);
end

fn main() = int: <0>
  let foo: float = 69.42;
  call print_float(sub(foo, 21 / 21));
  let x: int = 32;
  call print_int(x);
  call printf("And of course, Hello World!");
end
```
