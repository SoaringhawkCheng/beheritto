#ifndef astree_h
#define astree_h

#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>

#include "treenode"

using namespace std;

/****************************************************************/
/***************语法树节点类定义***************/

class TreeNode{
public:
    TreeNode();
    virtual string toString()=0;//凡是没有定义toString的派生类都是抽象类
    NodeType *analyzeSemantic()=0;
    int line;
};

class Expr:public TreeNode{
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

class ExprOpposite:public ExprOpUnary{
public:
    ExprOpposite(Expr *expr);
    string toString();
    NodeType *analyzeSemantic();
    int getExprNodeType();
    Result *evaluate();
};
class ExprNot:public ExprOpUnary{
public:
    ExprNegate(Expr *expr);
    string toString();
    NodeType *analyzeSemantic();
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
    ExprArray(const string &varname,Expr *index);
    string toString();
    NodeType *analyzeSemantic();
    int getExprNodeType();
    void setResult(Result *result);
    void setNodeType(NodeType *type);
    Result *evaluate();
    Expr *index;
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
    string toString();
    NodeType *analyzeSemantic();
    int getExprNodeType();
    Result *evaluate();
    string str;
};

class ExprArrayInit:public ExprConstant{
public:
    string toString();
    NodeType *analyzeSemantic();
    int getExprNodeType();
    Result *evaluate();
    vector<Expr *> initlist;
}
/*
class ExprCondition:public Expr{
public:
    ExprCondition();
    string toString();
    NodeType *analyzeSemantic();
    int getExprNodeType();
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
    NodeType *analyzeSemantic();
    int getExprNodeType();
    Result *evaluate();
    vector<Expr *> arglist;
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
    Statement *returnp;
    //bool break,continue;
};

class StmtAssign:public Statement{
public:
    StmtAssign();
    StmtAssign(Expr *lexpr,Expr *rexpr);
    string toString();
    void analyzeSemantic();
    void execute();
    Expr *lexpr;
    Expr *rexpr;
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
    StmtIf();
    string toString();
    void analyzeSemantic();
    void execute();
    Expr *condition;
    StmtBlock *block;
    vector<StmtElif *> eliflist;
    StmtElse *else;
};

class StmtElif:public Statement{
public:
    StmtElif(Expr *condition,Stmtblock *elifblock);
    string toString();
    void analyzeSemantic();
    void execute();
    Expr *condition;
    StmtBlock *block();
    //还有个bool值
};

class StmtElse:public Statement{
public:
    StmtELse(StmtBlock *elifblock);
    string toString();
    void analyzeSemantic();
    void execute();
    StmtBlock *block;
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
    Stmtblock *block;
};

class StmtFor:public Statement{
public:
    StmtFor();
    string toString();
    void analyzeSemantic();
    void execute();
    string targetname;
    Stmtblock *block;
    StmtRange *range;
    string objectname;
};

class StmtReturn:public Statement{
public:
    StmtReturn(Expr *exor);
    string toString();
    void analyzeSemantic();
    void execute();
    Expr * exprreturn;
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
    StmtInput(Expr *lvalue);
    string toString();
    void analyzeSemantic();
    void execute();
    Expr *lvalue;
};

class StmtPrint:public Statement{
public:
    StmtPrint();
    string toString();
    void analyzeSemantic();
    void execute();
    Vector<Expr *> printlist;
};

class StmtRange:public Statement{
public:
    StmtRange();
    string toString();
    void analyzeSemantic();
    void execute();
    int begin;
    int end;
    int step;
}

class Declaration:public TreeNode{
public:
    Declaration();
    virtual void intepret()=0;
    StackFrame *curstack;
};

class DeclProgram:public Declaration{
public:
    DeclProgram(const string &progname);
    ~DeclProgram();
    string toString();
    void analyzeSemantic();
    void intepret();
    string progname;
    vector<DeclModule *> modulelist;
    vector<DeclClass *> classlist;
    vector<DeclMethod *> methodlist;
    DeclEntry *entry;
};

class DeclModule:public Declaration{
public:
    DeclModule();
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
    DeclConstructor *constructor;
    vector<DeclMethod *> methodlist;
    vector<DeclField *> fieldlist;
};

class DeclConstructor:public Declaration{
public:
    DeclConstructor();
    ~DeclConstructor();
    string toString();
    void analyzeSemantic();
    void intepret();
    string name;
    ASTree *self;
    vector<string> paralist;
    StmtBlock *block;
};
class DeclMethod:public Declaration{
public:
    DeclMethod(const string &methodname);
    ~DeclMethod();
    string toString();
    void analyzeSemantic();
    void intepret();
    string methodname;
    vector<string> paralist;
    StmtBlock *block;
};
class DeclField:public Declaration{
public:
    DeclField(StmtAssign *stmtassign);
    ~DeclField();
    string toString();
    void analyzeSemantic();
    void intepret();
    StmtAssign *assign;
};

class DeclEntry{
public:

}

/****************************************************************/

class Type{
public:
    virtual int getType()=0;
    virtual bool isEquivalent(Type *type)=0;
};

class Equal:public Type{
public:
    int getType();
    bool isEquivalent(Tyoe *type);
};
class NotEqual:public Type{
public:
    int getType();
    bool isEquivalent(Tyoe *type);
};
class Boolean:public Type{
public:
    int getType();
    bool isEquivalent(Tyoe *type);
};

class String:public Type{
public:
    int getType();
    bool isEquivalent(Tyoe *type);
};

class Array:public Type{
public:
    int getType();
    bool isEquivalent(Tyoe *type);
    int size;
    Type *type;
};

class VoidF:public Type{
public:
    int getType();
    bool isEquivalent(Tyoe *type);
};

class Method:public Type{
public:
    int getType();
    bool isEquivalent(Tyoe *type);
    Type * returntype;
    map<string,Type *> Paramters;
};

/****************************************************************/

class StackFrame{
public:
    StackFrame(StackFrame *prev);
    bool exists(string key);
    Type *get(string key);
    void put(string key,Type *type);
    void set(string key,Type *type);
    StackFrame *prev;
    map<string,Type *> symboltable;
};

/****************************************************************/

class Variable{
public:
    Variable(string varname,Result *val);
    string name;
    Result *value;
}

#endif
