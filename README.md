# SevLang
First transpiler written in C by a newbie :3

## What is this?
This goes without saying, but this language is not meant for production nor is this a serious attempt at one. It took a little more than 4 days to finish this project with very little research done. However, it was a pretty interesting learning experience and I would like to revisit making my own programming language using something like LLVM and actually doing it correctly. SevLang is my attempt at a transpiler by reading a custom format and transforming it into runnable C code. 

## Why Sev?
I don't know. I didn't want to think of anything cool and slap it on a throw away project.

## Features *(and lack of thereof)* 
- [x] Declare functions
- [x] Call functions from C. This works by pasting includes at the top of the file before transpiling Sev code lol. 
- [x] Create and assign variables values. Current types are *int* and *float*.
- [x] Mathematical operations. Do it like it's done in C
- [ ] Meaningful error messages
- [ ] Conditions and loops
- [ ] Making custom types
- [ ] Everything you would expect in the language except for the aforementioned checkmarked features.

