//
//  tokens.h
//  corescript
//
//  Created by Доктор Кларик on 5/15/21.
//

#ifndef tokens_h
#define tokens_h

#include "token.h"

struct OperatorToken : public Token {
    
    char character;
    
    OperatorToken(char c): Token{OPERATOR}, character{c} { }
    
    bool isValid() const override { return true; }
    
    bool isOperator() const override { return true; }
    
    std::string str() const override { return "Operator[" + std::string(1, character) + "]"; }
    
};

struct WordToken : public Token {
    
    std::string word;
    
    WordToken(const std::string& rest): Token{WORD}, word{rest} { }
    
    bool isValid() const override { return true; }
    
    std::string str() const override { return "Word[" + word + "]"; }
};

struct KeywordToken : public WordToken {
    
    KeywordToken(const std::string& rest): WordToken(rest) { tokenType = KEYWORD; }
 
    bool isKeyword() const override { return true; };
    
    std::string str() const override { return "Keyword[" + word + "]"; }
    
};

struct WhitespaceToken : public Token {
    
    WhitespaceToken(char c): Token{WHITESPACE} { }
    
    bool isValid() const override { return true; }
    
    bool isWhitespace() const override { return true; }
    
    std::string str() const override { return "Whitespace[]"; }
    
};

struct LiteralToken : public Token {
    
    std::string value;
    
    LiteralToken(const std::string& string): Token{LITERAL}, value{string} { }
    
    bool isValid() const override { return true; }
    
    std::string str() const override { return "Literal[" + value + "]"; }
    
};

#endif /* tokens_h */
