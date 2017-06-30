#ifndef astree_h
#define astree_h

#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <stdlib.h>

#include "treenode.h"

using namespace std;
/****************************************************************/
/***************全局环境变量声明***************/

//StackFrameSlot
//StackFrame *curstackframe;

/****************************************************************/
/***************语法树节点类定义***************/

class TreeNode{
public:
    TreeNode();
    virtual string toString()=0;//凡是没有定义toString的派生类都是抽象类
    virtual Type *analyzeSemantic()=0;
    int line;
};

/****************************************************************/
/***************运算类节点类定义***************/

class Expr:public TreeNode{
public:
    Expr();
    virtual int getExprType()=0;
    virtual Result *evaluate()=0;
    StackFrame *stackframe;
};

class ExprOpUnary:public Expr{
public:
    ExprOpUnary(Expr *expr);
    Expr *expr;
};

class ExprOpposite:public ExprOpUnary{
public:
    ExprOpposite(Expr *expr);
    string toString();
    Type *analyzeSemantic();
    int getExprType();
    Result *evaluate();
};
class ExprNot:public ExprOpUnary{
public:
    ExprNot(Expr *expr);
    string toString();
    Type *analyzeSemantic();
    int getExprType();
    Result *evaluate();
};

class ExprOpBinary:public Expr{
public:
    ExprOpBinary(const string &opname,Expr *lexpr,Expr *rexpr);
    string opname;
    Expr *lexpr;
    Expr *rexpr;
};

class ExprArith:public ExprOpBinary{
public:
    ExprArith(const string &opname,Expr *lexpr,Expr *rexpr);
    string toString();
    Type *analyzeSemantic();
    int getExprType();
    Result *evaluate();
};

class ExprBitwise:public ExprOpBinary{
public:
    ExprBitwise(const string &opname,Expr *lexpr,Expr *rexpr);
    string toString();
    Type *analyzeSemantic();
    int getExprType();
    Result *evaluate();
};

class ExprCompare:public ExprOpBinary{
public:
    ExprCompare(const string &opname,Expr *lexpr,Expr *rexpr);
    string toString();
    Type *analyzeSemantic();
    int getExprType();
    Result *evaluate();
};

class ExprLogic:public ExprOpBinary{
public:
    ExprLogic(const string &opname,Expr *lexpr,Expr *rexpr);
    string toString();
    Type *analyzeSemantic();
    int getExprType();
    Result *evaluate();
};

class ExprVariable:public Expr{
public:
    ExprVariable(const string &varname);
    virtual void setResult(Result *result)=0;
    virtual void setType(Type *type)=0;
    string varname;
    //DeclMethod *enclosingMethod;
};

class ExprScalar:public ExprVariable{
public:
    ExprScalar(const string &varname);
    string toString();
    Type *analyzeSemantic();
    int getExprType();
    void setResult(Result *result);
    void setType(Type *type);
    Result *evaluate();
};

class ExprArray:public ExprVariable{
public:
    ExprArray(const string &varname,Expr *index);
    string toString();
    Type *analyzeSemantic();
    int getExprType();
    void setResult(Result *result);
    void setType(Type *type);
    Result *evaluate();
    Expr *index;
};

class ExprConstant:public Expr{
};

class ExprNum:public ExprConstant{
public:
    ExprNum(int val);
    string toString();
    Type *analyzeSemantic();
    int getExprType();
    Result *evaluate();
    int val;
};

class ExprBoolean:public ExprConstant{
public:
    ExprBoolean(bool val);
    bool val;
    string toString();
    Type *analyzeSemantic();
    int getExprType();
    Result *evaluate();
};

class ExprString:public ExprConstant{
public:
    ExprString(const string &str);
    string toString();
    Type *analyzeSemantic();
    int getExprType();
    Result *evaluate();
    string str;
};

class ExprArrayInit:public ExprConstant{
public:
    string toString();
    Type *analyzeSemantic();
    int getExprType();
    Result *evaluate();
    vector<Expr *> initlist;
};
/*
class ExprCondition:public Expr{
public:
    ExprCondition();
    string toString();
    Type *analyzeSemantic();
    int getExprType();
    Result *evaluate();
    bool hasnot;
    Expr * expr;
    bool hasin;
    bool notin;
    ExprArray * array;
}*/

class ExprMethodCall:public Expr{
public:
    ExprMethodCall(const string &methodname);
    string toString();
    Type *analyzeSemantic();
    int getExprType();
    Result *evaluate();
    vector<Expr *> arglist;
    string methodname;
};


/****************************************************************/
/***************语句类节点类定义***************/

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
    Type *analyzeSemantic();
    void execute();
    vector<Statement *> statements;
    StackFrame *curstackframe;
    Statement *returnp;
    //bool break,continue;
};

class StmtAssign:public Statement{
public:
    StmtAssign();
    StmtAssign(Expr *lexpr,Expr *rexpr);
    string toString();
    Type *analyzeSemantic();
    void execute();
    Expr *lexpr;
    Expr *rexpr;
};

class StmtMethodCall:public Statement{
public:
    StmtMethodCall(ExprMethodCall *methodcall);
    string toString();
    Type *analyzeSemantic();
    void execute();
    ExprMethodCall *methodcall;
};

class StmtIf:public Statement{
public:
    StmtIf();
    string toString();
    Type *analyzeSemantic();
    void execute();
    Expr *condition;
    StmtBlock *ifblock;
    vector<StmtElif *> eliflist;
    StmtBlock *elseblock;
};

