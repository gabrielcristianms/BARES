#include "parser.h"

/*!
 * Este é o **ponto de entrada**.
 * A partir daqui o parser tenta aceitar os símbolos não-terminais da
 * gramática, de maneira recursiva e descendente.
 *
 * \param e_ A string correspondente a uma expressão que o cliente quer analisar sintaticamente.
 * \return O resultado do parsing.
 */
Parser::ParserResult
Parser::parse( std::string e_ )
{
    // Os 4 comandos abaixo são executados a cada nova string a ser analisada.
    expr = e_;  // Guarda expressão passada.
    curr_symb = expr.begin(); // Iterador aponta p/ 1o caractere da string.
    curr_status = ParserResult( ParserResult::PARSER_OK ); // "Resetar" a msg de status p/ OK.
    token_list.clear(); // Limpar a lista de tokens para a próxima expressão.

    // Verificar se a string acabou sem conter uma expressão.
    skip_ws();
    if ( end_input() )
    {
        // Recebemos uma string vazia.
        curr_status = ParserResult( ParserResult::UNEXPECTED_END_OF_EXPRESSION,
                                    std::distance( expr.begin(), curr_symb ) );
    }
    else
    {
        // Se chegou aqui, então estamos esperando uma expressão bem formada.
        expression(); // Tenta aceitar uma expressão bem formada.
        
        // Se depois da expressão ter sido bem avaliada (sem erros), ainda existir algum
        // caractere (que não seja ws), então existem símbolo(s) estranho(s)...
        if ( curr_status.type == ParserResult::PARSER_OK )
        {
            // Saltamos qualquer espaço em branco remanescente da string.
            skip_ws();
            if ( not end_input() ) // Se não chegamos ao fim da string, é porque
            {                      // tem símbolo não-esperado na string!
                curr_status = ParserResult( ParserResult::EXTRANEOUS_SYMBOL,
                        std::distance( expr.begin(), curr_symb ) );
            }
        }
    }

    return curr_status; // Retorna para o cliente o resultado do parsing.
}

