#ifndef treenode_h
#define treenode_h

/****************************************************************/
/***************语法树节点继承关系***************/
class ASTree;//语法树节点基类
    class Expression;//表达式节点基类
        class ExprOpMember;//成员运算符
        class ExprOpUnary;//一元操作节点
            class ExprOpposite;//取反运算节点
            class ExprNot;//求逆预算节点
        class ExprOpBinary;//二元操作节点
            class ExprArith;//算术运算节点
            class ExprBitwise;//位运算节点
            class ExprCompare;//比较运算节点
            class ExprLogic;//逻辑运算节点
           // class ExprOpMember:public ExprOpBinary;//成员运算节点
        class ExprLValue;//左值节点
            class ExprID;//元素节点
            class ExprArray;//数组节点
            //class ExprDict:public ExprVariable;//字典节点
        class ExprConstant;//右值节点
            class ExprInteger;
            class ExprFloat;
            class ExprBoolean;
            class ExprString;
            class ExprArrayInit;
            class ExprRange;
        class ExprMethodCall;
        class ExprInput;
    class Statement;//语句节点基类
        class StmtBlock;//代码块
        class StmtAssign;//赋值不该放在前面？
        class StmtIf;
        class StmtElif;
        class StmtElse;
        class StmtLoop;
            class StmtWhile;
            class StmtFor;
        class StmtReturn;
        class StmtInput;
        class StmtPrint;
        class StmtPrintLn;
        class StmtBreak;
        class StmtContinue;
    class Declaration;
        class DeclModule;
        class DeclClass;
        class DeclMethod;
        class DeclField;
        class DeclEntry;
class Object;
    class ObjInteger;
    class ObjFloat;
    class ObjBoolean;
    class ObjString;
    class ObjArray;
    class ObjClass;
class SymbolTable;
class RuntimeStack;
class StackFrame;
class Variable;

/****************************************************************/
/***************节点类型枚举类***************/

enum ExprType{
    EXPROPUN,EXROPBIN,EXPRLVALUE,EXPRCONSTANT,EXPRMETHODCALL
};

enum DeclType{
    DECLMODULE,DECLCLASS,DECLMETHOD,DECLENTRY,DECLFIELD
};

enum ObjType{
    OBJINTEGER,OBJFLOAT,OBJBOOLEAN,OBJSTRING,OBJARRAY,OBJCLASS
};

/*
enum ObjectType{
    OBJINTEGER,_FLOAT,BOOLEAN,_OBJSTRING
};*/

#endif
