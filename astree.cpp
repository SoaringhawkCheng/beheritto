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

static bool isNumeric(int nodetype){
    switch(nodetype){
    case NodeType::_INTEGER:
    case NodeType::_FLOAT:
    case NodeType::BOOLEAN:
        return true;
    default:
        return false;
    }
}

static bool isConstant(int nodetype){
    switch(nodetype){
    case NodeType::_INTEGER:
    case NodeType::_FLOAT:
    case NodeType::BOOLEAN:
    case NodeType::_STRING:
        return true;
    default:
        return false;
    }
}

static vector<string> nameSplit(const string &name,const string &pattern){
    char *cstr=new char[strlen(name.c_str())+1];
    strcpy(cstr,name.c_str());
    vector<string> res;
    char *tmpstr=strtok(cstr,pattern.c_str());
    while(tmpstr!=NULL){
        res.push_back(string(tmpstr));
    }
    delete []cstr;
    return res;
}

static double getNumeric(Result *result){
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

static int getInteger(Result *result){
    ResInteger *resint=dynamic_cast<ResInteger *>(result->getValue());
    return resint->value;
}

static double getFloat(Result *result){
    ResFloat *resfloat=dynamic_cast<ResFloat *>(result->getValue());
    return resfloat->value;
}

static bool getBoolean(Result *result){
    ResBoolean *resboolean=dynamic_cast<ResBoolean *>(result->getValue());
    return resboolean->value;
}

static string getString(Result *result){
    ResString *resstring=dynamic_cast<ResString *>(result->getValue());
    return resstring->value;
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
    if(isNumeric(type->getNodeType()))
        return type;
    else if(type->getNodeType()==NodeType::WILDCARD)
        return new TypeWildcard();
    else
        throw SemanticError(curmodname, curline);
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
    throw ExecutiveError(curmodname, curline);
}

ExprNot::ExprNot(Expr *expr):ExprOpUnary(expr){}

Type *ExprNot::analyzeSemantic(){
    Type *type=expr->analyzeSemantic();
    if(isNumeric(type->getNodeType()))
        return type;
    else if(type->getNodeType()==NodeType::WILDCARD)
        return new TypeWildcard();
    else
        throw SemanticError(curmodname, curline);
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
    Type *ltype=lexpr->analyzeSemantic();
    Type *rtype=rexpr->analyzeSemantic();
    if(ltype->getNodeType()!=NodeType::WILDCARD&&!isConstant(ltype->getNodeType()))
        throw SemanticError(curmodname, curline);
    else if(rtype->getNodeType()!=NodeType::WILDCARD&&!isConstant(rtype->getNodeType()))
        throw SemanticError(curmodname, curline);
    else if(ltype->getNodeType()==rtype->getNodeType()&&isConstant(ltype->getNodeType()))
        return ltype;
    else if(ltype->isEquivalent(rtype))
        return new TypeWildcard();
    else
        throw SemanticError(curmodname, curline);
}

Result *ExprArith::evaluate(){
    Result *lres=lexpr->evaluate();
    Result *rres=rexpr->evaluate();
    if(lres->getNodeType()==NodeType::_STRING){
        if(rres->getNodeType()==NodeType::_STRING&&opname=="+")
            return new ResString(getString(lres)+getString(rres));
        else if(rres->getNodeType()==NodeType::_INTEGER&&opname=="*"){
            string resstring;
            for(int i=0;i<getInteger(rres);++i)
                resstring+=getString(lres);
            return new ResString(resstring);
        }
        else
            throw ExecutiveError(curmodname, curline);
    }
    else if(isNumeric(lres->getNodeType())&&isNumeric(rres->getNodeType())){
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
        else if(opname=="-"){
            if(restype==NodeType::_FLOAT)
                return new ResFloat(getNumeric(lres)-getNumeric(rres));
            else
                return new ResInteger(getNumeric(lres)-getNumeric(rres));
        }
        else if(opname=="*"){
            if(restype==NodeType::_FLOAT)
                return new ResFloat(getNumeric(lres)*getNumeric(rres));
            else
                return new ResInteger(getNumeric(lres)*getNumeric(rres));
        }
        else if(opname=="/"){
            if(getNumeric(rres)){
                if(restype==NodeType::_FLOAT)
                    return new ResFloat(getNumeric(lres)/getNumeric(rres));
                else
                    return new ResInteger(getNumeric(lres)/getNumeric(rres));
            }
            else
                throw ExecutiveError(curmodname, curline);
                
        }
        else if(opname=="%"){
            if(lres->getNodeType()==NodeType::_INTEGER
               &&rres->getNodeType()==NodeType::_INTEGER
               &&getNumeric(rres))
                return new ResInteger(int(getNumeric(lres))%int(getNumeric(rres)));
            else
                throw ExecutiveError(curmodname, curline);
        }
        else
            throw ExecutiveError(curmodname, curline);
    }
    else
        throw ExecutiveError(curmodname, curline);
}

ExprBitwise::ExprBitwise(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Type *ExprBitwise::analyzeSemantic(){
    Type *ltype=lexpr->analyzeSemantic();
    Type *rtype=rexpr->analyzeSemantic();
    if(ltype->getNodeType()==NodeType::_INTEGER&&ltype->isEquivalent(rtype))
        return ltype;
    else if(rtype->getNodeType()==NodeType::_INTEGER&&rtype->isEquivalent(ltype))
        return rtype;
    else if(ltype->getNodeType()!=NodeType::WILDCARD&&rtype->getNodeType()!=NodeType::WILDCARD)
        return new TypeInteger();
    else
        throw SemanticError(curmodname, curline);
}

Result *ExprBitwise::evaluate(){
    Result *lres=lexpr->evaluate();
    Result *rres=lexpr->evaluate();
    if(lres->getNodeType()!=NodeType::_INTEGER||rres->getNodeType()!=NodeType::_INTEGER)
        throw ExecutiveError(modname, line);
    if(getInteger(rres)<0) throw ExecutiveError(modname, line);
    if(opname=="<<")
        return new ResInteger(getInteger(lres)<<getInteger(rres));
    else if(opname==">>")
        return new ResInteger(getInteger(lres)>>getInteger(rres));
    else
        throw ExecutiveError(modname, line);
}


ExprCompare::ExprCompare(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}


Type *ExprCompare::analyzeSemantic(){
    Type *ltype=lexpr->analyzeSemantic();
    Type *rtype=rexpr->analyzeSemantic();
    if(isNumeric(ltype->getNodeType())&&ltype->isEquivalent(rtype))
        return new TypeBoolean();
    else if(isNumeric(rtype->getNodeType()&&rtype->isEquivalent(ltype)))
        return new TypeBoolean();
    else if(ltype->getNodeType()==NodeType::WILDCARD&&rtype->getNodeType()==NodeType::WILDCARD)
        return new TypeBoolean();
    else
        throw SemanticError(modname, line);
}

Result *ExprCompare::evaluate(){
    Result *lres=lexpr->evaluate();
    Result *rres=rexpr->evaluate();
    if(lres->getNodeType()!=rres->getNodeType())
        throw ExecutiveError(modname, line);
    else if(lres->getNodeType()==NodeType::_STRING&&rres->getNodeType()==NodeType::_STRING){
        if(opname=="==")
            return new ResBoolean(getString(lres)==getString(rres));
        else if(opname=="!=")
            return new ResBoolean(getString(lres)==getString(rres));
        else if(opname==">")
            return new ResBoolean(getString(lres)==getString(rres));
        else if(opname==">=")
            return new ResBoolean(getString(lres)==getString(rres));
        else if(opname=="<")
            return new ResBoolean(getString(lres)==getString(rres));
        else if(opname=="<=")
            return new ResBoolean(getString(lres)==getString(rres));
        else
            throw ExecutiveError(modname, line);
    }
    else if(isNumeric(lres->getNodeType())&&isNumeric(rres->getNodeType())){
        if(opname=="==")
            return new ResBoolean(getNumeric(lres)==getNumeric(rres));
        else if(opname=="!=")
            return new ResBoolean(getNumeric(lres)!=getNumeric(rres));
        else if(opname==">")
            return new ResBoolean(getNumeric(lres)>getNumeric(rres));
        else if(opname==">=")
            return new ResBoolean(getNumeric(lres)>=getNumeric(rres));
        else if(opname=="<")
            return new ResBoolean(getNumeric(lres)<getNumeric(rres));
        else if(opname=="<=")
            return new ResBoolean(getNumeric(lres)<=getNumeric(rres));
        else
            throw ExecutiveError(modname, line);
    }
    else
        throw ExecutiveError(modname, line);
}

ExprLogic::ExprLogic(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Type *ExprLogic::analyzeSemantic(){
    Type *ltype=lexpr->analyzeSemantic();
    Type *rtype=rexpr->analyzeSemantic();
    
    if(isConstant(ltype->getNodeType())&&ltype->isEquivalent(rtype))
        return new TypeBoolean();
    else if(isConstant(rtype->getNodeType())&&rtype->isEquivalent(ltype))
        return new TypeBoolean();
    else if(ltype->getNodeType()==NodeType::WILDCARD&&rtype->getNodeType()==NodeType::WILDCARD)
        return new TypeBoolean();
    else
        throw ExecutiveError(modname, line);
}

Result *ExprLogic::evaluate(){
    Result *lres=lexpr->evaluate();
    Result *rres=rexpr->evaluate();
    if(lres->getNodeType()==NodeType::_STRING&&rres->getNodeType()==NodeType::_STRING){
        if(opname=="and")
            return new ResBoolean(!getString(lres).empty()&&!getString(rres).empty());
        else if(opname=="or")
            return new ResBoolean(!getString(lres).empty()||!getString(rres).empty());
        else
            throw ExecutiveError(modname, line);
    }
    else if(isNumeric(lres->getNodeType())&&isNumeric(rres->getNodeType())){
        if(opname=="and")
            return new ResBoolean(getNumeric(lres)&&getNumeric(rres));
        else if(opname=="or")
            return new ResBoolean(getNumeric(lres)||getNumeric(rres));
        else
            throw ExecutiveError(modname, line);
    }
    else
        throw ExecutiveError(modname, line);
}

/****************************************************************/
/*************************左值变量节点类定义*************************/
/*
ExprLValue::ExprLValue(const string &varname)
    :varname(varname),enclosingmethod(NULL),enclosingclass(NULL),enclosingmodule(NULL){}

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
*/
/****************************************************************/
/*************************常量运算节点类定义*************************/
/*
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
}*/

/****************************************************************/
/*************************函数调用节点类定义*************************/
/*
ExprMethodCall::ExprMethodCall(const string &methodname):methodname(methodname){}

Type *ExprMethodCall::analyzeSemantic(){
    
}

Result *ExprMethodCall::evaluate(){
    //通过调用函数名寻找函数定义，未来这块要修改
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
*/
/****************************************************************/
/*************************语句节点类定义*************************/

Statement::Statement():enclosingmethod(curmethod){}

StmtBlock::StmtBlock():symboltable(symboltable),continuepoint(continuepoint),breakpoint(breakpoint){}

void StmtBlock::analyzeSemantic(){
    for(int i=0;i<statements.size();++i)
        statements[i]->analyzeSemantic();
}
/*
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
*/
StmtAssign::StmtAssign(Expr *lexpr,Expr *rexpr):lexpr(lexpr),rexpr(rexpr){}

void StmtAssign::analyzeSemantic(){
//    if(lexpr->getExprType()==ExprType::LVALUE){
//        ExprLValue *exprlvalue=dynamic_cast<ExprLValue *>(lexpr);
//        if(exprlvalue->symboltable->exists(exprlvalue->varname)){
//            Type *ltype=exprlvalue->symboltable->get(varname);
//            Type *rtype=lexpr->analyzeSemantic();
//            
//        }
//        else
//            throw SemanticError(modname, line);
//    }
//    else
//        throw SemanticError(modname, line);
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
//    Type *type=condition->analyzeSemantic();
//    if(type->isEquivalent(new TypeBoolean())){
//        ifblock->analyzeSemantic();
//        for(int i=0;i<eliflist.size();++i)
//            eliflist[i]->analyzeSemantic();
//        if(elseblock)
//            elseblock->analyzeSemantic();
//    }
}

void StmtIf::execute(){
    Result *result=condition->evaluate();
    if(isNumeric(result->getNodeType())){
        if(getNumeric(result))
            ifblock->execute();
        else{
            bool flag=false;
            for(int i=0;i<eliflist.size();++i){
                eliflist[i]->execute();
                if(eliflist[i]->executed){
                    eliflist[i]->executed=false;
                    flag=true;
                    break;
                }
            }
            if(flag)
                if(elseblock)
                    elseblock->execute();
        }
    }
    else
        throw ExecutiveError(modname, line);
}

StmtElif::StmtElif(Expr *condition,StmtBlock *elifblock)
    :condition(condition),elifblock(elifblock),executed(false){}

void StmtElif::analyzeSemantic(){
//    Type *type=condition->analyzeSemantic();
//    if(isNumeric(type->getNodeType())&&type->getNodeType()==NodeType::WILDCARD)
//    
}

void StmtElif::execute(){
    Result *result=condition->evaluate();
    if(isNumeric(result->getNodeType()))
    {
        if(getNumeric(result))
            elifblock->execute();
        else
            throw ExecutiveError(modname, line);
    }
    else
        throw ExecutiveError(modname, line);
}

StmtWhile::StmtWhile(Expr *condition,StmtBlock *whileblock):condition(condition),whileblock(whileblock){}

void StmtWhile::analyzeSemantic(){
//    Type *type=condition->analyzeSemantic();
//    if(type->isEquivalent(new TypeBoolean()))
//        whileblock->analyzeSemantic();
}

void StmtWhile::execute(){
    while(true){
        Result *result=condition->evaluate();
        if(isNumeric(result->getNodeType())){
            if(getNumeric(result))
                whileblock->execute();
            else
                break;
        }
        else
            throw ExecutiveError(modname, line);
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
/*
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

void DeclMethod::intepret(){methodblock->execute();}
*/
/****************************************************************/
/*************************类型类节点类定义*************************/
/*
int TypeWildcard::getNodeType(){return NodeType::WILDCARD;}

bool TypeWildcard::isEquivalent(Type *type){return true;}

int TypeInteger::getNodeType(){return NodeType::_INTEGER;}

bool TypeInteger::isEquivalent(Type *type){
    return type->getNodeType()==NodeType::_INTEGER
    ||type->getNodeType()==NodeType::WILDCARD;
}

int TypeFloat::getNodeType(){return NodeType::_FLOAT;}

bool TypeFloat::isEquivalent(Type *type){
    return type->getNodeType()==NodeType::_FLOAT
    ||type->getNodeType()==NodeType::WILDCARD;
}

int TypeBoolean::getNodeType(){return NodeType::BOOLEAN;}

bool TypeBoolean::isEquivalent(Type *type){
    return type->getNodeType()==NodeType::BOOLEAN
    ||type->getNodeType()==NodeType::WILDCARD;
}

int TypeString::getNodeType(){return NodeType::_STRING;}

bool TypeString::isEquivalent(Type *type){
    return type->getNodeType()==NodeType::_STRING
    ||type->getNodeType()==NodeType::WILDCARD;
}

int TypeMethod::getNodeType(){return NodeType::METHOD;}

bool TypeMethod::isEquivalent(Type *type){
    if(type->getNodeType()==NodeType::METHOD){
        TypeMethod *typemethod=dynamic_cast<TypeMethod *>(type);
        if(typemethod->returntype->isEquivalent(returntype))
            return true;
        else return false;
    }
    else return false;
}*/

/****************************************************************/
/**************************运算结果节点类定义*************************/
/*
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
*/
/****************************************************************/
/*************************环境变量节点类定义*************************/
    
SymbolTable::SymbolTable(SymbolTable *prev):prev(prev){}

bool SymbolTable::exists(const string& key){
    vector<string> keyarray=nameSplit(key,".");
    for(auto iter=keyarray.begin();iter!=keyarray.end();++iter){
        bool flag=false;
        for(auto table=this;table!=NULL;table=table->prev){
            if(table->symbolmap.count(*iter)>0){
                flag=true;
                break;
            }
        }
        if(!flag) return false;
    }
    return true;
}

void SymbolTable::put(const string &key, Type *type){
    if(key.find(".")!=key.npos)
        throw SemanticError(curmodname, curline);
    if(symbolmap.count(key))
        throw SemanticError(curmodname, curline);
    else
        symbolmap[key]=type;
}

Type *SymbolTable::get(const string &key){
    vector<string> keyarray=nameSplit(key,".");
    SymbolTable *table=this;
    auto iter=keyarray.begin();
    bool flag=false;
    for(table=this;table!=NULL;table=table->prev){
        if(table->symbolmap.count(*iter)>0){
            flag=true;
            break;
        }
    }
    if(!flag) throw SemanticError(curmodname,curline);
    Type *keytype=table->symbolmap[*iter];
    while(iter!=keyarray.end()){
        if(keytype->getNodeType()==NodeType::MODULE){
            TypeModule *typemodule=dynamic_cast<TypeModule *>(keytype);
            if(typemodule->modulemap.count(*iter)){
                keytype=typemodule->modulemap[*iter];
                iter++;
                continue;
            }
            if(typemodule->classmap.count(*iter)){
                keytype=typemodule->classmap[*iter];
                iter++;
                continue;
            }
            if(typemodule->methodmap.count(*iter)){
                keytype=typemodule->methodmap[*iter];
                iter++;
                continue;
            }
            throw SemanticError(curmodname,curline);
        }
        if(keytype->getNodeType()==NodeType::_CLASS){
            TypeClass *typeclass=dynamic_cast<TypeClass *>(keytype);
            if(typeclass->methodmap.count(*iter)){
                keytype=typeclass->methodmap[*iter];
                iter++;
                break;
            }
            throw SemanticError(curmodname,curline);
        }
    }
    if(iter==keyarray.end()) return keytype;
    else throw SemanticError(curmodname, curline);
}

void SymbolTable::set(const string &key, Type *type){
    for(auto table=this;table!=NULL;table=table->prev){
        if(table->symbolmap.count(key))
            table->symbolmap[key]=type;
    }
}

Variable::Variable(const string &varname,Result *value):varname(varname),value(value){}
/*
bool StackFrame::exists(const string &key){
    for(auto iter=vartablelist.rbegin();iter!=vartablelist.rend();++iter){
        
    }
}*/

