# Contribution Guidelines
For the scale of this project is tiny so far, we will not give a detailed Contribution Guideline. Instead, we will gradually add to this documentation during development.

## File Structure
* `include/OpenPT/` contains exported functions.
* `lib/` contains 3rd-party libraries.
* `src/` contains private code.
* `tests/` is the Unit Test module.

## Naming Conventions
Explain by examples:
```cpp
/*
Note that you should put the definition and declaration respectively in the .h and .cpp files in the same folder.

!!! THIS IS ONLY A DEMONSTRATION FOR NAMING CONVENTIONS.
*/

#define PRECOMPILED_MACROS 3.14159f

int global_variable; // You should avoid using this.

class ClassName
{
    int member_variable;

    ClassName(int member_variable_) : member_variable(member_variable_) {}

    void Function()
    {
        auto temp_variable;
    }
};
```
You should **avoid** meaningless variable names unless the usage of the variable is very (I mean really) obvious like for-loop.

You should also **avoid** long names.

## Commenting Conventions
Every document should begin with a multi-line comment:
```cpp
/*
 *  OpenPT - <Module Name>
 *  File created on yyyy/MM/dd
 *  Last edited on yyyy/MM/dd (Editor name)
 *  Contributor name 1 <email1@example.com>
 *  Contributor name 2 <email2@example.com>
 */
```
In other areas, comments starting with slashes `//` should be used whenever possible. We make no requirements on the position of comments.

Commenting specifications for functions and classes are TBD.

## Object Oriented Programming Conventions
* **Declaring / Defining multiple classes in the same file** is permitted but not recommended, unless the classes have extreme functional similarity, or are only coupled within the same file.
* **Place everything in the same namespace called `OpenPT`**.
* **Always avoid global variables / functions**.