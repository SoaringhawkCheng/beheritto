#ifndef astree_h
#define astree_h

#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <string>
#include <stdlib.h>

#include "treenode.h"
#include "error.h"

using namespace std;
/****************************************************************/
/*************************全局环境变量声明*************************/

//SymbolTable
//SymbolTable *symboltable;
//extern Runtimestack runtimestack;
/****************************************************************/
/*************************语法树节点类定义*************************/

class ASTree{
public:
    ASTree();
    //virtual //string toString()()=0;//凡是没有定义toString的派生类都是抽象类
    //Type *analyzeSemantic()=0;
    //virtual int getExprType()=0;
    int line;
    string enclosingmodule;
};

/****************************************************************/
/*************************运算类节点类定义*************************/

class Expr:public ASTree{
public:
    Expr();
    virtual int getExprType()=0;
    virtual Type *analyzeSemantic()=0;
    virtual Result *evaluate()=0;
    SymbolTable *symboltable;
};

class ExprOpUnary:public Expr{
public:
    ExprOpUnary(Expr *expr);
    int getExprType();
    Expr *expr;
};

class ExprOpposite:public ExprOpUnary{
public:
    ExprOpposite(Expr *expr);
    //string toString()();
    Type *analyzeSemantic();
    Result *evaluate();
};
class ExprNot:public ExprOpUnary{
public:
    ExprNot(Expr *expr);
    //string toString()();
    Type *analyzeSemantic();
    Result *evaluate();
};

class ExprOpBinary:public Expr{
public:
    ExprOpBinary(const string &opname,Expr *lexpr,Expr *rexpr);
    int getExprType();
    string opname;
    Expr *lexpr;
    Expr *rexpr;
};

class ExprArith:public ExprOpBinary{
public:
    ExprArith(const string &opname,Expr *lexpr,Expr *rexpr);
    //string toString()();
    Type *analyzeSemantic();
    Result *evaluate();
};

class ExprBitwise:public ExprOpBinary{
public:
    ExprBitwise(const string &opname,Expr *lexpr,Expr *rexpr);
    //string toString()();
    Type *analyzeSemantic();
    Result *evaluate();
};

class ExprCompare:public ExprOpBinary{
public:
    ExprCompare(const string &opname,Expr *lexpr,Expr *rexpr);
    //string toString()();
    Type *analyzeSemantic();
    Result *evaluate();
};

class ExprLogic:public ExprOpBinary{
public:
    ExprLogic(const string &opname,Expr *lexpr,Expr *rexpr);
    //string toString()();
    Type *analyzeSemantic();
    Result *evaluate();
};

class ExprLValue:public Expr{
public:
    ExprLValue(const string &varname);
    int getExprType();
    virtual void setType(Type *type)=0;
    virtual void setResult(Result *result)=0;
    string varname;
    DeclMethod *enclosingmethod;
    //DeclClass *enclosingclass;
    //DeclModule *enclosingmodule;
};

class ExprID:public ExprLValue{
public:
    ExprID(const string &varname);
    //string toString()();
    Type *analyzeSemantic();
    Result *evaluate();
    void setType(Type *type);
    void setResult(Result *result);
};

class ExprArray:public ExprLValue{
public:
    ExprArray(const string &varname,Expr *index);
    //string toString()();
    Type *analyzeSemantic();
    Result *evaluate();
    void setType(Type *type);
    void setResult(Result *result);
    Expr *index;
};

class ExprConstant:public Expr{
public:
    int getExprType();
};

class ExprInteger:public ExprConstant{
public:
    ExprInteger(int value);
    //string toString()();
    Type *analyzeSemantic();
    Result *evaluate();
    int value;
};

class ExprFloat:public ExprConstant{
public:
    ExprFloat(double value);
    //string toString()();
    Type *analyzeSemantic();
    Result *evaluate();
    double value;
};

class ExprBoolean:public ExprConstant{
public:
    ExprBoolean(bool value);
    //string toString()();
    Type *analyzeSemantic();
    Result *evaluate();
    bool value;
};

class ExprString:public ExprConstant{
public:
    ExprString(const string &value);
    //string toString()();
    Type *analyzeSemantic();
    Result *evaluate();
    string value;
};

class ExprArrayInit:public ExprConstant{
public:
    //string toString()();
    Type *analyzeSemantic();
    Result *evaluate();
    vector<Expr *> initlist;
};
/*
class ExprCondition:public Expr{
public:
    ExprCondition();
    //string toString()();
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
    //string toString()();
    int getExprType();
    Type *analyzeSemantic();
    Result *evaluate();
    string methodname;
    vector<Expr *> arglist;
};


/****************************************************************/
/*************************语句类节点类定义*************************/

