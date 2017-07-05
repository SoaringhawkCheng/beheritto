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
extern DeclMethod *curmethod;
extern StmtLoop *curloop;
extern string curmodname;
extern int curline;
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
    //virtual int getSymType()=0;
    int line;
    string enclosingmodule;
};

/****************************************************************/
/*************************运算类节点类定义*************************/

class Expr:public ASTree{
public:
    virtual int getExprType()=0;
//    virtual Type *analyzeSemantic()=0;
    virtual Object *evaluate()=0;
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
//    Type *analyzeSemantic();
    Object *evaluate();
};
class ExprNot:public ExprOpUnary{
public:
    ExprNot(Expr *expr);
    //string toString()();
//    Type *analyzeSemantic();
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
    //string toString()();
//    Type *analyzeSemantic();
    Object *evaluate();
};

class ExprBitwise:public ExprOpBinary{
public:
    ExprBitwise(const string &opname,Expr *lexpr,Expr *rexpr);
    //string toString()();
//    Type *analyzeSemantic();
    Object *evaluate();
};

class ExprCompare:public ExprOpBinary{
public:
    ExprCompare(const string &opname,Expr *lexpr,Expr *rexpr);
    //string toString()();
//    Type *analyzeSemantic();
    Object *evaluate();
};

class ExprLogic:public ExprOpBinary{
public:
    ExprLogic(const string &opname,Expr *lexpr,Expr *rexpr);
    //string toString()();
//    Type *analyzeSemantic();
    Object *evaluate();
};

class ExprLValue:public Expr{
public:
    ExprLValue(const string &varname);
    int getExprType();
//    virtual void setType(Type *type)=0;
    virtual void setObject(Object *object)=0;
    string varname;
    DeclMethod *enclosingmethod;
    //DeclClass *enclosingclass;
    //DeclModule *enclosingmodule;
};

class ExprID:public ExprLValue{
public:
    ExprID(const string &varname);
    //string toString()();
//    Type *analyzeSemantic();
    Object *evaluate();
//    void setType(Type *type);
    void setObject(Object *object);
};

class ExprArray:public ExprLValue{
public:
    ExprArray(const string &varname,Expr *index);
    //string toString()();
//    Type *analyzeSemantic();
    Object *evaluate();
//    void setType(Type *type);
    void setObject(Object *object);
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
//    Type *analyzeSemantic();
    Object *evaluate();
    int value;
};

class ExprFloat:public ExprConstant{
public:
    ExprFloat(double value);
    //string toString()();
//    Type *analyzeSemantic();
    Object *evaluate();
    double value;
};

class ExprBoolean:public ExprConstant{
public:
    ExprBoolean(bool value);
    //string toString()();
//    Type *analyzeSemantic();
    Object *evaluate();
    bool value;
};

class ExprString:public ExprConstant{
public:
    ExprString(const string &value);
    //string toString()();
//    Type *analyzeSemantic();
    Object *evaluate();
    string value;
};

class ExprArrayInit:public ExprConstant{
public:
    //string toString()();
//    Type *analyzeSemantic();
    Object *evaluate();
    vector<Expr *> initlist;
};
/*
class ExprCondition:public Expr{
public:
    ExprCondition();
    //string toString()();
    Type *analyzeSemantic();
    int getExprType();
    Object *evaluate();
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
//    Type *analyzeSemantic();
    Object *evaluate();
    string methodname;
    vector<Expr *> arglist;
};

class ExprInput:public Expr{
public:
    ExprInput(const string &tip);
    //string toString()();
    //    void analyzeSemantic();
    string tip;
    int getExprType();
    Object *evaluate();
};

/****************************************************************/
/*************************语句类节点类定义*************************/

class Statement:public ASTree{
public:
    Statement();
    virtual void execute()=0;
//    virtual void analyzeSemantic()=0;
    DeclMethod *enclosingmethod;
};

class StmtBlock:public Statement{
public:
    StmtBlock();
    //string toString()();
//    void analyzeSemantic();
    void execute();
    vector<Statement *> statements;
    bool continuepoint;
    bool breakpoint;
};

class StmtAssign:public Statement{
public:
    StmtAssign();
    StmtAssign(Expr *lexpr,Expr *rexpr);
    //string toString()();
//    void analyzeSemantic();
    void execute();
    Expr *lexpr;
    Expr *rexpr;
};

class StmtMethodCall:public Statement{
public:
    StmtMethodCall(ExprMethodCall *methodcall);
    //string toString()();
//    void analyzeSemantic();
    void execute();
    ExprMethodCall *methodcall;
};

class StmtIf:public Statement{
public:
    StmtIf();
    //string toString()();
//    void analyzeSemantic();
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
//    void analyzeSemantic();
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
//    void analyzeSemantic();
    void execute();
    Expr *condition;
    StmtBlock *whileblock;
};

class StmtFor:public Statement{
public:
    StmtFor();
    //string toString()();
//    void analyzeSemantic();
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
//    void analyzeSemantic();
    void execute();
    Expr *ret;
};

class StmtBreak:public Statement{
public:
    StmtBreak();
    //string toString()();
//    void analyzeSemantic();
    void execute();
    StmtLoop *enclosingloop;
};

class StmtContinue:public Statement{
public:
    StmtContinue();
    //string toString()();
//    void analyzeSemantic();
    void execute();
    StmtLoop *enclosingloop;
};

