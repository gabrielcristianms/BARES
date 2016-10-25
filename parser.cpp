#include "parser.h"

Parser::ParserResult 
Parser::parse( std::string e_ )
{
    expr = e_; // Guarda expressão passada.
    curr_symb = expr.begin(); // Caractere inicial.

    //expression();

    return curr_status; // Retorna o resultado do parsing.
}


/*!
 *  \brief Verifica se o caractere atual (curr_symb) corresponde ao código esperado.
 *
 *  Porém, o método **não consome** o caractere, ou seja, o iterador `curr_symb` não avança.
 *  Basicamente este método funciona como uma tabela:
 *
 *  ----------+-------
 *  Caractere | Código
 *  ----------+-------
 *  "("       |  0
 *  ")"       |  1
 *  "+"       |  2
 *  "-"       |  3
 *  ...
 *
 *  \param c_ O caractere do símbolo que desejamos converter para o código (enum) do símbolo.
 *  \return O código (enum) do caractere indicado por parâmetro.
 */
Parser::terminal_symbol_t  Parser::lexer( char c_ ) const
{
    switch( c_ )
    {
        case '(':  return TS_L_PAREN;
        case '+':  return TS_PLUS;
        case '-':  return TS_MINUS;
        case ')':  return TS_R_PAREN;
        case ' ':  return TS_WS;
        case   9:  return TS_TAB;
        case '0':  return TS_ZERO;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':  return TS_NON_ZERO_DIGIT;
        case   0:  return TS_EOS; // end of string: the $ terminal symbol
        default :  return TS_INVALID;
    }
}

/*!
 * \brief Verifica se o caractere atual (`*curr_symb`) corresponde ao símbolo passado.
 * \param s_ Código (enum) do símbolo desejado.
 * \return `true`, se o símbolo atual for o desejado, `false`, caso contrário.
 * \sa accept(), expect(), next_symb(), lexer().
 */
bool Parser::peek( terminal_symbol_t s_ ) const
{
    // Caractere atual?
    return lexer( *curr_symb ) == s_ ;

}

/*!
 * \brief Avança o iterador que aponta para o símbolo (caractere) atual na expressão.
 * \sa skip_ws(), accept(), expect().
 */
void Parser::next_symbol( void )
{
    // Avançar iterador
    curr_symb++;
    // poderia ser:
    // std::advance( curr_symb, 1 );
}


/*!
 * \brief Verifica se chegamos ao fim da expressão.
 * \return `true` se chegamos ao fim da expressão, `false` caso contrário.
 * \sa next_symb(), skip_ws().
 */
bool Parser::end_input( void ) const
{
    // Verificar se o iterador chegou ao fim da string.
    return curr_symb == expr.end();
}

/*!
 * \brief Salta caracteres em branco ou tabs encontrados na expressão.
 * O método deixa o iterador `curr_symb` apontando para o primeiro próximo
 * caractere da expressão que não seja ws (espaço em branco ou tab).
 *
 * \note 
 * O método pode fazer o ponteiro chegar ao fim da expressão, se ela possuir
 * apenas ws até o final.
 * \sa end_input(), accept(), next_symb().
 */
void Parser::skip_ws( void )
{
    while( not end_input() and ( peek( TS_WS ) or peek( TS_TAB ) ) )
    {
        next_symbol();
    }
}

/*!
 * \brief Tenta "aceitar" o símbolo indicado.
 * O método tenta verificar se o símbolo indicado bate com o símbolo atual da expressão.
 * Se bate, o símbolo é consumido (i.e. o iterador avança) e o método retorna `true'.
 * Se falhar, o iterador fica onde está e o método retorna `false`.
 * \param s_ O cósigo do símbolo que desejamos aceitar.
 * \return `true` se o código for aceito, `false` caso contrário.
 */
bool Parser::accept( terminal_symbol_t s_ )
{
    // Tentando consumir (da expressão) o símbolo indicado (pelo processo de parsing).
    if ( not end_input() and peek( s_ ) )
    {
        next_symbol();
        return true;
    }
    return false;
}

/*!
 * \brief Salta ws e tenta "aceitar" o símbolo indicado.
 * O método é muito similar ao `Parser::accept()`, com a diferença que ele tenta
 * saltar ws que existam até chegar em um símbolo "processável".
 * Então o método tenta aceitar tal símbolo.
 * \param s_ O cósigo do símbolo que desejamos aceitar.
 * \return `true` se o código for aceito, `false` caso contrário.
 */
bool Parser::expect( terminal_symbol_t s_ )
{
    // (1) Saltar espaços em branco
    skip_ws();
    // (2) tentar aceitar o símbolo.
    return accept( s_ );
}