std::vector< Token >
Parser::get_tokens( void ) const
{
    return token_list;
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
        case '*':  return TS_TIMES;
        case '/':  return TS_DIVIDED;
        case '%':  return TS_MOD;
        case '^':  return TS_POWER;
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
 *
 * \sa end_input(), accept(), next_symb().
 */
void Parser::skip_ws( void )
{
    // Enquanto não esgotar a string e acharmos ws ou tab, vamos avançar...
    while( not end_input() and ( peek( TS_WS ) or peek( TS_TAB ) ) )
    {
        next_symbol(); // Avançar iterador 1 posição à frente.
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
        next_symbol(); // Consome caractere (símbolo) da expressão-string.
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

/*! \brief Parses a NTS <expression>.
 *
 *  This method parses part of the input expression looking for <expression>.
 *
 *  The production is:
 *  ```
 *  <expr> := <term>,{ ("+"|"-"|"*"|"/"|"%"|"^"),<term> };
 *  ```
 *  \sa parser(), term().
 */
void Parser::expression( void )
{
    auto begin_token = curr_symb;

    term(); // Procura aceitar um <term> dentro da expressõ.

    // Verificar se já não tem erro encontrado, ou seja, o <term> anterior foi mal-formado.
    if ( curr_status.type != ParserResult::PARSER_OK )
        return; // Não adiantar continuar processando, melhor voltar...

    // ===============================================================================
    // TOKENIZAÇÃO:
    // Este código separa o token e o insere na lista de tokens
    // -------------------------------------------------------------------------------
    // Recebe a string do token.
    std::string token_value;
    std::copy( begin_token, curr_symb, std::back_inserter( token_value ) );
    // Testar se o valor está dentro dos limites aceitáveis de um inteiro curto.

    if( outside_range( token_value ) )
    {
        // Gerar error de parser correspondente.
        curr_status = ParserResult( ParserResult::INTEGER_OUT_OF_RANGE,
                std::distance( expr.begin(), begin_token ) );
    }
    else{
        token_list.emplace_back( Token( token_value, Token::OPERAND ) );
    }
    // ===============================================================================


    // Se chegou aqui, quer dizer que o primeiro termo está ok.
    // devemos processar 0 ou mais <term>s
    while ( ( expect( TS_PLUS ) or expect( TS_MINUS ) or expect( TS_TIMES ) or expect( TS_DIVIDED) or expect( TS_MOD ) or expect( TS_POWER )
              /*or expect( TS_L_PAREN ) or expect( TS_R_PAREN )*/ ) 
              and curr_status.type == ParserResult::PARSER_OK ){
        // ===============================================================================
        // TOKENIZAÇÃO:
        // Este código separa o token e o insere na lista de tokens
        // -------------------------------------------------------------------------------
        // Salvar o token correspondente ao operador binário recém processado.
        std::advance( curr_symb, -1 ); // Voltei uma posição para apontar para o operador.
        token_value.clear(); // Limpar string para receber novo token.
        std::copy( curr_symb, curr_symb+1, // Uma string de apenas 1 caractere.
                   std::back_inserter( token_value ) ); // Copiando o operador binário.
        // Inserir token completo na lista de tokens.
        token_list.emplace_back( Token( token_value, Token::OPERATOR ) );
        // ===============================================================================

        // Avançar novamente o curr_symb.
        std::advance( curr_symb, +1 );

        skip_ws();
        if ( end_input() ) // Depois de saltar ws, não encontramos mais nada!! Erro!!
        {
            curr_status = ParserResult( ParserResult::MISSING_TERM,
                                        std::distance( expr.begin(), curr_symb ) );
        }
        else
        {
            // Iniciando um novo token.
            begin_token = curr_symb;
            // Situação normal, esperamos aceitar um novo termo.

            term();
            for( auto e : token_list )

            // ===============================================================================
            // TOKENIZAÇÃO:
            // Este código separa o token e o insere na lista de tokens
            // -------------------------------------------------------------------------------
            token_value.clear(); // Limpar string para receber novo token.
            // Copiar pedaço da expressão para a string temporária.
            std::copy( begin_token, curr_symb, std::back_inserter( token_value ) );
            // Testar se o valor está dentro dos limites aceitáveis de um inteiro curto.
            if( outside_range( token_value ) )
            {
                curr_status = ParserResult( ParserResult::INTEGER_OUT_OF_RANGE,
                        std::distance( expr.begin(), begin_token ) );
            }
            else
            {
                // Inserir o token bem formado na lista.
                token_list.emplace_back( Token( token_value, Token::OPERAND ) );
            }
            // ===============================================================================
        }
    }
}
/*
    <expr> := <term>,{ ("+"|"-"|"*"|"/"|"%"|"^"),<term> };
    <term> := "(",<expr>,")" | <integer>;
*/

/*! \brief Parses a NTS <term>.
 *
 *  This method parses part of the input expression looking for <term>.
 *  The production is:
 *  ```
 *  <term> := "(",<expr>,")" | <integer>;
 *  ```
 *  \sa expression(), integer().
 */
void Parser::term( void )
{   
    integer();    
}

/*! \brief Parses a NTS <integer>.
 *
 *  This method parses part of the input expression looking for <integer>.
 *
 *  The production is:
 *  <interget> := ["-"],<natural_number> | "0";
 *
 *
 *  \sa natural_number()
 */
void Parser::integer( void )
{
    // Podemos receber um zero...
    if ( expect(TS_ZERO) )
    {
        return;
    }
    else  //... ou então um ["-"],<natural_number>
    {
        accept( TS_MINUS ); // Pode ser que venha um '-'.
        natural_number();   // Aqui tentamos aceitar um <número_naural>.
    }
}

/*! \brief Parses o símbolo não-terminal <natural_number>.
 *
 *  This method parses part of the input expression looking for <natural_number>.
 *
 *  The production is:
 *  ```
 *  <natural_number> := <digit_excl_zero>,{<digit>};
 *  ```
 *  \sa integer()
 */
void Parser::natural_number( void )
{
    // Tentando aceitar <digit_excl_zero>,
    if ( accept( TS_NON_ZERO_DIGIT ) )
    {
        // ... que pode ser seguido de 0 ou mais <digit>s.
        while( accept( TS_NON_ZERO_DIGIT ) or accept( TS_ZERO ) ) 
        {
            /* empty */ ; // Nada a fazer, apenas "consumir" (avançar) o símbolo da string.
        }
    }
    else
    {
        // Opa, veio "algo" que não é um 'dígito_diferente_de_zero'!
        curr_status = ParserResult( ParserResult::ILL_FORMED_INTEGER,
                                    std::distance( expr.begin(), curr_symb ) );
    }
}


bool Parser::outside_range( std::string val_ ) const
{
    short int value;
    std::istringstream iss( val_ );

    iss >> value;
    return iss.fail();
}

