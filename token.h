//
//  token.h
//  corescript
//
//  Created by Доктор Кларик on 5/15/21.
//

#ifndef token_h
#define token_h

#include <string>

#include <vector>

enum TokenType {
    OPERATOR,
    WORD,
    KEYWORD,
    WHITESPACE,
    LITERAL
};

struct Token {
    
    TokenType tokenType;
    
    Token(TokenType type): tokenType{type} {}
    
    virtual bool isValid() const { return false; }
    
    virtual bool isOperator() const { return false; }
    
    virtual bool isWhitespace() const { return false; }
    
    virtual bool isKeyword() const { return false; };
    
    virtual std::string str() const { return "Token[]"; }
    
    friend std::ostream& operator<<(std::ostream& o, const Token& t) {
        return (o << t.str());
    }
    
};

typedef std::vector<std::shared_ptr<Token> > TokenString;

#endif /* token_h */
