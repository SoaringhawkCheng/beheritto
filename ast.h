#ifndef ast_h
#define ast_h

#include <iostream>
#include <vector>
#include <unordered_map>

#include "astnode.h"

class ASTNode{
public:
    ASTNode();
    virtual string toString()=0;//凡是没有定义toString的派生类都是抽象类
    int line;
};

class Expr:public ASTNode{
public:
    Expr();
    virtual NodeType *semanticAnalyzer();
    int getExprNodeType();
};

class ExprOpUnary:public Expr{
public:
    ExprOpUnary(Expr *expr);
    int getExprNodeType();
    Expr *expr;
};

class ExprInvert:public ExprOpUnary{
public:
    ExprInvert(Expr *expr);
    string toString();
    NodeType *semanticAnalyzer();
    Result *evaluate();
};
class ExprNegate:public ExprOpUnary{
public:
    ExprNegate(Expr *expr);
    string toString();
    NodeType *semanticAnalyzer();
    Result *evaluate();
};

class ExprOpBinary:public Expr{
public:
    ExprOpBinary(const string &name,Expr *lexpr,Expr *rexpr);
    string name;
    Expr *lexpr;
    Expr *rexpr;
    int getExprNodeType();
};

class ExprArithmetic:public ExprOpBinary{
public:
    ExprArithmetic(const string &name,Expr *lexpr,Expr *rexpr);
    string toString();
    NodeType *semanticAnalyzer();
    Result *evaluate();
};

class ExprBitwise:public ExprOpBinary{
public:
    ExprBitwise(const string &name,Expr *lexpr,Expr *rexpr);
    string toString();
    NodeType *semanticAnalyzer();
    Result *evaluate();
};

class ExprCompare:public ExprOpBinary{
public:
    ExprCompare(const string &name,Expr *lexpr,Expr *rexpr);
    string toString();
    NodeType *semanticAnalyzer();
    Result *evaluate();
};

class ExprLogic:public ExprOpBinary{
public:
    ExprLogic(Expr *lexpr,Expr *rexpr);
    string toString();
    NodeType *semanticAnalyzer();
    Result *evaluate();
};

class ExprLValue:public Expr{
public:
    ExprLValue(const string &name);
    virtual void setResult(Result *result)=0;
    virtual void setNodeType(NodeType *type)=0;
    int getNodeType();
    string varname;
    //DeclMethod *enclosingMethod;
};

class ExprVariable:public ExprLValue{
public:
    ExprVariable(const string &name);
    string toString();
    void setResult(Result *result);
    void setNodeType(NodeType *type);
    NodeType *semanticAnalyzer();
    Result *evaluate();
};

class ExprArray:public ExprLValue{
public:
    ExprList(const string &name,Expr *expr);
    vector<ExprRValue *> list;
    int refcount;
    string toString();
    void setResult(Result *result);
    void setNodeType(NodeType *type);
    NodeType *semanticAnalyzer();
    Result *evaluate();
};

class ExprConstant:public Expr{
    int getExprNodeType();
};

class ExprNum:public ExprConstant{
public:
    ExprNum(int val);
    int val;
    string toString();
    NodeType *semanticAnalyzer();
    Result *evaluate();
};

class ExprBoolean:public ExprConstant{
public:
    ExprBoolean(bool val);
    bool val;
    string toString();
    NodeType *semanticAnalyzer();
    Result *evaluate();
};

class ExprString:public ExprConstant{
public:
    ExprString(const string &str);
    string str;
    string toString();
    NodeType *semanticAnalyzer();
    Result *evaluate();
};

class StackFrame{
public:
    StackFrame(StackFrame *);

};

class

#endif
