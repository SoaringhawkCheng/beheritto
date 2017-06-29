#ifndef treenode_h
#define treenode_h

/****************************************************************/
/***************语法树节点继承关系***************/
class TreeNode;//语法树节点基类
    class Expression;//表达式节点基类
        class ExprOpMember;//成员运算符
        class ExprOpUnary;//一元操作节点
            class ExprInvert;//取反运算节点
            class ExprNot;//求逆预算节点
        class ExprOpBinary;//二元操作节点
            class ExprArithmetic;//算术运算节点
            class ExprBitwise;//位运算节点
            class ExprCompare;//比较运算节点
            class ExprLogic;//逻辑运算节点
           // class ExprOpMember:public ExprOpBinary;//成员运算节点
        class ExprLValue;//左值节点
            class ExprVariable;//元素节点
            class ExprArray;//列表节点
            //class ExprDict:public ExprLValue;//字典节点
        class ExprConstant;//右值节点
            class ExprNum;
            class ExprBoolean;
            class ExprString;
        class ExprMethodCall;
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
        class StmtArgList;//形参列表
        class StmtIndex;//下标索引
        class StmtBreak;
        class StmtContinue;
        class StmtExprList;
        class StmtRange;
    class Block;//代码块节点基类
    class Declaration;
        class DeclProgram;
        class DeclModule;
        class DeclClass;
        class DeclMethod;
        class DeclField;
        class DeclMain;
class NodeType;
    class Equal;
    class NotEqual;
    class Boolean;
    class String;
    class Array;
    class voidF;
    class Method;
class Result;
class StackFrame;

#endif
