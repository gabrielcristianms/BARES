#include <iostream>
/*!
 * Implements a recursive descendent parser for a EBNF grammar.
 *
 *   <expr>            := <term>,{ "+",<term> };
 *   <term>            := <integer>;
 *   <integer>         := 0 | ["-"],<natural_number>;
 *   <natural_number>  := <digit_excl_zero>,{<digit>};
 *   <digit_excl_zero> := "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
 *   <digit>           := "0"| <digit_excl_zero>;
 */
class Parser
{
    public:
        /*! Classe interna representado o resultado do parsing.
         *  Ela deve ser pública para que o 'cliente' possa acessar o resultado.
         *  Por ser uma classe muito simples, vamos deixar seus campos públicos,
         *  para evitar o _overhead_ de chamar métodos _getter_ e _setter_.
         */
        class ParserResult
        {
            private:
                typedef size_t size_type; //<! Tipo de dado representando a posição (coluna) do erro dentro da expressão.

            public:
                /*! Aqui é uma simple tabela de erros. */
                enum code_t {
                    PARSER_OK = 0,
                    UNEXPECTED_END_OF_EXPRESSION,
                    ILL_FORMED_INTEGER,
                    MISSING_TERM,
                    EXTRANEOUS_SYMBOL,
                    MISSING_CLOSING_PARENTHESIS,
                    INTEGER_OUT_OF_RANGE,
                    N_CODES   // Indicates the # of codes.
                };

                // Membros públicos da classe. Não precisamos de métodos.
                code_t type; //<! Guarda o tipo de erro.
                size_type at_col; //<! Guarda a coluna do erro.

                // Por padrão, o resultado é positivo.
                explicit ParserResult( code_t type_=PARSER_OK , size_type col_=0u )
                    : type{ type_ }
                    , at_col{ col_ }
                { /* empty */ }
        };

        /// Recebe uma expressão, realiza o parsing e retorna o resultado.
        ParserResult parse( std::string e_ );

        /// Constutor default.
        Parser() = default;
        ~Parser() = default;
        /// Desligar cópia e atribuição.
        Parser( const Parser & ) = delete;  // Construtor cópia.
        Parser & operator=( const Parser & ) = delete; // Atribuição.

    private:

        // Tabela de símbolos terminais.
        // Cada símbolo (caractere terminal) está associado a um código.
        enum terminal_symbol_t{  // The symbols:-
            TS_L_PAREN=0,       //<! "("
            TS_R_PAREN,	        //<! ")"
            TS_PLUS,	        //<! "+"
            TS_MINUS,	        //<! "-"
            TS_ZERO,            //<! "0"
            TS_NON_ZERO_DIGIT,  //<! "1"->"9"
            TS_WS,              //<! white-space
            TS_TAB,             //<! tab
            TS_EOS,		//<! End-of-strin, or '\0'
            TS_INVALID,	        //<! invalid token
            //
            N_SYMBOLS            //<! total of existing symbols.
        };

        // Membros privados do parser.
        std::string expr;                //<! Expressão para ser avaliada.
        std::string::iterator curr_symb; //<! Posição atualmente processada dentro da expressão.
        ParserResult curr_status;        //<! Guarda o estado atual da operação de parsing.

        /// Converte de caractere para código do símbolo terminal.
        terminal_symbol_t lexer( char ) const;

        // Métodos de suporte
        bool peek( terminal_symbol_t s_ ) const; // Espia o caractere atual.
        bool accept( terminal_symbol_t s_ ); // Tenta aceita o símbolo indicado.
        void next_symbol( void ); // Avança o iterador para o próximo símbolo.
        bool expect( terminal_symbol_t ); // Pula ws e tenta aceitar o primeiro caractere que não seja ws.
        void skip_ws( void ); // Pula os caracteres ws (espaço em branco ou tab).
        bool end_input( void ) const; // Verifica se chegamos ao fim da expressão.

        // Aqui vem os métodos correspondentes às regras de produção da gramática.

};
