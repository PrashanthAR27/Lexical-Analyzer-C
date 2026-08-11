# Lexical Analyzer in C

A modular Lexical Analyzer implemented in **C99** that reads a C source file and identifies different types of tokens such as keywords, identifiers, literals, headers, operators, comments, and invalid tokens.

## Project Objective

The main objective of this project is to understand the **lexical analysis stage of a compiler** and gain practical experience with C programming concepts such as file handling, structures, enumerations, strings, character processing, and modular programming.

## Features

- Identifies C keywords
- Identifies identifiers
- Identifies integer and floating-point literals
- Identifies character and string literals
- Identifies header files
- Identifies operators
- Identifies single-line and multi-line comments
- Detects invalid tokens
- Displays tokenized output
- Displays token statistics

## Technologies Used

- **Language:** C
- **Standard:** C99
- **Compiler:** GCC
- **Platform:** Linux / WSL

## Project Structure

```text
Lexical-Analyzer-C/
│
├── main.c
├── lexer.c
├── lexer.h
├── scanner.c
├── scanner.h
├── display.c
├── display.h
├── keywords.c
├── keywords.h
├── token.h
├── sample.c
├── sample_output.txt
└── .gitignore