class Statement:public ASTree{
public:
    Statement();
    virtual void execute()=0;
    virtual void analyzeSemantic()=0;
    DeclMethod *enclosingmethod;
};

class StmtBlock:public Statement{
public:
    StmtBlock();
    //string toString()();
    void analyzeSemantic();
    void execute();
    vector<Statement *> statements;
    SymbolTable *symboltable;
    bool continuepoint;
    bool breakpoint;
};

class StmtAssign:public Statement{
public:
    StmtAssign();
    StmtAssign(Expr *lexpr,Expr *rexpr);
    //string toString()();
    void analyzeSemantic();
    void execute();
    Expr *lexpr;
    Expr *rexpr;
};

class StmtMethodCall:public Statement{
public:
    StmtMethodCall(ExprMethodCall *methodcall);
    //string toString()();
    void analyzeSemantic();
    void execute();
    ExprMethodCall *methodcall;
};

class StmtIf:public Statement{
public:
    StmtIf();
    //string toString()();
    void analyzeSemantic();
    void execute();
    Expr *condition;
    StmtBlock *ifblock;
    vector<StmtElif *> eliflist;
    StmtBlock *elseblock;
};

class StmtElif:public Statement{
public:
    StmtElif(Expr *condition,StmtBlock *elifblock);
    //string toString()();
    void analyzeSemantic();
    void execute();
    Expr *condition;
    StmtBlock *elifblock;
    bool executed;
};
/*
class StmtElse:public Statement{
public:
    StmtElse(StmtBlock *elifblock);
    //string toString()();
    void analyzeSemantic();
    void execute();
    StmtBlock *elseblock;
};
*/

class StmtIteration:public Statement{
};

class StmtWhile:public Statement{
public:
    StmtWhile(Expr *condition,StmtBlock *whileblock);
    //string toString()();
    void analyzeSemantic();
    void execute();
    Expr *condition;
    StmtBlock *whileblock;
};

class StmtFor:public Statement{
public:
    StmtFor();
    //string toString()();
    void analyzeSemantic();
    void execute();
    string targetname;
    StmtRange *range;
    string objectname;
    StmtBlock *forblock;
};

class StmtReturn:public Statement{
public:
    StmtReturn(Expr *ret);
    //string toString()();
    void analyzeSemantic();
    void execute();
    Expr *ret;
};

class StmtBreak:public Statement{
public:
    StmtBreak();
    //string toString()();
    void analyzeSemantic();
    void execute();
    StmtLoop *enclosingloop;
};

class StmtContinue:public Statement{
public:
    StmtContinue();
    //string toString()();
    void analyzeSemantic();
    void execute();
    StmtLoop *enclosingloop;
};

class StmtInput:public Statement{
public:
    StmtInput(Expr *lvalue);
    //string toString()();
    void analyzeSemantic();
    void execute();
    Expr *lvalue;
};

class StmtPrint:public Statement{
public:
    StmtPrint();
    //string toString()();
    void analyzeSemantic();
    void execute();
    vector<Expr *> printlist;
};

class StmtRange:public Statement{
public:
    StmtRange();
    //string toString()();
    void analyzeSemantic();
    void execute();
    int begin;
    int end;
    int step;
};

/****************************************************************/
/*************************声明类节点类定义*************************/

class Declaration:public ASTree{
public:
    Declaration();
    virtual void analyzeSemantic()=0;
    virtual void intepret()=0;
    SymbolTable *symboltable;
};

class DeclModule:public Declaration{
public:
    DeclModule(const string &modname);
    ~DeclModule();
    //string toString()();
    void analyzeSemantic();
    void intepret();
    string modname;

    vector<DeclModule *> modulelist;
    vector<DeclClass *> classlist;
    vector<DeclMethod *> methodlist;
    DeclEntry *entry;
};
/*
class DeclFunction:public Declaration{
public:
    DeclFunction(const string &functionname);
    ~DeclFunction();
    //string toString()();
    Type *analyzeSemantic();
    void intepret();
    string functionname;
    vector<string> paralist;
    StmtBlock *block;
};*/

class DeclClass:public Declaration{
public:
    DeclClass(const string &classname);
    ~DeclClass();
    //string toString()();
    void analyzeSemantic();
    void intepret();
    string classname;
    StmtBlock *classblock;
    vector<string> paralist;
    vector<DeclMethod *> methodlist;
    vector<DeclField *> fieldlist;
};

class DeclMethod:public Declaration{
public:
    DeclMethod(const string &methodname);
    ~DeclMethod();
    //string toString()();
    void analyzeSemantic();
    void intepret();
    string methodname;
    vector<string> paralist;
    StmtBlock *methodblock;
    Result *resret;
};