class StmtElif:public Statement{
public:
    StmtElif(Expr *condition,StmtBlock *block);
    string toString();
    Type *analyzeSemantic();
    void execute();
    Expr *condition;
    StmtBlock *block();
    //还有个bool值
};

class StmtElse:public Statement{
public:
    StmtElse(StmtBlock *elifblock);
    string toString();
    Type *analyzeSemantic();
    void execute();
    StmtBlock *elifblock;
};

class StmtIteration:public Statement{
};

class StmtWhile:public Statement{
public:
    StmtWhile(Expr *condition,StmtBlock *whileblock);
    string toString();
    Type *analyzeSemantic();
    void execute();
    Expr *condition;
    StmtBlock *whileblock;
};

class StmtFor:public Statement{
public:
    StmtFor();
    string toString();
    Type *analyzeSemantic();
    void execute();
    string targetname;
    StmtBlock *forblock;
    StmtRange *range;
    string objectname;
};

class StmtReturn:public Statement{
public:
    StmtReturn(Expr *exor);
    string toString();
    Type *analyzeSemantic();
    void execute();
    Expr * exprreturn;
};

class Stmtbreak:public Statement{
public:
    Stmtbreak();
    string toString();
    Type *analyzeSemantic();
    void execute();
    StmtLoop *loop;
};

class StmtContinue:public Statement{
public:
    StmtContinue();
    string toString();
    Type *analyzeSemantic();
    void execute();
    StmtLoop *loop;
};

class StmtInput:public Statement{
public:
    StmtInput(Expr *lvalue);
    string toString();
    Type *analyzeSemantic();
    void execute();
    Expr *lvalue;
};

class StmtPrint:public Statement{
public:
    StmtPrint();
    string toString();
    Type *analyzeSemantic();
    void execute();
    vector<Expr *> printlist;
};

class StmtRange:public Statement{
public:
    StmtRange();
    string toString();
    Type *analyzeSemantic();
    void execute();
    int begin;
    int end;
    int step;
};

/****************************************************************/
/***************声明类节点类定义***************/

class Declaration:public TreeNode{
public:
    Declaration();
    virtual void intepret()=0;
    StackFrame *curstack;
};

class DeclModule:public Declaration{
public:
    DeclModule(const string &modname);
    ~DeclModule();
    string toString();
    Type *analyzeSemantic();
    void intepret();
    string progname;
    StackFrame *curstackframe;

    vector<DeclModule *> modulelist;
    vector<DeclClass *> classlist;
    vector<DeclFunction *> functionlist;
    DeclEntry *entry;
};

class DeclFunction:public Declaration{
public:
    DeclFunction(const string &functionname);
    ~DeclFunction();
    string toString();
    Type *analyzeSemantic();
    void intepret();
    string functionname;
    vector<string> paralist;
    StmtBlock *functionblock;
};

class DeclClass:public Declaration{
public:
    DeclClass(const string &classname);
    ~DeclClass();
    string toString();
    Type *analyzeSemantic();
    void intepret();
    string classname;
    vector<string> paralist;
    vector<DeclMethod *> methodlist;
    vector<DeclField *> fieldlist;
};

class DeclMethod:public Declaration{
public:
    DeclMethod(const string &methodname);
    ~DeclMethod();
    string toString();
    Type *analyzeSemantic();
    void intepret();
    string methodname;
    vector<string> paralist;
    DeclClass *self;
    StmtBlock *methodblock;
};

class DeclField:public Declaration{
public:
    DeclField(StmtAssign *stmtassign);
    ~DeclField();
    string toString();
    Type *analyzeSemantic();
    void intepret();
    StmtAssign *assign;
};

class DeclEntry{
public:
    DeclEntry();
    ~DeclEntry();
    string toString();
    Type *analyzeSemantic();
    void intepret();
    vector<Statement *> statements;
};

/****************************************************************/
/***************类型类节点类定义***************/

class Type{
public:
    virtual int getType()=0;
    virtual bool isEquivalent(Type *type)=0;
};

class Var:public Type{
public:
    int getType();
    bool isEquivalent(Type *type);
};
class Num:public Type{
public:
    int getType();
    bool isEquivalent(Type *type);
};
class Boolean:public Type{
public:
    int getType();
    bool isEquivalent(Type *type);
};

class String:public Type{
public:
    int getType();
    bool isEquivalent(Type *type);
};

class Array:public Type{
public:
    int getType();
    bool isEquivalent(Type *type);
    int size;
    Type *type;
};

class Void:public Type{
public:
    int getType();
    bool isEquivalent(Type *type);
};

class Method:public Type{
public:
    int getType();
    bool isEquivalent(Type *type);
    Type * returntype;
    unordered_map<string,Type *> Paramters;
};

/****************************************************************/
/***************运算结果类节点类定义***************/



/****************************************************************/

class Variable{
public:
    Variable(string varname,Result *val);
    string name;
    Result *value;
};

/****************************************************************/
/***************环境变量类节点类定义***************/

class StackFrame{
public:
    StackFrame(StackFrame *prev);
    bool exists(string key);
    Type *get(string key);
    void put(string key,Type *type);
    void set(string key,Type *type);
    StackFrame *prev;
    unordered_map<string,Type *> symboltable;
};

//class StackFrameSlot{
//public:
//    vector<>
//};

//class Procedure{
//public:
//};

#endif
