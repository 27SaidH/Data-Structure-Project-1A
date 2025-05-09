#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <cctype>
#include <cmath>

using namespace std;

/**
 * Custom exception type for signaling parsing or evaluation errors in expressions.
 */
class ExpressionError : public runtime_error {
public:
    ExpressionError(const string& msg) : runtime_error(msg) {}
};

/**
 * MathLogicEvaluator: A class for parsing and evaluating complex infix expressions
 * with both arithmetic and logical operators, including error checking.
 */
class MathLogicEvaluator {
private:
    /**
     * A map storing operator precedence values.
     * Higher values mean higher precedence.
     */
    unordered_map<string, int> operatorPrecedence = {
        {"||", 1}, {"&&", 2}, {"==", 3}, {"!=", 3},
        {">", 4}, {">=", 4}, {"<", 4}, {"<=", 4},
        {"+", 5}, {"-", 5},
        {"*", 6}, {"/", 6}, {"%", 6},
        {"^", 7},
        {"!", 8}, {"++", 8}, {"--", 8}, {"neg", 8}  // "neg" = unary minus
    };

    /**
     * Checks if an operator is right-associative.
     * Right-associative operators are evaluated from right to left (e.g., exponentiation).
     */
    bool isRightAssociative(const string& op) {
        return op == "^" || op == "!" || op == "++" || op == "--" || op == "neg";
    }

    /**
     * Determines if a string is a unary operator.
     * Unary operators operate on a single operand (e.g., -3 or !1).
     */
    bool isUnaryOperator(const string& op) {
        return op == "!" || op == "++" || op == "--" || op == "neg";
    }

    /**
     * Tokenizes an input string into a vector of valid expression elements (tokens),
     * including numbers, operators, and parentheses. Also handles implicit unary minus.
     */
    vector<string> tokenizeExpression(const string& expr) {
        vector<string> tokens;
        string number;

        for (size_t i = 0; i < expr.length(); ++i) {
            char c = expr[i];

            // Skip whitespace
            if (isspace(c)) continue;

            // Parse numbers
            if (isdigit(c)) {
                number += c;
                while (i + 1 < expr.length() && isdigit(expr[i + 1]))
                    number += expr[++i];
                tokens.push_back(number);
                number.clear();
            }
            // Handle operators and parentheses
            else {
                string current(1, c);

                // Try to form a two-character operator (e.g., >=, <=, ==, ++, etc.)
                if (i + 1 < expr.length()) {
                    string twoChar = current + expr[i + 1];
                    if (operatorPrecedence.count(twoChar)) {
                        tokens.push_back(twoChar);
                        ++i;
                        continue;
                    }
                }

                // If '-' is at the beginning or after a left parenthesis or another operator,
                // treat it as unary negative (e.g., -3 becomes "neg 3")
                if (current == "-" && (tokens.empty() || tokens.back() == "(" || operatorPrecedence.count(tokens.back()))) {
                    tokens.push_back("neg");
                } else {
                    tokens.push_back(current);
                }
            }
        }

        return tokens;
    }

    /**
     * Performs error checking on the list of tokens to identify malformed expressions.
     * Checks for issues like consecutive operators, misplaced parentheses, and division by zero.
     */
    void validateTokenSequence(const vector<string>& tokens) {
        for (size_t i = 0; i < tokens.size(); ++i) {
            const string& token = tokens[i];
            const string& prev = (i > 0) ? tokens[i - 1] : "";
            const string& next = (i + 1 < tokens.size()) ? tokens[i + 1] : "";

            // Can't start with a closing parenthesis
            if (token == ")" && i == 0)
                throw ExpressionError("Expression can't start with a closing parenthesis @ char: 0");

            // Can't start with a binary operator
            if (operatorPrecedence.count(token) && !isUnaryOperator(token) && i == 0)
                throw ExpressionError("Expression can't start with a binary operator @ char: 0");

            // Two binary operators in a row (e.g., "3 && && 4")
            if (operatorPrecedence.count(token) && !isUnaryOperator(token) &&
                operatorPrecedence.count(prev) && !isUnaryOperator(prev))
                throw ExpressionError("Two binary operators in a row @ char: " + to_string(i));

            // Two numbers in a row (e.g., "4 5")
            if (isdigit(token[0]) && !prev.empty() && isdigit(prev[0]))
                throw ExpressionError("Two operands in a row @ char: " + to_string(i));

            // A unary operator directly followed by a binary operator (e.g., ++ < 5)
            if (isUnaryOperator(token) && !next.empty() && operatorPrecedence.count(next) && !isUnaryOperator(next))
                throw ExpressionError("A unary operand can’t be followed by a binary operator @ char: " + to_string(i + 1));
        }
    }

