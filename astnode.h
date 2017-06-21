#ifndef astnode_h
#define astnode_h

enum ExprType{
：
};

/*语法树节点*/

/*语法树节点继承关系*/
class ASTNode;//语法树节点基类
    class Expr:public ASTNode;//表达式节点基类
        class ExprOpMember:public Expr;//成员运算符
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
            class ExprVariable:public ExprLValue;//元素节点
            class ExprArray:public ExprLValue;//列表节点
            //class ExprDict:public ExprLValue;//字典节点
        class ExprRValue:public Expr;//右值节点
            class ExprNum:public ExprConstant;
            class ExprBoolean:public ExprConstant;
            class ExprString:public ExprConstant;
        class ExprMethodCall:public ExprLValue
    class Stmt:public ASTNode;//语句节点基类
        class StmtBlock:public Stmt;//代码块
        class StmtAssignment:public Stmt;//赋值不该放在前面？
        class StmtIf:public Stmt;
        class StmtElif:public Stmt;
        class StmtElse:public Stmt;
        class StmtWhile:public Stmt;
        class StmtFor:public Stmt;
        class StmtReturn:public Stmt;
        class StmtInput:public Stmt;
        class StmtPrint:public Stmt;
        class StmtArgList:public Stmt;//形参列表
        class StmtSlice:public Stmt;//切片
        class StmtIndex:public Stmt;下标索引
        class StmtBreak:public Stmt;
        class StmtContinue:public Stmt;
        class StmtRange:public Stmt;
    class Block:public ASTNode;//代码块节点基类
    class Definition:public ASTNode;
        class DeclClass:public Definition;
        class MethodDef:public Definition;
        class DeclMethod:public Definition;
class
class Result;
class StackFrame;
/*语法树节点类声明*/
#endif
