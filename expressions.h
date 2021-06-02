//
//  expressions.h
//  corescript
//
//  Created by Доктор Кларик on 1.06.21.
//

#ifndef expressions_h
#define expressions_h

#include <variant>

#include <optional>

#include "expression.h"

struct Operator : public Expression {
  
    std::shared_ptr<OperatorToken> op_tok;
    
    Operator(const std::shared_ptr<OperatorToken>& tok): op_tok{tok} {}
    
};


struct Name : public Expression {
    
    std::shared_ptr<WordToken> word_tok;
    
    Name(const std::shared_ptr<WordToken>& tok): word_tok{tok} {}
    
};

struct Literal : public Expression {
    
    std::shared_ptr<LiteralToken> lit_tok;
    
    Literal(const std::shared_ptr<LiteralToken>& tok): lit_tok{tok} {}
    
};

struct ValExpr : public Expression {
  
    typedef std::variant<std::shared_ptr<Literal>, std::shared_ptr<Name>, std::shared_ptr<Expr>> ValExprValue;
    
    ValExprValue value;
    
    ValExpr(const std::shared_ptr<Literal>& l): value{l} {}
    
    ValExpr(const std::shared_ptr<Name>& n): value{n} {}
    
    ValExpr(const std::shared_ptr<Expr>& e): value{e} {}
    
    
};

struct Expr : public Expression {
  
    std::shared_ptr<ValExpr> first;
    std::optional<std::pair<std::shared_ptr<Operator>, std::shared_ptr<ValExpr>>> rest;
    
    Expr(const std::shared_ptr<ValExpr>& f): first{f}, rest{} {}
    
    Expr(const std::shared_ptr<ValExpr>& f, const std::shared_ptr<Operator>& o, const std::shared_ptr<ValExpr>& v):
    first{f}, rest{std::make_pair(o, v)} { }
    
};

#endif /* expressions_h */
