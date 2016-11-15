#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib> // EXIT_SUCCESS

#include "parser.h"
#include "evaluator.h"

std::vector<std::string> expressions =
{
    "32767 - 32768 + 3",
    "5 + -32766",
    "5 + -32769",
    "12 + 3",
    "-3+-5+-6",
    "12 + 3     -3 + -34 ",
    "+12",
    " ( -   12  )",
    "( 120 - ( -2 ))",
    "5 + (-3 + 5(",
    "12  3",
    "12  # 3",
    "12  + 3 - ( 534 + 48  ) + ( ( 500 - 10) + 4 ) - 1234",
    "12",
    "-12",
    "12 +",
    "  ( 12 + ( 3  + 5555 ) )",
    "(  (  ( 123 + 22)))",
    "((( 400 + 23232 + 12 + 12 ))",
    "  -3 + 4 + ( 1 + -3 )",
    "1.3 * 4",
    "a + 4",
    ")3+4",
    "(31+   15)-",
    " ( ( 2 + 4 ) )",
    "( 1 + 2",
    "(-3+4)(10- 5)",
    "2 + 4)",
    "2 ) + 4",
    "  (",
    "       ",
    " 1 + 2 + 50 + -1253"
};

void print_msg( const Parser::ParserResult & result, std::string str )
{
    std::string error_indicator( str.size()+1, ' ');

    // Have we got a parsing error?
    error_indicator[result.at_col] = '^';
    switch ( result.type )
    {
        case Parser::ParserResult::UNEXPECTED_END_OF_EXPRESSION:
            std::cout << ">>> Unexpected end of input at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::ILL_FORMED_INTEGER:
            std::cout << ">>> Ill formed integer at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::MISSING_TERM:
            std::cout << ">>> Missing <term> at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::EXTRANEOUS_SYMBOL:
            std::cout << ">>> Extraneous symbol after valid expression found at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::MISSING_CLOSING_PARENTHESIS:
            std::cout << ">>> Missing closing \")\" at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::INTEGER_OUT_OF_RANGE:
            std::cout << ">>> Integer constant out of range beginning at column (" << result.at_col << ")!\n";
            break;
        default:
            std::cout << ">>> Unhandled error found!\n";
            break;
    }

    std::cout << "\"" << str << "\"\n";
    std::cout << " " << error_indicator << std::endl;
}

void print_msg_eval( const Evaluator::EvaluatorResult & result, std::string str )
{
    
    // Have we got a parsing error?
    switch ( result.type )
    {
        case Evaluator::EvaluatorResult::DIVISION_BY_ZERO:
            std::cout << ">>> Division by zero!\n";
            break;
        case Evaluator::EvaluatorResult::RESULT_OVERFLOW:
            std::cout << ">>> Numeric overflow error!\n";
            break;
        default:
            std::cout << ">>> Unhandled error found!\n";
            break;
    }

    std::cout << "\"" << str << "\"\n";
}


int main()
{
    Parser my_parser; // Instancia um parser.
    Evaluator my_evaluator; //Instancia um evaluator.
    // Tentar analisar cada expressão da lista.
    for( const auto & expr : expressions )
    {
        // Fazer o parsing desta expressão.
        auto result = my_parser.parse( expr );
        // Preparar cabeçalho da saida.
        std::cout << std::setfill('=') << std::setw(80) << "\n";
        std::cout << std::setfill(' ') << ">>> Parsing \"" << expr << "\"\n";
        // Se deu pau, imprimir a mensagem adequada.
        if ( result.type != Parser::ParserResult::PARSER_OK ){
            print_msg( result, expr );
            continue;
        }
        else
            std::cout << ">>> Expression SUCCESSFULLY parsed!\n";

        // Recuperar a lista de tokens.
        auto lista = my_parser.get_tokens();
        std::cout << ">>> Tokens: { ";
        std::copy( lista.begin(), lista.end(),
                std::ostream_iterator< Token >(std::cout, " ") );
        std::cout << "}\n";

        auto eval_result = my_evaluator.evaluate( lista );

        if( eval_result.type != Evaluator::EvaluatorResult::EVALUATOR_OK )
            print_msg_eval( eval_result, expr );
        else std::cout << ">>> Expression SUCCESSFULLY evaluated, the result is " << my_evaluator.get_result() << "\n\n";
    }

    std::cout << "\n>>> Normal exiting...\n";

    return EXIT_SUCCESS;
}
