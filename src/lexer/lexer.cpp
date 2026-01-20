#pragma once

#include "tokens.cpp"

class Lexer {
private:
	int CurrentPosition = 0;
	std::vector<Token> Tokens;

	void MakeNewToken(std::string& Lexeme, int Line) {
		if(Lexeme.empty()) return;

		TokenType NewType;
		if(Lexeme == "console") NewType = TokenType::PRINT;
		else if(Lexeme == "+") NewType = TokenType::PLUS;
		else if(Lexeme == "-") NewType = TokenType::MINUS;
		else if(Lexeme == "*") NewType = TokenType::MULT;
		else if(Lexeme == "/") NewType = TokenType::DIV;
		else if(Lexeme == "(") NewType = TokenType::LEFT_PAREN;
		else if(Lexeme == ")") NewType = TokenType::RIGHT_PAREN;
		else {
			if(Lexeme[0] == '"' && Lexeme.back() == '"') {
				NewType = TokenType::STRING;
				Lexeme = Lexeme.substr(1, Lexeme.length() - 2);	
			}
			else {
				try {
					int TestNumber = stoi(Lexeme);
					NewType = TokenType::NUMBER;
				}
				catch(...) {
					std::cerr << "ERROR: Unknown token: '" << Lexeme << "'" << std::endl;
					std::cerr << "Line: " << Line << std::endl;
					exit(0);
				}
			}
		}

		Token NewToken(Lexeme, NewType, CurrentPosition++, Line);
		Tokens.push_back(NewToken);
		
		Lexeme.clear();
	}

public:
	std::vector<Token> Tokenize(const std::string Source) {
		std::string CurrentToken = "";
		int Line = 0;

		for(size_t pos = 0; pos < Source.size(); ++pos) {
			char sym = Source[pos];

			if(sym == '\n') {
				Line++;
			}

			if(sym == '#') {
				if(!CurrentToken.empty()) {
					MakeNewToken(CurrentToken, Line);
				}
				
				++pos;
				for(; pos < Source.size(); ++pos) {
					sym = Source[pos];

					if(sym == '\n') {
						Line++;
						break;
					}
				}

				continue;
			}
	
			if(sym == '"') {
				if(!CurrentToken.empty()) {
					MakeNewToken(CurrentToken, Line);
				}

				CurrentToken += sym;
				++pos;
				for(; pos < Source.size(); pos++) {
					sym = Source[pos];

					if(sym == '\n') {
						Line++;
					}

					if(sym == '"') {
						CurrentToken += sym;
						break;
					}
					else {
						CurrentToken += sym;
					}
				}

				continue;
			}

			if(sym == '+' || sym == '-' || sym == '*' || sym == '/' || sym == ')' || sym == '(') {
				if(!CurrentToken.empty()) {
 					MakeNewToken(CurrentToken, Line);
				}
				CurrentToken += sym;
				MakeNewToken(CurrentToken, Line);
 				continue;
			}

			if(isspace(sym) || sym == '\n') {
 				if(!CurrentToken.empty()) {
 					MakeNewToken(CurrentToken, Line);
 				}
				continue;
 			}

			CurrentToken += sym;
 		}

		if(!CurrentToken.empty()) {
			MakeNewToken(CurrentToken, Line);
		}

		Token EOFToken(" ", TokenType::EOF_TOKEN, CurrentPosition++, Line);
		Tokens.push_back(EOFToken);

		return Tokens;
	}
};
