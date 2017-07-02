#include "astree.h"

/****************************************************************/
/*************************外部变量引用声明*************************/

extern unordered_map<string,Procedure *>procedures;
extern StackFrame stackframe;
extern SymbolTable *symboltable;
extern DeclMethod *curmethod;
extern StmtLoop *curloop;
extern string curmodname;
extern int curline;

/****************************************************************/
/*************************全局静态函数定义*************************/
/*
bool isNumeric(Type *type){
    switch(type->getNodeType()){
    case NodeType::_INTEGER:
    case NodeType::_FLOAT:
    case NodeType::BOOLEAN:
        return true;
    default:
        return false;
    }
}*/

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
/*************************表达式节点类定义*************************/

ASTree::ASTree():line(curline),modname(curmodname){}

Expr::Expr():symboltable(symboltable){}

/****************************************************************/
/*************************一元运算符节点类定义*************************/

ExprOpUnary::ExprOpUnary(Expr *expr):expr(expr){}

int ExprOpUnary::getExprType(){return ExprType::OPBIN;}

ExprOpposite::ExprOpposite(Expr *expr):ExprOpUnary(expr){}

Type *ExprOpposite::analyzeSemantic(){
    Type *type=expr->analyzeSemantic();
    if(type->getNodeType()==NodeType::NUM)
        return type;
    else if(type->getNodeType()==NodeType::UNKNOWN)
        return new TypeUnknown();
    else
        throw SemanticError(modname, line);
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
    if(res->getNodeType()==NodeType::BOOLEAN){
        ResBoolean *resboolean=dynamic_cast<ResBoolean *>(res);
        return new ResBoolean(-1*resboolean->value);
    }
    throw ExecutiveError(modname, line);
}

ExprNot::ExprNot(Expr *expr):ExprOpUnary(expr){}

Type *ExprNot::analyzeSemantic(){
    Type *type=expr->analyzeSemantic();
    if(type->getNodeType()==NodeType::NUM)
        return type;
    else if(type->getNodeType()==NodeType::UNKNOWN)
        return new TypeUnknown();
    else
        throw SemanticError(modname, line);
}

Result *ExprNot::evaluate(){
    Result *res=expr->evaluate();
    return new ResBoolean(getNumeric(res)!=0);
}

/****************************************************************/
/*************************二元运算节点类定义*************************/

ExprOpBinary::ExprOpBinary(const string &opname,Expr *lexpr,Expr *rexpr)
    :opname(opname),lexpr(lexpr),rexpr(rexpr){}

int ExprOpBinary::getExprType(){
    return ExprType::OPBIN;
}

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
/*************************左值变量节点类定义*************************/

ExprLValue::ExprLValue(const string &varname):varname(varname){}

int ExprLValue::getExprType(){return ExprType::LVALUE;}

ExprID::ExprID(const string &varname):ExprLValue(varname){}

Type *ExprID::analyzeSemantic(){
    Type *type=symboltable->get(varname);
    return type;
}

void ExprID::setType(Type *type){
//    if(enclosingMethod)
//    if(symboltable->exists(varname))
//        symboltable->set(varname, type);
}

Result *ExprID::evaluate(){
    Result *result=stackframe->get(varname)->value;
}

void ExprID::setType(Type *type){
    
}

void ExprID::setResult(Result *result){
    if(stackframe->exists(varname)){
        Variable *variable=stackframe->get(varname);
        variable->value=result;
    }
    else{
        Variable *variable=new Variable(varname,result);
        stackframe->put(varname, variable);
    }
}

ExprArray::ExprArray(const string &varname,Expr *index):ExprLValue(varname),index(index){}

Type *ExprArray::analyzeSemantic(){
    Type *type=symboltable->get(varname);
    if(type->getNodeType()==NodeType::ARRAY){
        TypeArray *typearray=dynamic_cast<TypeArray *>(type);
        if(index->analyzeSemantic()->isEquivalent(new TypeInteger()))
           return typearray->arraytype;
        else
            throw SemanticError(curmodname, curline);
    }
    else
        throw SemanticError(curmodname, curline);
}

Result *ExprArray::evaluate(){
    Result *result=stackframe->get(varname)->value;
    ResArray *resarray=dynamic_cast<ResArray *>(result);
    ResInteger *resinteger=dynamic_cast<ResInteger *>(index->evaluate());
    if(resinteger->value<resarray->value.size()
       &&resinteger->value>=0)
        return resarray->value[resinteger->value];
    else
        throw SemanticError(curmodname, curline);
}