class DeclField:public Declaration{
public:
    DeclField(StmtAssign *assign);
    ~DeclField();
    //string toString()();
    void analyzeSemantic();
    void intepret();
    StmtAssign *assign;
};

class DeclEntry:public Declaration{
public:
    DeclEntry();
    ~DeclEntry();
    //string toString()();
    void analyzeSemantic();
    void intepret();
    vector<Statement *> statements;
};

/****************************************************************/
/*************************类型类节点类定义*************************/

class Type {
public:
    virtual int getNodeType()=0;
    virtual bool isEquivalent(Type *type)=0;
};

class TypeID:public Type{
public:
    int getNodeType();
    bool isEquivalent(Type *type);
};

class TypeWildcard:public Type{
public:
    int getNodeType();
    bool isEquivalent(Type *type);
};

class TypeScalar:public Type{
public:
    int getNodeType();
    bool isEquivalent(Type *type);
};

class TypeArray:public Type{
public:
    int getNodeType();
    bool isEquivalent(Type *type);
    int size;
    Type *arraytype;
};

class TypeInteger:public Type{
public:
    int getNodeType();
    bool isEquivalent(Type *type);
};

class TypeFloat:public Type{
public:
    int getNodeType();
    bool isEquivalent(Type *type);
};

class TypeBoolean:public Type{
public:
    int getNodeType();
    bool isEquivalent(Type *type);
};

class TypeString:public Type{
public:
    int getNodeType();
    bool isEquivalent(Type *type);
};

class TypeVoid:public Type{
public:
    int getNodeType();
    bool isEquivalent(Type *type);
};

class TypeMethod:public Type{
public:
    int getNodeType();
    bool isEquivalent(Type *type);
    Type * returntype;
    unordered_map<string,Type *> paramap;
};

class TypeClass:public Type{
public:
    int getNodeType();
    bool isEquivalent(Type *type);
    unordered_map<string,Type *> fieldmap;
    unordered_map<string, Type *> methodmap;
};

class TypeModule:public Type{
public:
    int getNodeType();
    bool isEquivalent(Type *type);
    unordered_map<string, Type *> modulemap;
    unordered_map<string, Type *> classmap;
    unordered_map<string, Type *> methodmap;
};

/****************************************************************/
/*************************运算结果节点类定义*************************/

class Result{
public:
    virtual int getNodeType()=0;
    virtual Result *getValue()=0;
    virtual void print()=0;
};

class ResInteger:public Result{
public:
    ResInteger(int value);
    int getNodeType();
    Result *getValue();
    void print();
    int value;
};

class ResFloat:public Result{
public:
    ResFloat(int value);
    int getNodeType();
    Result *getValue();
    void print();
    double value;
};

class ResBoolean:public Result{
public:
    ResBoolean(bool value);
    int getNodeType();
    Result *getValue();
    void print();
    bool value;
};

class ResString:public Result{
public:
    ResString(string value);
    int getNodeType();
    Result *getValue();
    void print();
    string value;
};

class ResArray:public Result{
public:
    int getNodeType();
    Result *getValue();
    void print();
    vector<Result *> value;
};

class ResClass:public Result{
public:
    int getNodeType();
    Result *getValue();
    void print();
    vector<string> paralist;
    unordered_map<string, Variable *> member;
};

class TypeMethod:public Type{
public:
    int getNodeType();
    bool isEquivalent(Type *type);
    Type * returntype;
    unordered_map<string,Type *> paramap;
};

class TypeClass:public Type{
public:
    int getNodeType();
    bool isEquivalent(Type *type);
    unordered_map<string,Type *> fieldmap;
    unordered_map<string, Type *> methodmap;
};

/****************************************************************/
/*************************环境变量节点类定义*************************/

class SymbolTable{
public:
    SymbolTable(SymbolTable *prev);
    bool exists(const string &key);
    Type *get(const string &key);
    void put(const string &key,Type *type);
    void set(const string &key,Type *type);
    SymbolTable *prev;
    unordered_map<string,Type *> symbolmap;
};

class RuntimeStack{
public:
    bool exists(const string &key);
    Variable *get(const string &key);
    void put(const string &key,Variable *variable);
    void push(StackFrame *);
    void pop();
    vector<StackFrame *> stackframelist;
};

class StackFrame{
public:
    unordered_map<string,Variable *> variabletable;
};

class Variable{
public:
    Variable(const string &varname,Result *result);
    string varname;
    Result *result;
};

class Procedure{
public:
    Procedure(DeclMethod *method);
    DeclMethod *method;
};

#endif
