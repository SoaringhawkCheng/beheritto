#ifndef treenode_h
#define treenode_h

#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>

#include "declnode.h"

class TreeNode{
public:
    TreeNode();
    virtual string toString()=0;//凡是没有定义toString的派生类都是抽象类
    NodeType *analyzeSemantic()=0;
    int line;
};

class Expr:public TreeNode{
public:
    Expression();
    int getExprNodeType()=0;
    Result *evaluate()=0;
    StackFrame *curstackframe;
};

class ExprOpUnary:public Expression{
public:
    ExprOpUnary(Expressoin *expression);
    Expression *expr;
};

class ExprInvert:public ExprOpUnary{
public:
    ExprInvert(Expression *expr);
    string toString();
    NodeType *analyzeSemantic();
    int getExprNodeType();
    Result *evaluate();
};
class ExprNegate:public ExprOpUnary{
public:
    ExprNegate(Expression *expr);
    string toString();
    NodeType *analyzeSemantic();
    int getExprNodeType();
    Result *evaluate();
};

class ExprOpBinary:public Expression{
public:
    ExprOpBinary(const string &opname,Expr *lexpr,Expr *rexpr);
    string opname;
    Expression *lexpr;
    Expression *rexpr;
};

class ExprArithmetic:public ExprOpBinary{
public:
    ExprArithmetic(const string &opname,Expr *lexpr,Expr *rexpr);
    string toString();
    NodeType *analyzeSemantic();
    int getExprNodeType();
    Result *evaluate();
};

class ExprBitwise:public ExprOpBinary{
public:
    ExprBitwise(const string &opname,Expr *lexpr,Expr *rexpr);
    string toString();
    NodeType *analyzeSemantic();
    int getExprNodeType();
    Result *evaluate();
};

class ExprCompare:public ExprOpBinary{
public:
    ExprCompare(const string &opname,Expr *lexpr,Expr *rexpr);
    string toString();
    NodeType *analyzeSemantic();
    int getExprNodeType();
    Result *evaluate();
};

class ExprLogic:public ExprOpBinary{
public:
    ExprLogic(const string &opname,Expr *lexpr,Expr *rexpr);
    string toString();
    NodeType *analyzeSemantic();
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
    NodeType *analyzeSemantic();
    int getExprNodeType();
    void setResult(Result *result);
    void setNodeType(NodeType *type);
    Result *evaluate();
};

class ExprArray:public ExprLValue{
public:
    ExprList(const string &varname,StmtSlice *slice);
    string toString();
    NodeType *analyzeSemantic();
    int getExprNodeType();
    void setResult(Result *result);
    void setNodeType(NodeType *type);
    Result *evaluate();
    StmtSlice *slice;
};

class ExprConstant:public Expr{
};

class ExprNum:public ExprConstant{
public:
    ExprNum(int val);
    string toString();
    NodeType *analyzeSemantic();
    int getExprNodeType();
    Result *evaluate();
    int val;
};

class ExprBoolean:public ExprConstant{
public:
    ExprBoolean(bool val);
    bool val;
    string toString();
    NodeType *analyzeSemantic();
    int getExprNodeType();
    Result *evaluate();
};

class ExprString:public ExprConstant{
public:
    ExprString(const string &str);
    string str;
    string toString();
    NodeType *analyzeSemantic();
    int getExprNodeType();
    Result *evaluate();
};

class ExprMethodCall:public Expr{
public:
    ExprMethodCall(const string &methodname);
    string toString();
    NodeType *analyzeSemantic();
    int getExprNodeType();
    Result *evaluate();
    vector<Expression *> arglist;
    string methodname;
};


class Statement:public TreeNode{
public:
    Statement();
    virtual void execute()=0;
    //DeclMethod *enclosingMethod;
};

class StmtBlock:public Statement{
public:
    StmtBlock();
    string toString();
    void analyzeSemantic();
    void execute();
    vector<Statement *> statements;
    StackFrame *curstackframe;
    //bool break,continue;
};

