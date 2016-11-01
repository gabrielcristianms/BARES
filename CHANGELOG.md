# Change Log
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/) 
and this project adheres to [Semantic Versioning](http://semver.org/).

## [Unreleased]
### Added
* Oct-25th:
    - Created class [`Parser`](parser.h), to parse an expression.
    - Created class [`ParserResult`](parser.h) (inner struct of [`Parser`](parser.h)), to receive the error code and location, or the success code.
    - Created a simple driver that tries to parse a series of expressions.
    - Created basic processing methods for the class [`Parser`](parser.cpp).
* Oct-27th:
    - Non-terminal methods implemented:
        - expression()
        - term()
        - integer()
        - natural_number()
    - Tested parser with several examples
    - Create the struct [`Token`](token.h) to represent a Bares' token. The objective is to create a list of tokens during the parsing process. In turn, this list shall be the input of the [infix-to-postfix algorithm](http://projetos.imd.ufrn.br/LP1_20162/bares).
* Nov-1st:
    - Implemented the "tokenization" process, that creates a list of [`Token`](token.h)
    - Added method that returns a `std::vector` of tokens.
    - New parser error added: _integer constant out or range_
