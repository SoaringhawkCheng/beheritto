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

extern DeclModule *program;
extern SymbolTable symboltable;
extern RuntimeStack runtimestack;
extern DeclModule *curmodule;
extern DeclClass *curclass;
extern DeclMethod *curmethod;
extern StmtLoop *curloop;
extern string curmodname;
extern int curline;

/****************************************************************/
/*************************语法树节点类定义*************************/

class ASTree{
public:
    int line;
    string modname;
};

/****************************************************************/
/*************************运算类节点类定义*************************/

class Expr:public ASTree{
public:
    virtual int getExprType()=0;
    virtual Object *evaluate()=0;
};

class ExprOpUnary:public Expr{
public:
    int getExprType();
    ExprOpUnary(Expr *expr);
    Expr *expr;
};

class ExprOpposite:public ExprOpUnary{
public:
    ExprOpposite(Expr *expr);
    Object *evaluate();
};
class ExprNot:public ExprOpUnary{
public:
    ExprNot(Expr *expr);
    Object *evaluate();
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
    Object *evaluate();
};

class ExprBitwise:public ExprOpBinary{
public:
    ExprBitwise(const string &opname,Expr *lexpr,Expr *rexpr);
    Object *evaluate();
};

class ExprCompare:public ExprOpBinary{
public:
    ExprCompare(const string &opname,Expr *lexpr,Expr *rexpr);
    Object *evaluate();
};

class ExprLogic:public ExprOpBinary{
public:
    ExprLogic(const string &opname,Expr *lexpr,Expr *rexpr);
    Object *evaluate();
};

class ExprLValue:public Expr{
public:
    ExprLValue(const string &varname);
    int getExprType();
    virtual void setObject(Object *object)=0;
    string varname;
    DeclMethod *enclosingmethod;
};

class ExprID:public ExprLValue{
public:
    ExprID(const string &varname);
    Object *evaluate();
    void setObject(Object *object);
};

class ExprArray:public ExprLValue{
public:
    ExprArray(const string &varname,Expr *index);
    Object *evaluate();
    void setObject(Object *object);
    Expr *index;
};

class ExprConstant:public Expr{
    int getExprType();
};

class ExprInteger:public ExprConstant{
public:
    ExprInteger(int value);
    Object *evaluate();
    int value;
};

class ExprFloat:public ExprConstant{
public:
    ExprFloat(double value);
    Object *evaluate();
    double value;
};

class ExprBoolean:public ExprConstant{
public:
    ExprBoolean(bool value);
    Object *evaluate();
    bool value;
};

class ExprString:public ExprConstant{
public:
    ExprString(const string &value);
    Object *evaluate();
    string value;
};

class ExprArrayInit:public ExprConstant{
public:
    Object *evaluate();
    vector<Expr *> initlist;
};

class ExprRange:public ExprConstant{
public:
    Object *evaluate();
    Expr *begin;
    Expr *end;
    Expr *step;
};

class ExprMethodCall:public Expr{
public:
    ExprMethodCall(const string &methodname);
    int getExprType();
    DeclModule *enclosingmodule;
    DeclClass *enclosingclass;
    Object *evaluate();
    string methodname;
    vector<Expr *> arglist;
};

class ExprInput:public Expr{
public:
    ExprInput(const string &tip);
    int getExprType();
    string tip;
    Object *evaluate();
};

/****************************************************************/
/*************************语句类节点类定义*************************/

class Statement:public ASTree{
public:
    Statement();
    virtual void execute()=0;
    DeclMethod *enclosingmethod;
};

class StmtBlock:public Statement{
public:
    StmtBlock();
    void execute();
    vector<Statement *> statements;
    bool continuepoint;
    bool breakpoint;
};

class StmtAssign:public Statement{
public:
    StmtAssign();
    StmtAssign(Expr *lexpr,Expr *rexpr);
    void execute();
    Expr *lexpr;
    Expr *rexpr;
};

class StmtMethodCall:public Statement{
public:
    StmtMethodCall(ExprMethodCall *methodcall);
    void execute();
    ExprMethodCall *methodcall;
};

class StmtIf:public Statement{
public:
    StmtIf();
    void execute();
    Expr *condition;
    StmtBlock *ifblock;
    vector<StmtElif *> eliflist;
    StmtBlock *elseblock;
};

class StmtElif:public Statement{
public:
    StmtElif(Expr *condition,StmtBlock *elifblock);
    void execute();
    Expr *condition;
    StmtBlock *elifblock;
    bool executed;
};

