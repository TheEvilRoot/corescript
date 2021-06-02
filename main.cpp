//
//  main.cpp
//  corescript
//
//  Created by Доктор Кларик on 5/14/21.
//

#include <iostream>

#include <vector>

#include <stdexcept>

#include <set>

#include <stack>

#include "token.h"

#include "tokens.h"

#include "expression.h"

#include "expressions.h"

// as a token delimiter could be recognized:
// - whitespace
// - tab
// - line seprator
// - end of file
// - OperatorToken

// structure:
// WORD_TOKEN|OPERATOR_TOKEN + WHITESPACE|OPERATOR_TOKEN + ...

inline std::string first(const std::string& string, size_t n) {
    if (string.size() <= n)
        return string;
    return string.substr(0, n);
}

inline std::string consumeUntil(const std::string& source, const std::string& chars) {
    if (source.empty())
        return std::string();
    if (chars.empty())
        return source;
    
    auto itBegin = std::begin(source);
    auto itEnd = std::begin(source);
    
    while (itEnd != std::end(source)) {
        char current = *itEnd;
        
        if (chars.find(current) != std::string::npos) {
            return std::string(itBegin, itEnd);
        }
        
        itEnd++;
    }
    
    return source;
}

struct Interpreter {
    
    struct Consumption {
        
        std::shared_ptr<Token> token;
        
        std::string rest;
        
        Consumption(std::string source): token{nullptr}, rest{source} { }
        
        Consumption(std::shared_ptr<Token> token, std::string rest): token{token}, rest{rest} { }
        
        Consumption(std::shared_ptr<Token> token, const std::string& source, size_t count): token{token}, rest{source.begin() + count, source.end()} { }
        
    };
    
    struct Exception : public std::exception {
        const std::string parseMessage;
        Exception(const std::string &message, const std::string &source):
        parseMessage{message + " on ..." + first(source, 12) + "..."} { }
        
        const char * what() const noexcept override {
            return parseMessage.c_str();
        }
    };
    
    struct UnknownTokenException : public std::exception {
        const std::shared_ptr<Token> token;
        const std::string message;
        
        UnknownTokenException(const std::shared_ptr<Token>& token):
        token{token},
        message{"Unexpected token " + token->str()} { }
        
        const char * what() const noexcept override {
            return message.c_str();
        }
    };
    
    enum ParseState { TOKEN, DELIM };
    
    std::string operators;
    std::string whitespaces;
    std::string quotes;
    std::set<std::string> keywords;
    
    Interpreter() { initTokens(); }
    
    Consumption next(std::string rest) {
        if (rest.empty())
            return rest; // implicit convertion
        
        auto start = rest[0];
        if (whitespaces.find(start) != std::string::npos)
            return Consumption(std::make_shared<WhitespaceToken>(start), rest, 1);
        
        if (quotes.find(start) != std::string::npos) {
            auto literal = consumeUntil(rest.substr(1), std::string(1, start));
            if (literal.size() == rest.size() - 1)
                throw Exception("Expected closing quote", rest);
            return Consumption(std::make_shared<LiteralToken>(literal), rest, 2 + literal.size());
        }
        
        if (operators.find(start) != std::string::npos)
            return Consumption(std::make_shared<OperatorToken>(start), rest, 1);
        
        auto string = consumeUntil(rest, operators + whitespaces);
        if (keywords.find(string) != keywords.end())
            return Consumption(std::make_shared<KeywordToken>(string), rest, string.size());
        
        return Consumption(std::make_shared<WordToken>(string), rest, string.size());
    }
    
    auto parse(const TokenString& tokens) {
        std::stack<std::shared_ptr<Expression>> stack;
        
        
    }
    
    auto tokenize(std::string source) {
        TokenString tokens;
        auto state = ParseState::TOKEN;
        do {
            auto result = next(source);
            if (!result.token->isValid()) {
                throw Exception("Invalid token", source);
            }
            
            if (state == ParseState::TOKEN) {
                if (!result.token->isWhitespace() && !result.token->isOperator()) {
                    state = ParseState::DELIM;
                }
                
                if (!result.token->isWhitespace())
                    tokens.push_back(result.token);
                source = result.rest;
            } else if (state == DELIM) {
                if (result.token->isWhitespace() || result.token->isOperator()) {
                    if (!result.token->isWhitespace())
                        tokens.push_back(result.token);
                    state = ParseState::TOKEN;
                    source = result.rest;
                } else {
                    throw Exception("Unexpected token", source);
                }
            } else {
                throw Exception("Unexpected state", source);
            }
        } while (!source.empty());
        return tokens;
    }
    
    void initTokens() {
        op('+'); whitespace(' ');
        op('-'); whitespace('\t');
        op('*'); whitespace('\r');
        op('/'); whitespace('\n');
        op('('); keyword("var");
        op(')'); keyword("if");
        op('['); keyword("else");
        op(']'); keyword("then");
        op('{'); keyword("endif");
        op('}'); keyword("var");
        op('<'); quote('"');
        op('>'); quote('\'');
        op('.');
        op(',');
        op('!');
        op('@');
        op('#');
        op('$');
        op('%');
        op('^');
        op('&');
        op(';');
        op('=');
        op('~');
    }
    
    void op(char op) {
        operators += op;
    }
    
    void whitespace(char ws) {
        whitespaces += ws;
    }
    
    void keyword(const std::string& kw) {
        keywords.insert(kw);
    }
            
    void quote(char c) {
        quotes += c;
    }
};

int main(int argc, const char * argv[]) {
    auto string = "var hello = \"'asd 123' asd\"\nvar c= euqwioe";
    Interpreter interpreter;
    auto tokens = interpreter.tokenize(string);
    for (const auto& token : tokens) {
        std::cout << *token << "\n";
    }
    return 0;
}
