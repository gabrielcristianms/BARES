#include <iostream>
#include <iomanip>
#include <vector>

#include "parser.h"

std::vector<std::string> expressions =
{
    "+12",
    " ( -   12  )",
    "( 120 - ( -2 ))",
    "5 + (-3 + 5(",
    "-3+-5+-6",
    "12 + 3",
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
    "       "
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
        case Parser::ParserResult::PARSER_OK:
            std::cout << ">>> Expression SUCCESSFULLY parsed!\n";
            break;
        default:
            std::cout << ">>> Unhandled error found!\n";
            break;
    }

    std::cout << "\"" << str << "\"\n";
    std::cout << " " << error_indicator << std::endl;
}


int main()
{
    Parser my_parser; // Instancia um parser.
    // Tentar analisar cada expressão da lista.
    for( const auto & expr : expressions )
    {
        // Fazer o parsing desta expressão.
        auto result = my_parser.parse( expr );
        // Preparar cabeçalho da saida.
        std::cout << std::setfill('=') << std::setw(80) << "\n";
        std::cout << std::setfill(' ') << ">>> Parsing \"" << expr << "\"\n";
        // Se deu pau, imprimir a mensagem adequada.
        if ( result.type != Parser::ParserResult::PARSER_OK )
            print_msg( result, expr );
        else
            std::cout << ">>> Expression SUCCESSFULLY parsed!\n";

    }

    std::cout << "\n>>> Normal exiting...\n";

    return EXIT_SUCCESS;
}