class StmtPrint:public Statement{
public:
    StmtPrint();
    //string toString()();
//    void analyzeSemantic();
    void execute();
    vector<Expr *> printlist;
};

class StmtRange:public Statement{
public:
    StmtRange();
    //string toString()();
//    void analyzeSemantic();
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
//    virtual void analyzeSemantic()=0;
    virtual void intepret()=0;
    virtual int getDeclType()=0;
};

class DeclModule:public Declaration{
public:
    DeclModule(const string &modname);
    ~DeclModule();
    int getDeclType();
    //string toString()();
//    void analyzeSemantic();
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
    ~DeclClass();
    int getDeclType();
    //string toString()();
//    void analyzeSemantic();
    void intepret();
    string classname;
    StmtBlock *classblock;
    vector<string> paralist;
    unordered_map<string, DeclMethod *> methodlist;
    vector<DeclField *> fieldlist;
};

class DeclMethod:public Declaration{
public:
    DeclMethod(const string &methodname);
    ~DeclMethod();
    int getDeclType();
    
    void intepret();
    string methodname;
    vector<string> paralist;
    StmtBlock *methodblock;
    Object *returnobj;
};

class DeclField:public Declaration{
public:
    DeclField(StmtAssign *assign);
    ~DeclField();
    int getDeclType();
    //string toString()();
//    void analyzeSemantic();
    void intepret();
    StmtAssign *assign;
};

class DeclEntry:public Declaration{
public:
    DeclEntry();
    ~DeclEntry();
    int getDeclType();
    //string toString()();
//    void analyzeSemantic();
    void intepret();
    vector<Statement *> statements;
};

/****************************************************************/
/*************************类型类节点类定义*************************/

//class Type {
//public:
//    virtual int getObjType()=0;
//    virtual bool isEquivalent(Type *type)=0;
//};
//
//class TypeID:public Type{
//public:
//    int getObjType();
//    bool isEquivalent(Type *type);
//};
//
//class TypeWildcard:public Type{
//public:
//    int getObjType();
//    bool isEquivalent(Type *type);
//};
//
//class TypeScalar:public Type{
//public:
//    int getObjType();
//    bool isEquivalent(Type *type);
//};
//
//class TypeArray:public Type{
//public:
//    int getObjType();
//    bool isEquivalent(Type *type);
//    int size;
//    Type *arraytype;
//};
//
//class TypeInteger:public Type{
//public:
//    int getObjType();
//    bool isEquivalent(Type *type);
//};
//
//class TypeFloat:public Type{
//public:
//    int getObjType();
//    bool isEquivalent(Type *type);
//};
//
//class TypeBoolean:public Type{
//public:
//    int getObjType();
//    bool isEquivalent(Type *type);
//};
//
//class TypeString:public Type{
//public:
//    int getObjType();
//    bool isEquivalent(Type *type);
//};
//
//class TypeVoid:public Type{
//public:
//    int getObjType();
//    bool isEquivalent(Type *type);
//};
//
//class TypeMethod:public Type{
//public:
//    int getObjType();
//    bool isEquivalent(Type *type);
//    Type * returntype;
//    unordered_map<string,Type *> paramap;
//};
//
//class TypeClass:public Type{
//public:
//    int getObjType();
//    bool isEquivalent(Type *type);
//    vector<string> paralist;
//    unordered_map<string, Type *> fieldmap;
//    unordered_map<string, Type *> methodmap;
//};
//
//class TypeModule:public Type{
//public:
//    int getObjType();
//    bool isEquivalent(Type *type);
//    unordered_map<string, Type *> modulemap;
//    unordered_map<string, Type *> classmap;
//    unordered_map<string, Type *> methodmap;
//};

/****************************************************************/
/*************************运算结果节点类定义*************************/

class Object:public ASTree{
public:
    Object();
    virtual int getObjType()=0;
    virtual Object *getValue()=0;
    virtual void print()=0;
};

class ObjInteger:public Object{
public:
    ObjInteger(int value);
    int getObjType();
    Object *getValue();
    void print();
    int value;
};

class ObjFloat:public Object{
public:
    ObjFloat(int value);
    int getObjType();
    Object *getValue();
    void print();
    double value;
};

class ObjBoolean:public Object{
public:
    ObjBoolean(bool value);
    int getObjType();
    Object *getValue();
    void print();
    bool value;
};

class ObjString:public Object{
public:
    ObjString(string value);
    int getObjType();
    Object *getValue();
    void print();
    string value;
};

class ObjArray:public Object{
public:
    ObjArray();
    int getObjType();
    Object *getValue();
    void print();
    vector<Object *> value;
};

class ObjClass:public Object{
public:
    int getObjType();
    Object *getValue();
    void print();
    vector<string> paralist;
    unordered_map<string, Object *> fieldmap;
    unordered_map<string, DeclMethod *> methodmap;
};

/****************************************************************/
/*************************环境变量节点类定义*************************/

class SymbolTable{
public:
    int getDeclType(const string &key);
    DeclClass *getDeclClass(const string &key);
    DeclMethod *getDeclMethod(const string &key);
    DeclModule *program;
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
    Variable(const string &varname,Object *object);
    string varname;
    Object *object;
};

class Procedure{
public:
    Procedure(DeclMethod *method);
    DeclMethod *method;
};

#endif
