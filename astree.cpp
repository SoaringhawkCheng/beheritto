#include "astree.h"

extern StackFrame *curstackframe;
//extern DeclMethod

Expr::Expr():stackframe(curstackframe){}

ExprOpUnary::ExprOpUnary(Expr *expr):expr(expr){}

ExprOpposite::ExprOpposite(Expr *expr):ExprOpUnary(expr){}

Type *ExprOpposite::analyzeSemantic(){
//    Type *type=expr->analyzeSemantic();
//    if();
//    else if;
//    else;
}

ExprOpBinary::ExprOpBinary(const string &opname,Expr *lexpr,Expr *rexpr)
    :opname(opname),lexpr(lexpr),rexpr(rexpr){}

//int ExprOpBinary::getExprType(){
//    return ExprType::OPBIN;
//}

ExprCompare::ExprCompare(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Type *ExprCompare::analyzeSemantic(){
    Type *ltype=lexpr->analyzeSemantic();
    Type *rtype=rexpr->analyzeSemantic();
    if(ltype->getType()==NodeType::VAR)
}