void ExprArray::setType(Type *type){
    
}

void ExprArray::setResult(Result *result){
    if(stackframe->exists(varname)){
        Variable *variable=stackframe->get(varname);
        ResArray *resarray=dynamic_cast<ResArray *>(variable->value);
        ResInteger *resinteger=dynamic_cast<ResInteger *>(variable->value);
        resarray->value[resinteger->value]=result;
    }
    else
        throw SemanticError(curmodname, curline);
}

/****************************************************************/
/*************************常量运算节点类定义*************************/

int ExprConstant::getExprType(){return ExprType::CONST;}

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
/*************************函数调用节点类定义*************************/

ExprMethodCall::ExprMethodCall(const string &methodname):methodname(methodname){}

Type *ExprMethodCall::analyzeSemantic(){
    Type *type=symboltable->get(methodname);
    if(type->getNodeType()==NodeType::METHOD){
        TypeMethod *typemethod=dynamic_cast<TypeMethod *>(type);
        if(arglist.size()==typemethod->paramap.size()){
            int index=0;
            for(auto iter=typemethod->paramap.begin();iter!=typemethod->paramap.end();++iter){
                Type *argtype=arglist[index]->analyzeSemantic();
                Type *paratype=iter->second;
                //
                //
            }
        }
    }
}

Result *ExprMethodCall::evaluate(){
    /*通过调用函数名寻找函数定义，未来这块要修改*/
    Procedure *proc=procedures[methodname];
    SymbolTableVariables *envvar=new SymbolTableVariables();
    for(int i=0;i<arglist.size();++i){
        Result *result=arglist[i]->evaluate();
        Variable *variable=new Variable(proc->todo->paralist[i],result);//参数传递
        envvar->variabletable[proc->todo->paralist[i]]=variable;
    }
    stackframe->push(envvar);
    proc->todo->methodblock->execute();
    stackframe->pop();
    
    Result *resreturn=proc->todo->resreturn;
    proc->todo->resreturn=NULL;//??????????
    return resreturn;
}

/****************************************************************/
/*************************语句节点类定义*************************/

Statement::Statement():enclosingMethod(curmethod){}

StmtBlock::StmtBlock(){
    symboltable=symboltable;
    _break=_continue=false;
}

void StmtBlock::analyzeSemantic(){
    for(int i=0;i<statements.size();++i)
        statements[i]->analyzeSemantic();
}

void StmtBlock::execute(){
    for(int i=0;i<statements.size();++i){
        if(enclosingMethod->resreturn!=NULL) break;
        statements[i]->execute();
        if(_break==true) break;
        else if(_continue==true){
            _continue=false;
            continue;
        }
    }
}

StmtAssign::StmtAssign(Expr *lexpr,Expr *rexpr):lexpr(lexpr),rexpr(rexpr){}

void StmtAssign::analyzeSemantic(){
    
}

void StmtAssign::execute(){
    ExprLValue *exprlvalue=dynamic_cast<ExprLValue *>(lexpr);
    Result *result=rexpr->evaluate();
    exprlvalue->setResult(result->getValue());
}

StmtMethodCall::StmtMethodCall(ExprMethodCall *methodcall):methodcall(methodcall){}

void StmtMethodCall::analyzeSemantic(){methodcall->analyzeSemantic();}

void StmtMethodCall::execute(){methodcall->evaluate();}

StmtIf::StmtIf():elseblock(NULL){}

void StmtIf::analyzeSemantic(){
    Type *type=condition->analyzeSemantic();
    if(type->isEquivalent(new TypeBoolean())){
        ifblock->analyzeSemantic();
        for(int i=0;i<eliflist.size();++i)
            eliflist[i]->analyzeSemantic();
        if(elseblock)
            elseblock->analyzeSemantic();
    }
}

void StmtIf::execute(){
    if(getNumeric(condition->evaluate()))
        ifblock->execute();
    else{
        bool flag=false;
        for(int i=0;i<eliflist.size();++i){
            eliflist[i]->execute();
            
        }
    }
}

StmtElif::StmtElif(Expr *condition,StmtBlock *elifblock):condition(condition),elifblock(elifblock){}

