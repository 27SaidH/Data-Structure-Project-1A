# Data-Structure-Project-1A
Overview
This project consists of two parts:

Part 1: Infix Expression Evaluator
An infix expression evaluator was implemented using stacks with the following capabilities:
Tokenization – Parses the input string into numbers, operators, and parentheses while handling flexible spacing (e.g., "1+2" or "1 + 2").


Validation – Checks the input for common syntax errors:


Expressions that start with invalid characters (e.g., binary operators or ))


Consecutive binary operators or operands


Improper chaining of unary and binary operators


Division by zero


Infix to Postfix Conversion – Uses the Shunting Yard algorithm to handle operator precedence and associativity.


Evaluation – Evaluates the expression in postfix form using a stack.


Supported Operators:

Unary: !, ++, --, unary -

Arithmetic: +, -, *, /, %, ^

Logical/Comparison: >, <, >=, <=, ==, !=, &&, ||

Boolean Logic Handling – Returns results using C++'s implicit boolean-to-integer conversion (true = 1, false = 0).

Part 2: Error Reporting
The evaluator reports meaningful error messages for invalid input. It identifies the first error encountered and displays the character index where the issue occurs.
Examples:
)3+2 → “Expression can’t start with a closing parenthesis @ char: 0”


3&&&&5 → “Two binary operators in a row @ char: 3”

-----------------------------------

How to Run the Code
Steps in Visual Studio
Open Visual Studio and create a new C++ project.

Add main.cpp and main.h to the project.

Build the solution (Ctrl + Shift + B).

Run the program (Ctrl + F5).

