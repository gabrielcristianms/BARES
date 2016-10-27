#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>

/// Estrutura que representa um token, composto por seu valor (string) e seu tipo (enum).
struct Token
{
    public:
        enum token_t
        {
            OPERAND = 0, // Basicamente números.
            OPERATOR,    // "+", "-", "*", "/", "%", "^".
            SCOPE        // "(" ou ")"
        };

        std::string value; //<! Valor do token.
        token_t type;      //<! Tipo de token: operando, operador, escopo.

        /// Construtor default.
        explicit Token( std::string v_="", token_t t_ = OPERAND )
            : value( v_ )
            , type( t_ )
        {/* empty */}
};

#endif