void StmtElif::analyzeSemantic(){
    Type *type=condition->analyzeSemantic();
    
}

void StmtElif::execute(){
    if(getNumeric(condition->evaluate()))
        elifblock->execute();
}

StmtWhile::StmtWhile(Expr *condition,StmtBlock *whileblock):condition(condition),whileblock(whileblock){}

void StmtWhile::analyzeSemantic(){
    Type *type=condition->analyzeSemantic();
    if(type->isEquivalent(new TypeBoolean()))
        whileblock->analyzeSemantic();
}

void StmtWhile::execute(){
    while(true){
        if(getNumeric(condition->evaluate()))
            whileblock->execute();
        else break;
    }
}

void StmtFor::analyzeSemantic(){
    
}

void StmtFor::execute(){
    
}

StmtReturn::StmtReturn(Expr *ret){
    Type *type=new TypeVoid();
    if(ret) type=ret->analyzeSemantic();
    if(enclosingMethod){
        Type *type=enclosingMethod->methodblock->symboltable->get(enclosingMethod->methodname);
        
    }
}

void StmtReturn::execute(){
    Result *result=dynamic_cast<Result *>(ret->evaluate());
    enclosingMethod->resret=result;
}

void StmtBreak::analyzeSemantic(){
    if(enclosingloop==NULL)
        throw SemanticError(curmodname, curline);
}

void StmtBreak::execute(){
    enclosingmethod->methodblock->breakpoint=true;
}

void StmtContinue::analyzeSemantic(){
    if(enclosingloop==NULL)
        throw SemanticError(curmodname, curline);
}

void StmtContinue::execute(){
    enclosingmethod->methodblock->continuepoint=true;
}

StmtInput::StmtInput(Expr *lvalue):lvalue(lvalue){}

void StmtInput::analyzeSemantic(){
    if(lvalue->getExprType()!=ExprType::LVALUE)
        throw SemanticError(curmodname, curline);
}

void StmtInput::execute(){
    
}

void StmtPrint::analyzeSemantic(){
    if(printlist.size()>0){
        for(int i=0;i<printlist.size();++i)
            printlist[i]->analyzeSemantic();
    }
    else
        throw SemanticError(curmodname, curline);
}

void StmtPrint::execute(){
    for(int i=0;i<printlist.size();++i){
        printlist[i]->evaluate()->
    }
}

/****************************************************************/
/*************************声明节点类定义*************************/

Declaration::Declaration():symboltable(symboltable){}

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
    symboltable->put(methodname, typemethod);
    methodblock->analyzeSemantic();
}

void DeclMethod::intepret(){
    methodblock->execute();
}

/****************************************************************/
/*************************类型类节点类定义*************************/

/****************************************************************/
/**************************运算结果节点类定义*************************/

ResInteger::ResInteger(int value):value(value){}

int ResInteger::getNodeType(){return NodeType::_INTEGER;}

Result *ResInteger::getValue(){return new ResInteger(value);}

void ResInteger::print(){cout<<value;}


ResFloat::ResFloat(int value):value(value){}

int ResFloat::getNodeType(){return NodeType::_FLOAT;}

Result *ResFloat::getValue(){return new ResFloat(value);}

void ResFloat::print(){cout<<value;}


ResBoolean::ResBoolean(bool value):value(value){}

int ResBoolean::getNodeType(){return NodeType::BOOLEAN;}

Result *ResBoolean::getValue(){return new ResBoolean(value);}

void ResBoolean::print(){cout<<value;}


ResString::ResString(string value):value(value){}

int ResString::getNodeType(){return NodeType::_STRING;}

Result *ResString::getValue(){return new ResString(value);}

void ResString::print(){cout<<value;}


int ResArray::getNodeType(){return NodeType::ARRAY;}

Result *ResArray::getValue(){
    ResArray *arr=new ResArray();
    arr->value=value;
}

void ResArray::print(){
    cout<<"[";
    for(int i=0;i<value.size();++i){
        value[i]->getValue()->print();
        if(i<value.size()-1)
            cout<<",";
    }
    cout<<"]";
}

/****************************************************************/
/*************************环境变量节点类定义*************************/

SymbolTable::SymbolTable(SymbolTable *prev):prev(prev){}

bool SymbolTable::exists(string key){
    
}

Variable::Variable(const string &varname,Result *value):varname(varname),value(value){}