    /**
     * Converts infix tokens to postfix (Reverse Polish Notation) using the Shunting Yard Algorithm.
     */
    vector<string> convertToPostfix(const vector<string>& tokens) {
        vector<string> output;
        stack<string> operators;

        for (const string& token : tokens) {
            if (isdigit(token[0])) {
                output.push_back(token);
            } else if (token == "(") {
                operators.push(token);
            } else if (token == ")") {
                while (!operators.empty() && operators.top() != "(") {
                    output.push_back(operators.top());
                    operators.pop();
                }
                if (!operators.empty()) operators.pop(); // Remove the '('
            } else {
                while (!operators.empty() && operators.top() != "(" &&
                       ((isRightAssociative(token) && operatorPrecedence[token] < operatorPrecedence[operators.top()]) ||
                        (!isRightAssociative(token) && operatorPrecedence[token] <= operatorPrecedence[operators.top()]))) {
                    output.push_back(operators.top());
                    operators.pop();
                }
                operators.push(token);
            }
        }

        // Pop remaining operators
        while (!operators.empty()) {
            output.push_back(operators.top());
            operators.pop();
        }

        return output;
    }

    /**
     * Applies a binary operator to two integer operands.
     */
    int calculateBinaryOperation(const string& op, int left, int right) {
        if (op == "+") return left + right;
        if (op == "-") return left - right;
        if (op == "*") return left * right;
        if (op == "/") {
            if (right == 0) throw ExpressionError("Division by zero");
            return left / right;
        }
        if (op == "%") return left % right;
        if (op == "^") return pow(left, right);
        if (op == "==") return left == right;
        if (op == "!=") return left != right;
        if (op == ">") return left > right;
        if (op == "<") return left < right;
        if (op == ">=") return left >= right;
        if (op == "<=") return left <= right;
        if (op == "&&") return left && right;
        if (op == "||") return left || right;

        throw ExpressionError("Unknown binary operator: " + op);
    }

    /**
     * Applies a unary operator to a single integer operand.
     */
    int calculateUnaryOperation(const string& op, int operand) {
        if (op == "!") return !operand;
        if (op == "++") return operand + 1;
        if (op == "--") return operand - 1;
        if (op == "neg") return -operand;

        throw ExpressionError("Unknown unary operator: " + op);
    }

    /**
     * Evaluates a postfix expression using a value stack.
     */
    int evaluatePostfixExpression(const vector<string>& postfix) {
        stack<int> values;

        for (const string& token : postfix) {
            if (isdigit(token[0])) {
                values.push(stoi(token));
            } else if (isUnaryOperator(token)) {
                if (values.empty()) throw ExpressionError("Missing operand for unary operator");
                int operand = values.top(); values.pop();
                values.push(calculateUnaryOperation(token, operand));
            } else {
                if (values.size() < 2) throw ExpressionError("Missing operands for binary operator");
                int right = values.top(); values.pop();
                int left = values.top(); values.pop();
                values.push(calculateBinaryOperation(token, left, right));
            }
        }

        if (values.size() != 1) throw ExpressionError("Expression evaluation error: leftover operands");
        return values.top();
    }

public:
    /**
     * Main function to be called from main().
     * Parses, validates, and evaluates a complete infix expression.
     */
    int evaluate(const string& expression) {
        vector<string> tokens = tokenizeExpression(expression);
        validateTokenSequence(tokens);
        vector<string> postfix = convertToPostfix(tokens);
        return evaluatePostfixExpression(postfix);
    }
};

/**
 * Entry point of the program. Evaluates a sample expression and prints the result.
 */
int main() {
    MathLogicEvaluator evaluator;

    try {
        // Change this expression to test other cases
        int result = evaluator.evaluate("1 + 2 * 3");
        cout << "Result: " << result << endl;  // Output: Result: 7
    } catch (const ExpressionError& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
