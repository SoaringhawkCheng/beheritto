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
extern string curmodname;
extern int curline;
extern int state;

class MemPool;
class MemList;
class MemBlock;
class MemBlockMeta;

/****************************************************************/
/*************************语法树节点类定义*************************/

class ASTree{
public:
    int line;
    string modname;
//    void *operator new(size_t size);
//    void operator delete(void *buff);
    virtual int getNodeType()=0;
};

/****************************************************************/
/*************************运算类节点类定义*************************/

class Expression:public ASTree{
public:
    int getNodeType();
    virtual int getExprType()=0;
    virtual Object *evaluate()=0;
};

class ExprOpUnary:public Expression{
public:
    int getExprType();
    ExprOpUnary(Expression *expr);
    Expression *expr;
};

class ExprOpposite:public ExprOpUnary{
public:
    ExprOpposite(Expression *expr);
    Object *evaluate();
};
class ExprNot:public ExprOpUnary{
public:
    ExprNot(Expression *expr);
    Object *evaluate();
};

class ExprOpBinary:public Expression{
public:
    ExprOpBinary(const string &opname,Expression *lexpr,Expression *rexpr);
    int getExprType();
    string opname;
    Expression *lexpr;
    Expression *rexpr;
};

class ExprArith:public ExprOpBinary{
public:
    ExprArith(const string &opname,Expression *lexpr,Expression *rexpr);
    Object *evaluate();
};

class ExprBitwise:public ExprOpBinary{
public:
    ExprBitwise(const string &opname,Expression *lexpr,Expression *rexpr);
    Object *evaluate();
};

class ExprCompare:public ExprOpBinary{
public:
    ExprCompare(const string &opname,Expression *lexpr,Expression *rexpr);
    Object *evaluate();
};

class ExprLogic:public ExprOpBinary{
public:
    ExprLogic(const string &opname,Expression *lexpr,Expression *rexpr);
    Object *evaluate();
};

class ExprLValue:public Expression{
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
    ExprArray(const string &varname,Expression *index);
    Object *evaluate();
    void setObject(Object *object);
    Expression *index;
};

class ExprConstant:public Expression{
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

class ExprArrayInit:public Expression{
public:
    int getExprType();
    Object *evaluate();
    vector<Expression *> initlist;
};

class ExprRange:public Expression{
public:
    int getExprType();
    Object *evaluate();
    Expression *begin;
    Expression *end;
    Expression *step;
};

class ExprMethodCall:public Expression{
public:
    ExprMethodCall(const string &methodname);
    int getExprType();
    DeclModule *enclosingmodule;
    DeclClass *enclosingclass;
    Object *evaluate();
    string methodname;
    vector<Expression *> arglist;
};

class ExprInput:public Expression{
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
    int getNodeType();
    virtual int getStmtType()=0;
    virtual void execute()=0;
    DeclMethod *enclosingmethod;
};

class StmtBlock:public Statement{
public:
    StmtBlock();
    int getStmtType();
    void execute();
    vector<Statement *> statements;
    bool continuepoint;
    bool breakpoint;
};

class StmtAssign:public Statement{
public:
    StmtAssign();
    StmtAssign(Expression *lexpr,Expression *rexpr);
    int getStmtType();
    void execute();
    Expression *lexpr;
    Expression *rexpr;
};

class StmtMethodCall:public Statement{
public:
    StmtMethodCall(ExprMethodCall *methodcall);
    int getStmtType();
    void execute();
    ExprMethodCall *methodcall;
};

class StmtIf:public Statement{
public:
    StmtIf();
    int getStmtType();
    void execute();
    Expression *condition;
    StmtBlock *ifblock;
    vector<StmtElif *> eliflist;
    StmtBlock *elseblock;
};

class StmtElif:public Statement{
public:
    StmtElif(Expression *condition,StmtBlock *elifblock);
    int getStmtType();
    void execute();
    Expression *condition;
    StmtBlock *elifblock;
    bool executed;
};

class StmtWhile:public Statement{
public:
    StmtWhile(Expression *condition,StmtBlock *whileblock);
    int getStmtType();
    void execute();
    Expression *condition;
    StmtBlock *whileblock;
};

class StmtFor:public Statement{
public:
    void execute();
    int getStmtType();
    string targetname;
    ExprRange *range;
    string objectname;
    StmtBlock *forblock;
};

class StmtReturn:public Statement{
public:
    StmtReturn(Expression *ret);
    int getStmtType();
    void execute();
    Expression *ret;
};

class StmtBreak:public Statement{
public:
    int getStmtType();
    void execute();
};

class StmtContinue:public Statement{
public:
    int getStmtType();
    void execute();
};

class StmtPrint:public Statement{
public:
    int getStmtType();
    void execute();
    vector<Expression *> printlist;
};

class StmtPrintLn:public Statement{
public:
    int getStmtType();
    void execute();
    vector<Expression *> printlist;
};

/****************************************************************/
/*************************声明类节点类定义*************************/

class Declaration:public ASTree{
public:
    int getNodeType();
    virtual int getDeclType()=0;
    virtual void intepret()=0;
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
    int getNodeType();
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
