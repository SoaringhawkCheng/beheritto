#ifndef astnode_h
#define astnode_h

enum ExprType{
：
};

/*语法树节点继承关系*/
class ASTNode;//语法树节点基类
    class Expr:public ASTNode;//表达式节点基类
        class ExprMember:public Expr;//成员运算符
        class ExprOpUnary:public Expr;//一元操作节点
            class ExprInvert:public ExprOpUnary;//求逆运算节点
            class ExprNegate:public ExprOpUnary;//取反预算节点
        class ExprOpBinary:public Expr;//二元操作节点
            class ExprArithmetic:public ExprOpBinary;//算术运算节点
            class ExprBitwise:public ExprOpBinary;//位运算节点
            class ExprCompare:public ExprOpBinary;//比较运算节点
            class ExprLogic:public ExprOpBinary;//逻辑运算节点
           // class ExprOpMember:public ExprOpBinary;//成员运算节点
        class ExprLValue:public Expr;//左值节点
            class ExprElement:public ExprLValue;//元素节点
            class ExprList:public ExprLValue;//列表节点
            class ExprDict:public ExprLValue;//字典节点
        class ExprRValue:public Expr;//右值节点
            class ExprNum:public ExprConstant;
            class ExprBoolean:public ExprConstant;
            class ExprString:public ExprConstant;
    class Statement:public ASTNode;//语句节点基类
        class StatementBlock:public Statement;//代码块
        class StatementArgumentList:public Statement;//形参列表
        class StatementAssign:public Statement;//赋值不该放在前面？
        class StatementMethodCall:public Statement;
        class StatementIf:public Statement;
        class StatementElif:public Statement;
        class StatementWhile:public Statement;
        class StatementFor:public Statement;
        class StatementReturn:public Statement;
        class StatementBreak:public Statement;
        class StatementContinue:public Statement;
        class StatementRange:public Statement;
        class StatementSlice:public Statement;
        class StatementInput:public Statement;
        class StatementPrint:public Statement;
        class StatementRead:public Statement;
        class StatementWrite:public Statement;
    class Block:public ASTNode;//代码块节点基类
    class Definition:public ASTNode;
        class DefineClass:public Definition;
        class MethodDef:public Definition;
        class FunctionDef:public Definition;
class Result;
class StackFrame;

/*语法树节点类声明*/
#endif
