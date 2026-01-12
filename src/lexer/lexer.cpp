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
	std::vector<Token> Tokenize(const std::vector<std::string>& Source) {
		std::string CurrentToken = "";
		bool InQuote = false;
		int Line = 0;

		for(int i = 0; i < Source.size(); ++i) {
			Line = i;
			for(char s : Source[i]) {
				if(s == '"') {
					InQuote = !InQuote;

					if(InQuote) {
						if(!CurrentToken.empty()) {
							MakeNewToken(CurrentToken, Line);		
						}
						CurrentToken += s;
					}
					else {
						CurrentToken += s;
						MakeNewToken(CurrentToken, Line);
					}
					continue;
				}
				if(InQuote) {
					CurrentToken += s;
					continue;
				}

				if(s == '+' || s == '-' || s == '*' || s == '/' || s == ')' || s == '(') {
					if(!CurrentToken.empty()) {
						MakeNewToken(CurrentToken, Line);
					}
					CurrentToken += s;
					MakeNewToken(CurrentToken, Line);
					continue;
				}

				if(isspace(s)) {
					if(!CurrentToken.empty()) {
						MakeNewToken(CurrentToken, Line);
					}
					continue;
				}

				CurrentToken += s;
			}

			if(!CurrentToken.empty()) {
				MakeNewToken(CurrentToken, Line);
			}
		}

		Token EOFToken(" ", TokenType::EOF_TOKEN, CurrentPosition++, Line);
		Tokens.push_back(EOFToken);

		return Tokens;
	}
};
