# Linkage

Linkage is a next generation programming language that allows for the use of other languages' features.
It has easy to use syntax and a simple compiler.

---

## Compilation

Compilation using Linkage is simple.

```console
llsc file/to/compile.lk <arguments>
```

Then the compiled file can be run.

```console
./executable_name <arguments>
```

## Syntax

Linkage's syntax is as follows:

### The Main Function

```
# main.lk

# main function
fn main (argc: num, argv: arr<str>): int {
    return argc # argc can be replaced with other integers
}
```

### Variables

```
;foo = 32 # variable
;;bar = 16 # constant

;baz: str = "Hello, World!" # typed variable
;;buz: arr<num> = [1, 2, 3] # typed constant
```

### Reassignment

```
foo = bar #
foo += 2 # foo = foo + 2
foo *= 3 # foo = foo * 3
foo /= 3 # foo = foo / 3
foo -= 2 # foo = foo - 2
foo ^= 2 # foo = foo ^ 2
foo `= 2 # foo = foo ` 2
```

### Binary Operations

```
foo + bar # + operator used for addition
foo - bar # - operator used for subtraction
foo / bar # / operator used for division
foo * bar # * operator used for multiplication
foo ^ bar # ^ operator used for powers
foo ` bar # ` operator used for roots
```

### Unary Operators

```
-foo # negative foo
foo-- # decrement foo
foo++ # increment foo
foo^^ # square foo
foo`` # square root foo
```
