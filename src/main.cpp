#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "lexer/lexer.cpp"

std::string TokenTypeToString(TokenType type) {
    switch(type) {
        case TokenType::PRINT: return "PRINT";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULT: return "MULT";
        case TokenType::DIV: return "DIV";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::STRING: return "STRING";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::EOF_TOKEN: return "EOF_TOKEN";
    };
}

void Print(std::vector<Token> Source) {
	for(int i = 0; i < Source.size(); ++i) {
		std::cout << "Value: " << Source[i].Value << ", ";
		std::cout << "Type: " << TokenTypeToString(Source[i].Type) << ", ";
		std::cout << "Pos: " << Source[i].Position << ", ";
		std::cout << "Line: " << Source[i].Line << ";\n";
	}
}

int main(int argc, char* argv[]) {
	if(argc < 1) {
		std::cerr << "ERROR: Not enough files to compile: Exit." << std::endl;
		return 1;
	}

	std::ifstream SourceFile(argv[1]);
	if(!SourceFile.is_open()) {
		std::cerr << "ERROR: File read error: Exit." << std::endl;
		return 1;
	}

	std::string Line;
	std::vector<std::string> Source;
	while(getline(SourceFile, Line)) {
		Source.push_back(Line);
	}

	Lexer NewLexer;
	std::vector<Token> Tokens = NewLexer.Tokenize(Source);

	Print(Tokens);

	return 0;
}
