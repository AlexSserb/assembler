#include "IntExprSolver.h"

// Expression Evaluation
int IntExprSolver::solve(const string& expr) {
	this->expr = expr;
	if (!is_correct_brackets())
        throw exprInvalidBracketsException();

	operations = std::stack<char>();
	values = std::stack<int>();
	may_unary = true; // Can an operation be unary

	// Translating an expression into reverse Polish notation with
	// parallel evaluation of expressions in parentheses
	for (int i = 0; i < expr.size(); i++) {
		if (expr[i] != ' ') {
			if (expr[i] == '(') {
				operations.push('('); may_unary = true;
			}
			else if (expr[i] == ')') {
				close_bracket_processing();
			}
			else if (is_operation(expr[i])) {
				operation_symbol_processing(expr[i]);
			}
			else if (isdigit(expr[i])) {
				digit_symbol_processing(i);
			}
			else throw exprOtherSymbolException();
		}
	}

	while (!operations.empty()) solve_operation();

	return values.top();
}

bool IntExprSolver::is_expr(const string& expr) noexcept
{
    for (char ch : expr)
    {
        if (!(std::isdigit(ch) || ch == '+'|| ch == '-'|| ch == '*'
            || ch == '/'|| ch == '%'|| ch == ')'|| ch == '(' || ch == ' '))
            return false;
    }
    return true;
}

// Checking the correctness of parentheses
bool IntExprSolver::is_correct_brackets() const noexcept {
	auto brackets = std::stack<char>();
	for (char ch : expr) {
		if (ch == '(') {
			brackets.push('(');
		}
		else if (ch == ')') {
			if (brackets.empty()) return false;
			brackets.pop();
		}
	}

	return brackets.empty();
}

// Processing a character from a string indicating an operation
void IntExprSolver::operation_symbol_processing(char op) noexcept {
	if (may_unary && (op == '-' || op == '+')) op = -op;
	int curr_prior = oper_priority(op); // Operation priority

	// Execute all recent operations on the stack with a priority higher than the current operation
	while (!operations.empty() &&
		((op >= 0 && oper_priority(operations.top()) >= curr_prior) ||
			(op < 0 && oper_priority(operations.top()) > curr_prior))
		) solve_operation();

	operations.push(op); // Add current operation to the stack
	may_unary = true;
}

// Processing a character from a string denoting a digit
void IntExprSolver::digit_symbol_processing(int& str_index) noexcept {
	// If a digit is found, then take subsequent digits and convert
	// them to type Int to create an operand. Place the operand on the stack
	int start_digit = str_index;
	while (str_index < expr.size() && isdigit(expr[str_index])) str_index++;
	int count_digits = str_index - start_digit;
	str_index--;

	int operand = stoi(expr.substr(start_digit, count_digits));
	values.push(operand);
	may_unary = false;
}

// Processing a character from a string indicating a closing parenthesis
void IntExprSolver::close_bracket_processing() noexcept {
	// If a closing parenthesis is found, then calculate everything up
	// to the first opening one, and remove the parentheses themselves
	while (!operations.empty() && operations.top() != '(')
		solve_operation();

	operations.pop();
	may_unary = false;
}

// Is a symbol an operation
bool IntExprSolver::is_operation(const char ch) noexcept {
	return ch == '+' || ch == '-' || ch == '*' ||
		ch == '/' || ch == '%';
}

// Getting the priority of an operation
int IntExprSolver::oper_priority(const char ch) noexcept {
	if (ch < 0) return 3; // Unary operations
	if (ch == '+' || ch == '-') return 1;
	if (ch == '*' || ch == '/' || ch == '%') return 2;
	return -1;
}

// Solving one element of an expression (2 operands and 1 operation)
void IntExprSolver::solve_operation() noexcept {
	char operation = operations.top();
	operations.pop();

	if (-operation == '+')
		return; // Unary plus is not processed

	if (-operation == '-') { // Unary minus
		int value = values.top();
		values.pop();
		values.push(-value);
	}
	else { // Binary operations
		solve_two_operand_operation(operation);
	}
}

// Solving one element of an expression (2 operands and 1 operation)
void IntExprSolver::solve_two_operand_operation(const char& operation) noexcept {
	// Getting Operands
	int right_val = values.top();
	values.pop();
	int left_val = values.top();
	values.pop();

	// Performing an operation with operands.
	// The result is pushed onto the stack as a new operand.
	switch (operation) {
	case '+':
		values.push(left_val + right_val);
		break;
	case '-':
		values.push(left_val - right_val);
		break;
	case '*':
		values.push(left_val * right_val);
		break;
	case '/':
	    if (right_val == 0) throw divByZeroException();
		values.push(left_val / right_val);
		break;
	case '%':
	    if (right_val == 0) throw divByZeroException();
		values.push(left_val % right_val);
		break;
	}
}
