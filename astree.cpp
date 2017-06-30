#include "astree.h"

extern StackFrameSlot curstackframeslot;
extern StackFrame *curstackframe;
extern DeclMethod *curmethod;
extern StmtLoop *curloop;
extern string curmodname;
extern int curline;

ASTree::ASTree():line(curline),modname(curmodname){}

Expr::Expr():stackframe(curstackframe){}

ExprOpUnary::ExprOpUnary(Expr *expr):expr(expr){}

ExprOpposite::ExprOpposite(Expr *expr):ExprOpUnary(expr){}

//Type *ExprOpposite::analyzeSemantic(){

//}

ExprOpBinary::ExprOpBinary(const string &opname,Expr *lexpr,Expr *rexpr)
    :opname(opname),lexpr(lexpr),rexpr(rexpr){}

//int ExprOpBinary::getExprType(){
//    return ExprType::OPBIN;
//}

ExprArith::ExprArith(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Type *ExprArith::analyzeSemantic(){
    
}

ExprCompare::ExprCompare(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}


Type *ExprCompare::analyzeSemantic(){
//    Type *ltype=lexpr->analyzeSemantic();
//    Type *rtype=rexpr->analyzeSemantic();
//    if(ltype->getType()==Types::NUM&&rtype->getType()==Types::NUM)
//        return new Boolean();
//    else if(ltype->getType()==Types::VAR||rtype->getType()==Types::VAR){
//        if(ltype->getType()==Types::VAR){
//            if(ltype->getType()==ExprType::LVALUE){
//                ExprLValue *lexprlvalue=dynamic_cast<ExprLValue *>(lexpr);
//                lexprlvalue->setType(new Var());
//            }
//            else throw SemanticError(modname, line);
//        }
//        else if(rtype->getType()==Types::VAR){
//            if(rtype->getType()==ExprType::LVALUE){
//                ExprLValue *rexprlvalue=dynamic_cast<ExprLValue *>(rexpr);
//                rexprlvalue->setType(new Var());
//            }
//            else throw SemanticError(modname, line);
//        }
//        return new Boolean();
//    }
//    else throw SemanticError(modname, line);
}

Result *ExprCompare::evaluate(){
    //ResFloat *lfloat=dynamic_cast<ResFloat *>(lexpr->evaluate());
    //ResFloat *rfloat=dynamic_cast<ResFloat *>(rexpr->evaluate());
    if(opname=="=="){}
    if(opname=="!="){}
    if(opname==">"){}
    if(opname==">="){}
    if(opname=="<"){}
    if(opname=="<="){}
    return new ResBoolean()
}




/****************************************************************/
/***************类型类节点类定义***************/

Variable::Variable(string varname,Result *result):varname(varname),result(result){}

ResInt::ResInt(int value):value(value){}
int ResInt::getType(){return ResType::RESNUM;}
Result *ResInt::getValue(){return new ResInt(value);}

ResBoolean::ResBoolean(bool value):value(value){}
int ResBoolean::getType(){return ResType::RESBOOLEAN;}
Result *ResBoolean::getValue(){return new ResBoolean(value);}

ResArray::ResArray(){}
int ResArray::getType(){return ResType::RESARRAY;}
Result *ResArray::getValue(){
    ResArray *arr=new ResArray();
    arr->value=value;
}

ResString::ResString(string value):value(value){}
int ResString::getType(){return ResType::RESSTRING;}
Result *ResString::getValue(){return new ResString(value);}


