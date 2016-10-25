
class Parser{
	public:
		class ParserResult{
			private:
				typedef size_t size_type;

			public:
				enum code_t {
					PARSER_OK = 0,
					UNEXPECTED_END_OF_EXPRESSION,
					ILL_FORMED_INTEGER,
					MISSING_TERM,
					EXTRANEOUS_SYMBOL,
					MISSING_CLOSING_PARENTHESIS,
					INTEGER_OUT_OF_RANGE
					N_CODES //Representa a quantidade de códigos
				};

				code_t type; // Tipo de erro
				size_type at_col; // Guarda a coluna

				explicit ParserResult( code_t type_ = PARSER_OK, size_type col_ = 0u )
				: type { type_ }
				, at_col { col_ }
				{/* empty */}
		};

		Parser() = default;
		ParserResult parse( std::string );

		std::vector<Token> get_tokens();

	private:
		// table of terminal symbols, simple grammar
		enum terminal_symbol_t {
			TS_L_PAREN,
			TS_R_PAREN,
			TS_PLUS,
			TS_MINUS,
			TS_ZERO,
			TS_NON_ZERO_DIGIT,
			TS_WS,
			TS_TAB,
			TS_INVALID,
			N_SYMBOLS
		};

		std::string exp;
		std::string::iterator curr_symb;
		ParserResult curr_status;
		std::vector<Token> m_tokens; // Criar a classe token para funcionar

		terminal_symbol_t lexer( char ) const;
		bool peek( terminal_symbol_t ) const;
		void skip_ws( void );
		bool end_input( void ) const;
		bool accept( terminal_symbol_t );
		bool expect( terminal_symbol_t );
		void next_symbol( void ); // avança o ponteiro curr_status, ou seja, para o pŕoximo simbolo.
};