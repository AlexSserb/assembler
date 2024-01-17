# Assembler

## Table of contents
* [Introduction](#introduction)
* [Architecture](#architecture)
* [Tools and technologies](#tools-and-technologies)
* [Assembly language file format](#assembly-language-file-format)
* [Formal assembly language](#formal-assembly-language)
* [Assembly code example](#assembly-code-example)
* [Program execution](#program-execution)

<a name="Introduction"></a>
## Introduction

An assembler is a translator of a program from an assembly language text to a machine language program. 

This assembler generates code that can be executed by the virtual machine I developed earlier: https://github.com/AlexSserb/virtual-machine. In this repository, you can familiarize yourself with the architecture of the virtual machine and the code that it can process.

The architecture of the virtual machine, the format of the processed codes and the assembly language were developed by me according to the task of the laboratory work.

Requirements for the assembler program:
* The program must be called from the command line and take as a parameter the path to the executable file with the code
* The executable file with the code must contain instructions in assembly language, where one line = one instruction
* The developed assembly language should support the following:
  - Creating variables and loading their addresses into registers
  - Arithmetic operations on signed integers, unsigned integers and fractional numbers
  - Conditional and unconditional transition commands
  - Executing expressions with integers at the translation stage
  - Creating and calling subroutines
  - Recursive subroutine invocation
  - Creating comments using the symbol "#"
* The translation result must be successfully executed by the previously developed virtual machine.

<a name="architecture"></a>
## Architecture

A Virtual Machine (VM) is a program that is designed to interpret the bytecode of other programs based on a language.
This virtual machine interprets the code that is passed to it in a file after processing by the assembler program.

The following RISC-like architecture was used to develop the virtual machine:
* PSW = IP + Flags = 16 + 16 = 32 bits
* Memory: words – 32 bits, address size – 16 bits
* Data types:
  - Signed integers – 1 word
  - Unsigned integers - 1 word
  - Fractional – 1 word
* Address registers – 256 pieces of 16 bits each
* Command structure: 1 word = 32 bits. Consists of:
  - operation code – 8 bits
  - regiter 1 – 8 bits
  - regiter 2 – 8 bits
  - regiter 3 – 8 bits
* Command for loading the address register: 1 word = 32 bits. Consists of:
  - operation code – 8 bits
  - register 1 – 8 bits
  - address (constant) – 16 bits
* Unconditional transitions:
  - r1 is the type of transition. There are four types:
     - 0: direct, IP = address (constant) in the command
     - 1: direct indirect, IP = the value in memory that the address (constant) in the command points to
     - 2: direct indirect register, IP = reg2 + reg3
     - 3: relative, IP = IP + offset; offset = constant in the command
* Conditional transitions have the same structure, but also check flags
* Subroutine call – the return address is stored in reg1
* Return from subroutine is an unconditional direct transfer (address in reg1)
* A stack is used for recursive subroutine calls. The stack is simulated by the last 16 registers (240 to 255)

<a name="tools"></a>
## Tools and technologies
* C++
* Linux Debian (with Oracle VM Virtual Box)
* Git
* Codeblocks IDE


<a name="assembly-language-file-format"></a>
## Assembly language file format

The file format is in assembly language:
1. Definition of procedures, variables (commands can be defined, but the program will start running with the "start" label)
2. The keyword "start" to declare the main procedure
3. The body of the main procedure (commands, variable declarations)
4. The keyword "end", indicating the end of the main procedure
5. Defining procedures, variables (commands can be defined, but the program will finish executing after the "end" label)

The format of the procedure in assembly language:
1. The keyword "proc" and the name of the procedure
2. The body of the procedure (commands, variable declarations)
3. The keyword "endp", indicating the end of the procedure


<a name="formal-assembly-language"></a>
## Formal assembly language

Description of the syntax of the assembly language developed by me in the extended Backus-Naur form:

```
<program> ::= { <code_line> | <procedure> } <main_prog> { <code_line> | <procedure> }

<main_prog> ::= start { <code_line> } end

<procedure> ::= proc <name> { <code_line> } endp

<code_line> ::= <var_definition> | <command_line> | <mark>

<mark> ::= <name>:

<name> ::= <letter>{<letter> | <digit>}

<letter> ::= a | b | c | d | e | f | g | h | i | j | k | l | m | n | o | p | q | r | s | t | u | v | w | x | y | z | _

<number> ::= <digit> { <digit> }

<digit> ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

<var_definition> ::= <type> <name> <number>

<type> ::= uint | int | float

<command_line> ::= <command> <oper> [,] [<oper>] [,] [<oper>]

<command> ::= <letter> { <letter> }

<oper> ::= <expr>

<expr> ::= [ ( ] <number> | <number> + <expr> | <number> - <expr> | <number> * <expr> | <number> / <expr> | <number> % <expr> [ ) ]
```
Notation in the extended form of backus naur:
* [ ... ] - The syntactic construction may be missing
* { ... } - Repetition of the syntactic construction (possibly 0 times)


<a name="assembly-code-example"></a>
## Assembly code example
An example of an assembler program for sequentially reading three numbers and outputting their factorials:

```
# A program for calculating the factorials of three numbers
start
uint start 1
uint res (2-2)-(-6+5)
uint const1 1
uint temp 0
# Three procedure calls
call printFact
call printFact
call printFact
end

# The procedure for calculating and printing the factorial
proc printFact
load 1, start
load 2, temp
load 3, res
load 4, const1
readu 2
againMulMark:
cmpu 1, 2
jgu printMark
mul 3, 3, 1
inc 1
jmp againMulMark
printMark:
printu 3
loadrv 1, 4
loadrv 3, 4
endp

```

This program demonstrates:
* definition of the main subroutine (it starts with the "start" label and ends with the "end" label)
* definition of a subroutine for calculating the factorial (it starts with the "proc" label and ends with the "endp" label)
* multiple subroutine calls (instractions CALL in main subroutine)
* evaluating an expression when declaring a res variable. Instead, the assembler substitutes only one number - the result of the expression.


As a result of the assembler's work, the output will be the following code for the virtual machine:

```
k 1 3 10 
u 1 
u 1 
u 1 
u 0 
k 51 18 
k 51 18 
k 51 18 
k 0 0 
k 23 1 2 
k 23 2 8 
k 23 3 4 
k 23 4 6 
k 43 2 
k 27 1 2 
k 6 0 38 
k 33 3 3 1 
k 40 1 
k 1 0 28 
k 21 3 
k 50 1 4 
k 50 3 4 
k 54

```

Next, the virtual machine reads this code from the file, loads it into its isolated memory and begins to execute.

By executing this code, the VM will prompt us to enter three numbers sequentially. After each number, its factorial will be displayed. After entering the number 5, 120 will be output (5! = 120). After entering 3, 6 will be output (3! = 6). After entering 10, 3628800 will be output (10! = 3628800).

The result of the program execution:
```
5           // input 1
120         // result 1
3           // input 2
6           // result 2
10          // input 3
3628800     // result 3
```

<a name="program_execution"></a>
## Program execution

To run the assembler, you need to:
1. Create an executable Assembler file using the Assembler project
2. Create a VirtualMachine9 executable file using the VirtualMachine project
3. Place both executable files in the same folder
4. Call the Assembler file and pass the path to the code file as a command-line parameter. This command will have the following format:
```bash
$ /home/user/path_to_executable_file/Assembler /home/user/path_to_ASM_file/file.txt
```
