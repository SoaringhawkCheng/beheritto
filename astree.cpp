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

double getNumeric(Result *result){
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

/****************************************************************/
/***************一元运算符节点类定义***************/

ExprOpUnary::ExprOpUnary(Expr *expr):expr(expr){}

ExprOpposite::ExprOpposite(Expr *expr):ExprOpUnary(expr){}

Type *ExprOpposite::analyzeSemantic(){
    return NULL;
}

Result *ExprOpposite::evaluate(){
    Result *res=expr->evaluate();
    if(res->getNodeType()==NodeType::_INTEGER){
        ResInteger *resinteger=dynamic_cast<ResInteger *>(res);
        return new ResInteger(-1*resinteger->value);
    }
    if(res->getNodeType()==NodeType::_FLOAT){
        ResFloat *resinteger=dynamic_cast<ResFloat *>(res);
        return new ResFloat(-1*resinteger->value);
    }
    return NULL;
}

//Type *ExprOpposite::analyzeSemantic(){

//}

/****************************************************************/
/***************二元运算节点类定义***************/

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
    //if(ltype->getNodeType()==NodeType::_ID){
    
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
/***************左值变量节点类定义***************/

ExprLValue::ExprLValue(const string &varname):varname(varname){}

ExprID::ExprID(const string &varname):ExprLValue(varname){}

Type *ExprID::analyzeSemantic(){
    Type *type=stackframe->get(varname);
    return type;
}

void ExprID::setType(Type *type){
    
}



ExprArray::ExprArray(const string &varname,Expr *index):ExprLValue(varname),index(index){}



/****************************************************************/
/***************常量运算节点类定义***************/

ExprInteger::ExprInteger(int value):value(value){}

Type *ExprInteger::analyzeSemantic(){
    return new TypeInteger();
}

Result *ExprInteger::evaluate(){
    return new ResInteger(value);
}

ExprFloat::ExprFloat(double value):value(value){}

Type *ExprFloat::analyzeSemantic(){
    return new TypeFloat();
}

Result *ExprFloat::evaluate(){
    return new ResFloat(value);
}

ExprBoolean::ExprBoolean(bool value):value(value){}

Type *ExprBoolean::analyzeSemantic(){
    return new TypeBoolean();
}

Result *ExprBoolean::evaluate(){
    return new ResBoolean(value);
}

ExprString::ExprString(const string &value):value(value){}

Type *ExprString::analyzeSemantic(){
    return new TypeString();
}

Result *ExprString::evaluate(){
    return new ResString(value);
}

Type *ExprArrayInit::analyzeSemantic(){
    Type *type=initlist[0]->analyzeSemantic();
    TypeArray *typearray=new TypeArray();
    return NULL;
}

Result *ExprArrayInit::evaluate(){
    ResArray *resarray=new ResArray();
    for(int i=0;i<initlist.size();++i)
        resarray->value.push_back(initlist[i]->evaluate());
    return resarray;
}

/****************************************************************/
/***************函数调用节点类定义***************/

ExprMethodCall::ExprMethodCall(const string &methodname):methodname(methodname){}

Type *ExprMethodCall::analyzeSemantic(){
    Type *type=stackframe->get(methodname);
}

/****************************************************************/
/***************语句节点类定义***************/



/****************************************************************/
/***************声明节点类定义***************/

Declaration::Declaration():stackframe(curstackframe){}

DeclModule::DeclModule(const string &modname):modname(modname){}

void DeclModule::analyzeSemantic(){
    for(int i=0;i<modulelist.size();++i)
        modulelist[i]->analyzeSemantic();
    for(int i=0;i<classlist.size();++i)
        classlist[i]->analyzeSemantic();
    for(int i=0;i<methodlist.size();++i)
        methodlist[i]->analyzeSemantic();
    if(entry) entry->analyzeSemantic();
    else throw SemanticError(curmodname, curline);
}

void DeclModule::intepret(){
    entry->intepret();
}

DeclMethod::DeclMethod(const string &methodname):methodname(methodname){}

void DeclMethod::analyzeSemantic(){
    TypeMethod *typemethod=new TypeMethod();
    for(int i=0;i<paralist.size();++i){
        if(typemethod->paramap.at(paralist[i])==0){
            //typemethod->paramap[paralist[i]]=new
        }
        else throw SemanticError(modname, line);
    }
    stackframe->put(methodname, typemethod);
    methodblock->analyzeSemantic();
}

void DeclMethod::intepret(){
    methodblock->execute();
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


