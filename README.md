# Brainrot Language

A **meme-based programming language** inspired by the Skibidi Toilet universe. 🧠🚽

## About

Brainrot is a programming language that uses sigmas keywords for common programming constructs, like `gyat` for `int`, `beta` for `if`, and so on.

(It's designed for fun but also works as a fully functional programming language that compiles down to assembly.
I honestly don't know why i made this. When i'm going to have more free time i will surely finish this "project" but for now it's kinda an "experiment" on how to make a compiler. Feel free to open an issue or make a pull request to fix this beta 0 rizz code)

---

## How to Use

### 1. Compile the Compiler
Use the following command to compile the Brainrot compiler:

```bash
gcc brainrot_compiler2.c -o brainrot_compiler
```

### 2. Write Your Code
Create a `.ohio` file using the Brainrot syntax. For example:

```c
toiletskibidi main() {
    gyat x = 10;
    beta (x < 20) {
        yapper("Hello, Brainrot!\n");
    }
    nomilk;
}
```

### 3. Compile Your Code
Compile your `.ohio` file to assembly using the Brainrot compiler:

```bash
./brainrot_compiler your_code.ohio output.asm
```

### 4. Run Your Code
Use an assembler like `nasm` to assemble the generated assembly file into an executable:

```bash
nasm -f elf64 output.asm -o output.o
ld output.o -o output
./output
```

---

## Brainrot Syntax Table

Below is a reference table for the Brainrot syntax:

| Brainrot Keyword  | C Equivalent | Description                      |
|--------------------|--------------|----------------------------------|
| `gyat`            | `int`        | Integer type                    |
| `grimace`         | `float`      | Floating-point type             |
| `yap`             | `char`       | Character type                  |
| `caseoh`          | `double`     | Double precision float          |
| `toiletskibidi`   | `void`       | Void type                       |
| `beta`            | `if`         | Conditional (if)                |
| `sigma`           | `else`       | Conditional (else)              |
| `edging`          | `while`      | While loop                      |
| `mangophonk`      | `for`        | For loop                        |
| `gooning`         | `do`         | Do-while loop                   |
| `nomilk`          | `return`     | Return statement                |
| `woke`            | `break`      | Break statement                 |
| `autoblu`         | `continue`   | Continue statement              |
| `yapper`          | `printf`     | Print formatted output          |
| `caseoh`          | `scanf`      | Scan formatted input            |
| `grind`           | `sizeof`     | Sizeof operator                 |
| `getout`          | `goto`       | Goto statement                  |

---

## Features

- **Dynamic Syntax**: Replace boring beta keywords with very sigma ones.
- **Assembly Output**: Generate `.asm` files from your `.ohio` code.

---

## To-Do

- [ ] Improve the assembler to compile `.asm` files automatically.
- [ ] Add support for more complex data structures.
- [ ] Create a standard library for common tasks.
- [ ] Actually fixing the assembler since it just generates a very beta, not sigma, 0 rizz "Hello world!" (☝️🤓) instead of real sigma rizzler kai cenat code.


---

### Example

Here’s a sample Brainrot program:

```c
toiletskibidi main() {
    gyat x = 10;
    beta (x < 20) {
        yapper("swag like ohio, 100% kai cenat rizz\n");
    }
    nomilk;
}
```

Compiling and running this will print:

```
swag like ohio, 100% kai cenat rizz
```

---

## Contributing

Want to add features or fix bugs? Feel free to submit a pull request or open an issue. Real sigmas with kai cenat rizz help other sigmas out.

---

## License

Apache-2.0 license
