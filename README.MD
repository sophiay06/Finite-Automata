# Finite Automata

This project implements a finite automata framework designed to define, execute, 
and explore both deterministic (DFA) and nondeterministic finite automata (NFA). 
The goal is to create a reusable system that can recognize various input patterns 
based on formal automata models.

## Features
1. **Deterministic Finite Automata (DFA):**
- Define and execute DFAs for specific patterns (e.g., strings containing "abc" 
or binary inputs with odd numbers of 0’s and 1’s).
- Provide functions to configure and run DFAs using a REPL (Read-Eval-Print Loop) 
for interactive testing.

2. **Nondeterministic Finite Automata (NFA):**
- Extend the framework to support NFAs for patterns like strings ending with "ing" 
or containing repeated characters.
- Implement REPL functions for NFAs to allow user input and result evaluation.

3. **NFA to DFA Conversion:**
- Implement the standard subset construction algorithm to translate any NFA into an equivalent DFA.
- Demonstrate equivalence by running the converted DFA on various input strings.

## Build and Run Instructions
- **Build**: Use gcc with C99 standards and necessary flags:
```
gcc -std=c99 -Wall -Werror -o EXECUTABLE *.c
```
- **Run**: Execute the program using the compiled executable:
```
./EXECUTABLE
```

## How it Works
- The framework provides a modular approach for creating, configuring, and running finite automata.
- Users can interact with the automata via the REPL, providing input strings to see whether they are 
accepted or rejected by the automaton.