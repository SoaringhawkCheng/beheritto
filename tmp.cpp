#include "ast.h"

class ASTNode{
public:
    ASTNode();
    int line;
};

class Expr:public ASTNode{
public:
    Expr();
    virtual type *valid();
};

class ExprOpUnary{
public:
    ExprOpUnary(const Expr &e);
    Expr *expression;
}

class ExprOp
