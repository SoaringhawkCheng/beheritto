#ifndef astnode_h
#define astnode_h

enum ExprType{
：
};

/*语法树节点*/

/*语法树节点继承关系*/
class ASTNode;//语法树节点基类
    class Expression:public ASTNode;//表达式节点基类
        class ExprOpMember:public Expression;//成员运算符
        class ExprOpUnary:public Expression;//一元操作节点
            class ExprInvert:public ExprOpUnary;//求逆运算节点
            class ExprNegate:public ExprOpUnary;//取反预算节点
        class ExprOpBinary:public Expression;//二元操作节点
            class ExprArithmetic:public ExprOpBinary;//算术运算节点
            class ExprBitwise:public ExprOpBinary;//位运算节点
            class ExprCompare:public ExprOpBinary;//比较运算节点
            class ExprLogic:public ExprOpBinary;//逻辑运算节点
           // class ExprOpMember:public ExprOpBinary;//成员运算节点
        class ExprLValue:public Expression;//左值节点
            class ExprVariable:public ExprLValue;//元素节点
            class ExprArray:public ExprLValue;//列表节点
            //class ExprDict:public ExprLValue;//字典节点
        class ExprRValue:public Expression;//右值节点
            class ExprNum:public ExprConstant;
            class ExprBoolean:public ExprConstant;
            class ExprString:public ExprConstant;
        class ExprMethodCall:public ExprLValue
    class Statement:public ASTNode;//语句节点基类
        class StmtBlock:public Statement;//代码块
        class StmtAssignment:public Statement;//赋值不该放在前面？
        class StmtIf:public Statement;
        class StmtElif:public Statement;
        class StmtElse:public Statement;
        class StmtLoop:public Statement;
            class StmtWhile:public StmtIteration;
            class StmtFor:public StmtIteration;
        class StmtReturn:public Statement;
        class StmtInput:public Statement;
        class StmtPrint:public Statement;
        class StmtArgList:public Statement;//形参列表
        class StmtSlice:public Statement;//切片
        class StmtIndex:public Statement;//下标索引
        class StmtBreak:public Statement;
        class StmtContinue:public Statement;
        class StmtRange:public Statement;
    class Block:public ASTNode;//代码块节点基类
    class Declaration:public ASTNode;
        class DeclModule:public Declaration;
        class DeclProgram:public Declartion;
        class DeclClass:public Declaration;
        class DeclMethod:public Declaration;
        class DeclField:public Declaration;
        class DeclFunction:public Declaration;
        class DeclMain:public Declaration;

class
class Result;
class StackFrame;
/*语法树节点类声明*/
#endif
