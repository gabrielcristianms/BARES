# BARES
Esta versão do BARES não está com a gramática totalmente completa. No caso, ela não é capaz de tratar parêntesis, mas realiza cálculos com todas as operações suportadas e faz o tratamento de erro sintático e avaliativo. Para leitura e impressão dos resultados foi utilizado std::cin e std::cout, o que resulta num detalhe um pouco diferente na hora da execução do programa.
Este projeto não está com a divisão em pastas. Comentários no formato doxygen foram feitos, mas não sou capaz de gerar os arquivos na minha máquina pessoal.

Para compilar execute
	g++ -Wall -std=c++11 token.h parser.h parser.cpp evaluator.h evaluator.cpp driver_parser.cpp -o bares

Na hora de executar o binário, faça-o da seguinte maneira
	./bares <ArquivoEntrada.txt >ArquivoSaida.txt
