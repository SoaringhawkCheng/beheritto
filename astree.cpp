#include "astree.h"

/****************************************************************/
/***************外部变量引用声明***************/

extern StackFrameSlot curstackframeslot;
extern StackFrame *curstackframe;
extern DeclMethod *curmethod;
extern StmtLoop *curloop;
extern string curmodname;
extern int curline;

/****************************************************************/
/***************全局静态函数定义***************/

bool isNumeric(Type *type){
    switch(type->getNodeType()){
    case NodeType::_INTEGER:
    case NodeType::_FLOAT:
    case NodeType::_STRING:
        return true;
    default:
        return false;
    }
}

float getNumeric(Result *result){
    if(result->getNodeType()==NodeType::_INTEGER){
        ResInteger *res=dynamic_cast<ResInteger *>(result->getValue());
        return res->value;
    }
    if(result->getNodeType()==NodeType::_FLOAT){
        ResFloat *res=dynamic_cast<ResFloat *>(result->getValue());
        return res->value;
    }
    else{//if(result->getNodeType()==NodeType::BOOLEAN)
        ResBoolean *res=dynamic_cast<ResBoolean *>(result->getValue());
        return res->value;
    }
}

int getInteger(Result *result){
    ResInteger *resint=dynamic_cast<ResInteger *>(result->getValue());
    return resint->value;
}

int getFloat(Result *result){
    ResFloat *resfloat=dynamic_cast<ResFloat *>(result->getValue());
    return resfloat->value;
}

bool getBoolean(Result *result){
    ResBoolean *resboolean=dynamic_cast<ResBoolean *>(result->getValue());
    return resboolean->value;
}

/****************************************************************/
/***************表达式节点类定义***************/

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
    return NULL;
}

Result *ExprArith::evaluate(){
    Result *lres=lexpr->evaluate();
    Result *rres=rexpr->evaluate();
    NodeType restype;
    if(lres->getNodeType()==NodeType::_FLOAT||rres->getNodeType()==NodeType::_FLOAT)
        restype=NodeType::_FLOAT;
    else restype=NodeType::_INTEGER;
    if(opname=="+"){
        if(restype==NodeType::_FLOAT)
            return new ResFloat(getNumeric(lres)+getNumeric(rres));
        else
            return new ResInteger(getNumeric(lres)+getNumeric(rres));
    }
    if(opname=="-"){
        if(restype==NodeType::_FLOAT)
            return new ResFloat(getNumeric(lres)-getNumeric(rres));
        else
            return new ResInteger(getNumeric(lres)-getNumeric(rres));
    }
    if(opname=="*"){
        if(restype==NodeType::_FLOAT)
            return new ResFloat(getNumeric(lres)*getNumeric(rres));
        else
            return new ResInteger(getNumeric(lres)*getNumeric(rres));
    }
    if(opname=="/"){
        if(restype==NodeType::_FLOAT)
            return new ResFloat(getNumeric(lres)/getNumeric(rres));
        else
            return new ResInteger(getNumeric(lres)/getNumeric(rres));
    }
    if(opname=="%")
        return new ResInteger(getInteger(lres)%getInteger(rres));
    else
        return NULL;
}

ExprBitwise::ExprBitwise(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Type *ExprBitwise::analyzeSemantic(){return NULL;}

Result *ExprBitwise::evaluate(){
    int lnum=getInteger(lexpr->evaluate());
    int rnum=getInteger(lexpr->evaluate());
    if(rnum<0) throw ExecutiveError(modname, line);
    if(opname=="<<")
        return new ResInteger(lnum<<rnum);
    if(opname==">>")
        return new ResInteger(lnum>>rnum);
    return NULL;
}


ExprCompare::ExprCompare(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}


Type *ExprCompare::analyzeSemantic(){
    Type *ltype=lexpr->analyzeSemantic();
    Type *rtype=rexpr->analyzeSemantic();
    if(isNumeric(ltype)&&isNumeric(rtype)) return new TypeBoolean();
    //if(ltype->getNodeType()==NodeType::SCALAR){
    
    //}
    return nullptr;
}

Result *ExprCompare::evaluate(){
    Result *lres=lexpr->evaluate();
    Result *rres=rexpr->evaluate();
    if(opname=="==")
        return new ResBoolean(getNumeric(lres)==getNumeric(rres));
    if(opname=="!=")
        return new ResBoolean(getNumeric(lres)!=getNumeric(rres));
    if(opname==">")
        return new ResBoolean(getNumeric(lres)>getNumeric(rres));
    if(opname==">=")
        return new ResBoolean(getNumeric(lres)>=getNumeric(rres));
    if(opname=="<")
        return new ResBoolean(getNumeric(lres)<getNumeric(rres));
    if(opname=="<=")
        return new ResBoolean(getNumeric(lres)<=getNumeric(rres));
    else return NULL;
}

ExprLogic::ExprLogic(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Type *ExprLogic::analyzeSemantic(){return NULL;}

Result *ExprLogic::evaluate(){
    Result *lres=lexpr->evaluate();
    Result *rres=rexpr->evaluate();
    if(opname=="and")
        return new ResBoolean(getNumeric(lres)&&getNumeric(rres));
    if(opname=="or")
        return new ResBoolean(getNumeric(lres)||getNumeric(rres));
    return NULL;
}

/****************************************************************/
/***************类型类节点类定义***************/



/****************************************************************/
/***************运算结果节点类定义***************/

Variable::Variable(const string &varname,Result *result):varname(varname),result(result){}

ResInteger::ResInteger(int value):value(value){}
int ResInteger::getNodeType(){return NodeType::_INTEGER;}
Result *ResInteger::getValue(){return new ResInteger(value);}

ResFloat::ResFloat(int value):value(value){}
int ResFloat::getNodeType(){return NodeType::_FLOAT;}
Result *ResFloat::getValue(){return new ResFloat(value);}

ResBoolean::ResBoolean(bool value):value(value){}
int ResBoolean::getNodeType(){return NodeType::BOOLEAN;}
Result *ResBoolean::getValue(){return new ResBoolean(value);}

ResString::ResString(string value):value(value){}
int ResString::getNodeType(){return NodeType::_STRING;}
Result *ResString::getValue(){return new ResString(value);}

ResArray::ResArray(){}
int ResArray::getNodeType(){return NodeType::ARRAY;}
Result *ResArray::getValue(){
    ResArray *arr=new ResArray();
    arr->value=value;
}


