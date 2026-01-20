#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>

#include "lexer/lexer.cpp"
#include "parser/parser.cpp"

void ShowTree(const std::unique_ptr<Node>& node, int level = 0) {
    if (!node) return;

    for (int i = 0; i < level; i++) {
        std::cout << "  ";
    }

    if (auto num = dynamic_cast<NumberNode*>(node.get())) {
        std::cout << "Num: " << num->Value << std::endl;
    }
    else if (auto str = dynamic_cast<StringNode*>(node.get())) {
        std::cout << "Str: \"" << str->Value << "\"" << std::endl;
    }
    else if (auto bin = dynamic_cast<BinaryOpNode*>(node.get())) {
        std::string op;
        if (bin->Operation == TokenType::PLUS) op = "+";
        else if (bin->Operation == TokenType::MINUS) op = "-";
        else if (bin->Operation == TokenType::MULT) op = "*";
        else if (bin->Operation == TokenType::DIV) op = "/";
        else op = "Unknown operation";

        std::cout << "Operation: " << op << std::endl;

        ShowTree(bin->Left, level + 1);
        ShowTree(bin->Right, level + 1);
    }
    else if(auto prn = dynamic_cast<PrintNode*>(node.get())) {
        std::cout << "Command: Print" << std::endl;
        ShowTree(prn->Expr, level + 1);
    }
}

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
		default: return "UNKNOWN";
	};
};

void Print(std::vector<Token> Source) {
	for(int i = 0; i < Source.size(); ++i) {
		std::cout << "Value: " << Source[i].Value << ", ";
		std::cout << "Type: " << TokenTypeToString(Source[i].Type) << ", ";
		std::cout << "Pos: " << Source[i].Position << ", ";
		std::cout << "Line: " << Source[i].Line << ";\n";
	}

	std::cout << std::endl;
}

int main(int argc, char* argv[]) {
	if(argc < 2) {
		std::cerr << "ERROR: Not enough files to compile: Exit." << std::endl;
		return 1;
	}

	std::ifstream SourceFile(argv[1]);
	if(!SourceFile.is_open()) {
		std::cerr << "ERROR: File read error: Exit." << std::endl;
		return 1;
	}

	std::string Line;
	std::string Source;
	while(getline(SourceFile, Line)) {
		Source += Line;
		Source += '\n';
	}

	Lexer NewLexer;
	std::vector<Token> Tokens = NewLexer.Tokenize(Source);

	Print(Tokens);

	Parser Parse;
	std::vector<std::unique_ptr<Node>> AST = Parse.ParseTokens(Tokens);

	for(const auto& Node : AST) {
    	ShowTree(Node);
    	std::cout << std::endl;
	}

	return 0;
}