class StmtIteration:public Statement{
};

class StmtWhile:public Statement{
public:
    StmtWhile(Expr *condition,StmtBlock *whileblock);
    void execute();
    Expr *condition;
    StmtBlock *whileblock;
};

class StmtFor:public Statement{
public:
    void execute();
    string targetname;
    ExprRange *range;
    string objectname;
    StmtBlock *forblock;
};

class StmtReturn:public Statement{
public:
    StmtReturn(Expr *ret);
    void execute();
    Expr *ret;
};

class StmtBreak:public Statement{
public:
    void execute();
};

class StmtContinue:public Statement{
public:
    void execute();
};

class StmtPrint:public Statement{
public:
    void execute();
    vector<Expr *> printlist;
};

class StmtPrintLn:public Statement{
public:
    void execute();
    vector<Expr *> printlist;
};

/****************************************************************/
/*************************声明类节点类定义*************************/

class Declaration:public ASTree{
public:
    virtual void intepret()=0;
    virtual int getDeclType()=0;
};

class DeclModule:public Declaration{
public:
    DeclModule(const string &modname);
//    ~DeclModule();
    int getDeclType();
    void intepret();
    string modname;

    unordered_map<string, DeclModule *> modulelist;
    unordered_map<string, DeclClass *> classlist;
    unordered_map<string, DeclMethod *> methodlist;
    DeclEntry *entry;
};

class DeclClass:public Declaration{
public:
    DeclClass(const string &classname);
//    ~DeclClass();
    int getDeclType();
    void intepret();
    string classname;
    DeclModule *enclosingmodule;
    DeclClass *enclosingclass;
    StmtBlock *classblock;
    vector<string> paralist;
    unordered_map<string, DeclMethod *> methodlist;
    vector<DeclField *> fieldlist;
};

class DeclMethod:public Declaration{
public:
    DeclMethod(const string &methodname);
//    ~DeclMethod();
    int getDeclType();
    void intepret();
    string methodname;
    DeclModule *enclosingmodule;
    DeclClass *enclosingclass;
    vector<string> paralist;
    StmtBlock *methodblock;
    Object *returnobj;
};

class DeclField:public Declaration{
public:
    DeclField(StmtAssign *assign);
//    ~DeclField();
    int getDeclType();
    void intepret();
    StmtAssign *assign;
};

class DeclEntry:public Declaration{
public:
//    ~DeclEntry();
    int getDeclType();
    void intepret();
    vector<Statement *> statements;
};

/****************************************************************/
/*************************运算结果节点类定义*************************/

class Object:public ASTree{
public:
    Object();
    virtual void toString()=0;
    virtual int getObjType()=0;
    virtual void print()=0;
};

class ObjInteger:public Object{
public:
    void toString();
    ObjInteger(int value);
    int getObjType();
    void print();
    int value;
};

class ObjFloat:public Object{
public:
    void toString();
    ObjFloat(double value);
    int getObjType();
    void print();
    double value;
};

class ObjBoolean:public Object{
public:
    void toString();
    ObjBoolean(bool value);
    int getObjType();
    void print();
    bool value;
};

class ObjString:public Object{
public:
    void toString();
    ObjString(string value);
    int getObjType();
    void print();
    string value;
};

class ObjArray:public Object{
public:
    void toString();
    ObjArray();
    int getObjType();
    void print();
    vector<Object *> value;
};

class ObjClass:public Object{
public:
    void toString();
    int getObjType();
    void print();
    vector<string> paralist;
    unordered_map<string, Object *> fieldlist;
    unordered_map<string, DeclMethod *> methodlist;
};

/****************************************************************/
/*************************环境变量节点类定义*************************/

class SymbolTable{
public:
    Declaration *get(const string &key,DeclModule *enclosingmodule,DeclClass *enclosingclass);
    DeclModule *enclosingmodule;
    DeclClass *enclosingclass;
};

class RuntimeStack{
public:
    bool exists(const string &key);
    Variable *get(const string &key);
    void toString();
    void put(const string &key,Variable *variable);
    void push(StackFrame * newstackframe);
    void pop();
    vector<StackFrame *> stackframelist;
};

class StackFrame{
public:
    unordered_map<string,Variable *> variabletable;
};

class Variable{
public:
    Variable(const string &varname,Object *object);
    string varname;
    Object *object;
};

#endif
