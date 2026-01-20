#pragma once

class Node {
public:
    virtual ~Node() = default;
};

class NumberNode : public Node {
public:
    int Value;

    NumberNode(int NewValue) : Value(NewValue) {}
};

class StringNode : public Node {
public:
    std::string Value;

    StringNode(const std::string& NewValue) : Value(NewValue) {}
};

class BinaryOpNode : public Node {
public:
    TokenType Operation;
    std::unique_ptr<Node> Left;
    std::unique_ptr<Node> Right;

    BinaryOpNode(TokenType NewOp, std::unique_ptr<Node> NewLeft, std::unique_ptr<Node> NewRight) :
        Operation(NewOp), Left(std::move(NewLeft)), Right(std::move(NewRight)) {}
};

class PrintNode : public Node {
public:
    std::unique_ptr<Node> Expr;

    PrintNode(std::unique_ptr<Node> NewExpr) : Expr(std::move(NewExpr)) {}
};

class Parser {
private:
    std::vector<Token> Tokens;
    size_t Pos = 0;

    bool Check(TokenType CheckType) {
        return Pos < Tokens.size() && Tokens[Pos].Type == CheckType;
    }

    bool Match(TokenType Type) {
        if(Check(Type)) {
            ++Pos;
            return true;
        }
        return false;
    }

    void Expect(TokenType Type, const std::string& error) {
        if (!Match(Type)) {
            throw std::runtime_error(error + "\nLine: " + std::to_string(Tokens[Pos].Line));
        }
    }

    std::unique_ptr<Node> ParseBase() {
        if(Match(TokenType::NUMBER)) {
            try {
                int value = std::stoi(Tokens[Pos-1].Value);
                return std::make_unique<NumberNode>(value);
            } catch (...) {
                throw std::runtime_error("Invalid number\nLine: " + std::to_string(Tokens[Pos-1].Line));
            }
        }

        if(Match(TokenType::STRING)) {
            return std::make_unique<StringNode>(Tokens[Pos-1].Value);
        }

        if(Match(TokenType::LEFT_PAREN)) {
            auto node = ParseExpression();
            Expect(TokenType::RIGHT_PAREN, "Parenthesis Closing Error");
            return node;
        }

        throw std::runtime_error("Expression Processing Assignment Error\nLine: " + std::to_string(Tokens[Pos].Line));
    }
    
    std::unique_ptr<Node> ParseMultAndDiv() {
        auto left = ParseBase();

        while(Match(TokenType::MULT) || Match(TokenType::DIV)) {
            TokenType operation = Tokens[Pos-1].Type;
            auto right = ParseBase();

            auto newNode = std::make_unique<BinaryOpNode>(
                operation, 
                std::move(left), 
                std::move(right)
            );
            
            left = std::move(newNode);
        }

        return left;
    }

    std::unique_ptr<Node> ParsePlusAndMinus() {
        auto left = ParseMultAndDiv();

        while(Match(TokenType::PLUS) || Match(TokenType::MINUS)) {
            TokenType operation = Tokens[Pos-1].Type;
            auto right = ParseMultAndDiv();

            auto newNode = std::make_unique<BinaryOpNode>(
                operation, 
                std::move(left), 
                std::move(right)
            );
            
            left = std::move(newNode);
        }

        return left;
    }

    std::unique_ptr<Node> ParseExpression() {
        return ParsePlusAndMinus();
    }

    std::unique_ptr<Node> ParseCommands() {
        if(Match(TokenType::PRINT)) {
            auto expr = ParseExpression();
            return std::make_unique<PrintNode>(std::move(expr));
        }

        throw std::runtime_error("Error using the function\nLine: " + 
            std::to_string(Tokens[Pos].Line));
    }

public:
    std::vector<std::unique_ptr<Node>> ParseTokens(const std::vector<Token>& InputTokens) {
        Tokens = InputTokens;
        Pos = 0;
    
        std::vector<std::unique_ptr<Node>> AST;
        while(Pos < Tokens.size() && Tokens[Pos].Type != TokenType::EOF_TOKEN) {
            AST.push_back(ParseCommands());
        }

    	return AST;
	}
};
