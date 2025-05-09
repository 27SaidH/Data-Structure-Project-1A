#ifndef MAIN_H
#define MAIN_H

#include <string>

/**
 * 
 * @brief Provides functionality to parse, validate, and evaluate
 * infix expressions containing arithmetic, logical, and comparison operators.
 * 
 * This class supports the following:
 * - Unary operators: !, ++, --, unary -
 * - Binary operators: +, -, *, /, %, ^, >, <, >=, <=, ==, !=, &&, ||
 * - Parentheses for grouping

 */
class MathLogicEvaluator {
public:
    /**
     * @brief Parses, validates, and evaluates an infix expression.
     * 
     * @param expression A string containing the infix expression (e.g., "1 + 2 * 3").
     * @return int The result of the expression evaluation.
     * @throws ExpressionError if there are any validation or runtime errors.
     */
    int evaluate(const std::string& expression);
};

#endif // MAIN_H
