#ifndef ast_h
#define ast_h

#include <iostream>
#include <vector>
#include <unordered_map>

#include "astnode.h"

class ASTNode{
public:
    ASTNode();
    virtual string ToString()=0;
    int lint;
};

class Expr:public ASTNode{
public:
    Expr();

};

class ExprOpUnary:public Expr{
public:
    ExprOpUnary();
};

class ExprInvert:public ExprOpUnary{
public:
    ExprInvert();

};
class ExprNegate:public ExprOpUnary{
public:
    ExprNegate();
};

class ExprOpBinary:public Expr{
public:
    ExprOpBinary(Expr *lhs,Expr *rhs);
    Expr *lhs;
    Expr *rhs;
};

class ExprCompare:public ExprOpBinary{
public:
    ExprCompare(Expr *lhs,Expr *rhs);
    Expr *lhs;
    Expr *rhs;
};

class ExprArithmetic:public ExprOpBinary{
public:
    ExprArithmetic(Expr *lhs,Expr *rhs);
    Expr *lhs;
    Expr *rhs;
};

class ExprLogic:public ExprOpBinary{
public:
    ExprLogic(Expr *lhs,Expr *rhs);
    Expr *lhs;
    Expr *rhs;
}

class ExprOpMember:public ExprOpBinary{
public:
    ExprOpMember();
};

class ExprLValue:public Expr{
public:
    ExprLValue(const string &name);
};

class ExprElement:public ExprLValue{
public:
    ExprElement(const string &name);
    ExprRValue *element;
};

class ExprList:public ExprLValue{
public:
    ExprList(const string &name);
    vector<ExprRValue *> list;
    int refcount;
};

class ExprDict:public ExprLValue{
public:
    ExprDict(const string &name);
    unorder_map<ExprRValue *,ExprRValue *> map;
    int refcount;
};

class ExprRValue:public Expr{
};

class ExprNum:public ExprRValue{
public:
    ExprNum(int val);
    int val;
}

class ExprBoolean:public ExprNum{
public:
    ExprBoolean(bool val);
    bool val;
};

class ExprString:public ExprRValue{
public:
    ExprString(const string &str);
    string str;
}

class StackFrame{
public:
    StackFrame(StackFrame *);

};

class

#endif