class StmtAssignment:public Statement{
public:
    StmtAssignment(Expr *lvalue,rvalue);
    string toString();
    void analyzeSemantic();
    void execute();
    Expression *lvalue;
    Expression *rvalue;
};

class StmtMethodCall:public Statement{
public:
    StmtMethodCall(ExprMethodCall *methodcall);
    string toString();
    void analyzeSemantic();
    void execute();
    ExprMethodCall *methodcall;
};

class StmtIf:public Statement{
public:
    StmtIf(Expr *condition,Stmtblock *ifblock);
    string toString();
    void analyzeSemantic();
    void execute();
    Expression *condition;
    StmtBlock *ifblock;
    vector<StmtElif *> elifblocklist;
    Stmtblock *elseblock;
};

class StmtElif:public Statement{
public:
    StmtElif(Expr *condition,Stmtblock *elifblock);
    string toString();
    void analyzeSemantic();
    void execute();
    Expression *condition;
    Stmtblock *elifblock;
    //还有个bool值
};

class StmtElse:public Statement{
public:
    StmtELse(StmtBlock *elifblock);
    string toString();
    void analyzeSemantic();
    void execute();
    StmtBlock *elseblock;
};

class StmtIteration:public Statement{
}

class StmtWhile:public Statement{
public:
    StmtWhile(Expr *condition,Stmtblock *whileblock);
    string toString();
    void analyzeSemantic();
    void execute();
    Expr *condition;
    Stmtblock *whileblock;
};

class StmtFor:public Statement{
public:
    StmtFor();
    string toString();
    void analyzeSemantic();
    void execute();
    Stmtblock *forblock;
};

class Stmtreturn:public Statement{
public:
    StmtReturn(Expression *exor);
    string toString();
    void analyzeSemantic();
    void execute();
    Expression * expr;
};

class Stmtbreak:public Statement{
public:
    Stmtbreak();
    string toString();
    void analyzeSemantic();
    void execute();
    StmtLoop *loop;
};

class StmtContinue:public Statement{
public:
    StmtContinue();
    string toString();
    void analyzeSemantic();
    void execute();
    StmtLoop *loop;
};

class StmtInput:public Statement{
public:
    StmtInput(Expression *lvalue);
    string toString();
    void analyzeSemantic();
    void execute();
    Expression *lvalue;
};

class StmtPrint:public Statement{
public:
    StmtPrint(vector<Expression *> printlist);
    string toString();
    void analyzeSemantic();
    void execute();
};

class Declaration:public TreeNode{
public:
    Declaration();
    virtual void intepret()=0;
    StackFrame *curstackframe;
};

class DeclProgram:public Declaration{
public:
    DeclProgram();
    ~DeclProgram();
    string toString();
    void analyzeSemantic();
    void intepret();
    vector<DeclModule *> modulelist;
    vector<DeclClass *> classlist;
    vector<DeclMethod *> methodlist;
    DeclEntry *entry;
};

class DeclModule:public Declaration{
public:
    DeclModule(const string &modname);
    ~DeclModule();
    string toString();
    void analyzeSemantic();
    void intepret();
    vector<DeclModule *> modulelist;
    vector<DeclClass *> classlist;
    vector<DeclMethod *> methodlistl
};

class DeclClass:public Declaration{
public:
    DeclClass(const string &classname);
    ~DeclClass();
    string toString();
    void analyzeSemantic();
    void intepret();
    string classname;
    vector<DeclMethod *> methodlist;
    vector<DeclField *> fieldlist;
};
class DeclMethod:public Declaration{
public:
    DeclMethod(const string &methodname);
    ~DeclMethod();
    string toString();
    void analyzeSemantic();
    void intepret();
    string methodname;
    StmtBlock *block;
};
class DeclField{
public:
    DeclField(StmtAssign *stmtassign);
    ~DeclField();
    string toString();
    void analyzeSemantic();
    void intepret();
    StmtAssign *assign;
}

class DeclEntry{
public:

}

class StackFrame{
    public:
    StackFrame(StackFrame *);

};

class

#endif
