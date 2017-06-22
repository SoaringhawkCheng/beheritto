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
    NodeType *semanticAnalyzer()=0;
    int line;
};

class Expr:public ASTNode{
public:
    Expr();
    int getExprNodeType()=0;
    Result *evaluate()=0;
    StackFrame *curstackframe;
};

class ExprOpUnary:public Expr{
public:
    ExprOpUnary(Expr *expr);
    Expr *expr;
};

class ExprInvert:public ExprOpUnary{
public:
    ExprInvert(Expr *expr);
    string toString();
    NodeType *semanticAnalyzer();
    int getExprNodeType();
    Result *evaluate();
};
class ExprNegate:public ExprOpUnary{
public:
    ExprNegate(Expr *expr);
    string toString();
    NodeType *semanticAnalyzer();
    int getExprNodeType();
    Result *evaluate();
};

class ExprOpBinary:public Expr{
public:
    ExprOpBinary(const string &opname,Expr *lexpr,Expr *rexpr);
    string opname;
    Expr *lexpr;
    Expr *rexpr;
};

class ExprArithmetic:public ExprOpBinary{
public:
    ExprArithmetic(const string &opname,Expr *lexpr,Expr *rexpr);
    string toString();
    NodeType *semanticAnalyzer();
    int getExprNodeType();
    Result *evaluate();
};

class ExprBitwise:public ExprOpBinary{
public:
    ExprBitwise(const string &opname,Expr *lexpr,Expr *rexpr);
    string toString();
    NodeType *semanticAnalyzer();
    int getExprNodeType();
    Result *evaluate();
};

class ExprCompare:public ExprOpBinary{
public:
    ExprCompare(const string &opname,Expr *lexpr,Expr *rexpr);
    string toString();
    NodeType *semanticAnalyzer();
    int getExprNodeType();
    Result *evaluate();
};

class ExprLogic:public ExprOpBinary{
public:
    ExprLogic(const string &opname,Expr *lexpr,Expr *rexpr);
    string toString();
    NodeType *semanticAnalyzer();
    int getExprNodeType();
    Result *evaluate();
};

class ExprLValue:public Expr{
public:
    ExprLValue(const string &varname);
    virtual void setResult(Result *result)=0;
    virtual void setNodeType(NodeType *type)=0;
    string varname;
    //DeclMethod *enclosingMethod;
};

class ExprVariable:public ExprLValue{
public:
    ExprVariable(const string &varname);
    string toString();
    NodeType *semanticAnalyzer();
    int getExprNodeType();
    void setResult(Result *result);
    void setNodeType(NodeType *type);
    Result *evaluate();
};

class ExprArray:public ExprLValue{
public:
    ExprList(const string &varname,StmtSlice *slice);
    StmtSlice *slice;
    string toString();
    NodeType *semanticAnalyzer();
    int getExprNodeType();
    void setResult(Result *result);
    void setNodeType(NodeType *type);
    Result *evaluate();
};

class ExprConstant:public Expr{
};

class ExprNum:public ExprConstant{
public:
    ExprNum(int val);
    string toString();
    NodeType *semanticAnalyzer();
    int getExprNodeType();
    Result *evaluate();
    int val;
};

class ExprBoolean:public ExprConstant{
public:
    ExprBoolean(bool val);
    bool val;
    string toString();
    NodeType *semanticAnalyzer();
    int getExprNodeType();
    Result *evaluate();
};

class ExprString:public ExprConstant{
public:
    ExprString(const string &str);
    string str;
    string toString();
    NodeType *semanticAnalyzer();
    int getExprNodeType();
    Result *evaluate();
};

class ExprMethodCall:public Expr{
public:
    ExprMethodCall(const string &methodname);
    string toString();
    NodeType *semanticAnalyzer();
    int getExprNodeType();
    Result *evaluate();
    vector<Expr *> arglist;
    string methodname;
}


class Statement:public ASTNode{
public:
    Statement();
    virtual void execute()=0;
    //DeclMethod *enclosingMethod;
}

class StmtBlock:public Statement{
public:
    StmtBlock();
    string toString();
    void semanticAnalyzer();
    void execute();
    vector<Statement *> statements;
    StackFrame *curstackframe;
    //bool break,continue;

}


class StackFrame{
public:
    StackFrame(StackFrame *);

};

class

#endif
