/*!
 * Infix to postfix conversion in C++
 * Input Postfix expression must be in a desired format.
 * Operands and operator, **both must be single character**.
 * Only '+', '-', '*', '/', and '^' (for exponentiation) operators are expected;
 * Any other character is just ignored.
 */

#include <iostream>  // cout, endl
#include <stack>     // stack
#include <string>    // string
#include <cassert>   // assert
#include <cmath>     // pow

/// Converts a expression in infix notation to a corresponding profix representation.
std::string infix_to_postfix( std::string );

/// Checks whether the first operator has higher precedence over the second one.
bool has_higher_precedence(char , char);

/// Checks whether a token is operator symbol or not. 
bool is_operator(char);

/// Checks whether a token is a character is alphanumeric chanaracter (letter or numeric digit) or not. 
bool is_operand(char);

/// Checks whether the token is an opening scope symbol
bool is_opening_scope(char);

/// Checks whether the token is a closing scope symbol
bool is_closing_scope(char);

/// Returns the precedence of the operator.
int get_operator_precedence(char);


int main()
{
    std::string expression = "4/(5^2)+(6^2^3)";
    //std::string expression = "A+(B*C-(D/E^F)+G)*H";
    auto postfix = infix_to_postfix(expression);
    std::cout << ">>> Input (infix)    = " << expression << "\n";
    std::cout << ">>> Output (postfix) = " << postfix << "\n";

    //auto result = evaluate_postfix( postfix );
    //std::cout << ">>> Result is: " << result << std::endl;

    std::cout << "\n>>> Normal exiting...\n";

    return EXIT_SUCCESS;
}

