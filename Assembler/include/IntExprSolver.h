#ifndef INTEXPRSOLVER_H
#define INTEXPRSOLVER_H

#include <string>
#include <stack>
#include <iostream>

using std::string;

class IntExprSolver {
public:
	// Expression Evaluation
	int solve(const string& expr);

	static bool is_expr(const string& expr) noexcept;

private:
	std::stack<int> values;		 // Expression values
	std::stack<char> operations; // Expression Operations
	string expr;			     // Expression
	int may_unary;				 // Can the following operation be unary

	// Is a symbol an operation
	static bool is_operation(const char ch) noexcept;

	// Getting the priority of an operation
	static int oper_priority(const char ch) noexcept;

	// Solving one element of an expression (1 or 2 operands and 1 operation)
	void solve_operation() noexcept;

	// Solving one element of an expression (2 operands and 1 operation)
	void solve_two_operand_operation(const char& operation) noexcept;


	// Processing a character from a string indicating an operation
	void operation_symbol_processing(char op) noexcept;

	// Processing a character from a string indicating a digit
	void digit_symbol_processing(int& str_index) noexcept;

	// Processing a character from a string indicating a closing parenthesis
	void close_bracket_processing() noexcept;

	// Checking the correctness of parentheses
	bool is_correct_brackets() const noexcept;
};

class exprInvalidBracketsException : public std::exception
{
public:
    virtual const char* what() const noexcept
    {
        return "Error evaluating expression. The parentheses in the expression are not placed correctly.";
    }
};

class exprOtherSymbolException : public std::exception
{
public:
    virtual const char* what() const noexcept
    {
        return "Error evaluating expression. An invalid symbol was found.";
    }
};

class divByZeroException : public std::exception
{
public:
    virtual const char* what() const noexcept
    {
        return "Error evaluating expression. Division by zero.";
    }
};

#endif // INTEXPRSOLVER_H
