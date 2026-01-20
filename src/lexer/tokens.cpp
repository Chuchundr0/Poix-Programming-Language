#pragma once

enum class TokenType {
	PRINT,

	PLUS,
	MINUS,
	DIV,
	MULT,

	NUMBER,
	STRING,

	LEFT_PAREN,
	RIGHT_PAREN,
	
	EOF_TOKEN,
	UNKNOWN
};

class Token {
public: 
	std::string Value;
	TokenType Type;
	int Position;
	int Line;

	Token(std::string NewValue, TokenType NewType, int NewPosition, int NewLine) {
		this->Value = NewValue;
		this->Type = NewType;
		this->Position = NewPosition;
		this->Line = NewLine;
	}
};
