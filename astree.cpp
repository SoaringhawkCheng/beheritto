#include "astree.h"

/****************************************************************/
/*************************外部变量引用声明*************************/

extern unordered_map<string,Procedure *>procedures;
extern RuntimeStack runtimestack;
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

ASTree::ASTree(){}

Expr::Expr(){}

/****************************************************************/
/*************************一元运算符节点类定义*************************/

ExprOpUnary::ExprOpUnary(Expr *expr):expr(expr){}

int ExprOpUnary::getExprType(){return ExprType::OPBIN;}

ExprOpposite::ExprOpposite(Expr *expr):ExprOpUnary(expr){}

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

Result *ExprArith::evaluate(){
    Result *lres=lexpr->evaluate();
    Result *rres=rexpr->evaluate();
    if(lres->getNodeType()==NodeType::_STRING){
        if(rres->getNodeType()==NodeType::_STRING&&opname=="+")
            return new ResString(getString(lres)+getString(rres));
        else if(rres->getNodeType()==NodeType::_INTEGER&&opname=="*"){
            int resinteger=getInteger(rres);
            if(resinteger>0){
                string resstring;
                for(int i=0;i<resinteger;++i)
                    resstring+=getString(lres);
                return new ResString(resstring);
            }
            else
                throw ExecutiveError(enclosingmodule, line);
        }
        else
            throw ExecutiveError(enclosingmodule, line);
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
                throw ExecutiveError(enclosingmodule, line);
                
        }
        else if(opname=="%"){
            if(lres->getNodeType()==NodeType::_INTEGER
               &&rres->getNodeType()==NodeType::_INTEGER
               &&getNumeric(rres))
                return new ResInteger(getInteger(lres)%getInteger(rres));
            else
                throw ExecutiveError(enclosingmodule, line);
        }
        else
            throw ExecutiveError(enclosingmodule, line);
    }
    else
        throw ExecutiveError(enclosingmodule, line);
}

ExprBitwise::ExprBitwise(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Result *ExprBitwise::evaluate(){
    Result *lres=lexpr->evaluate();
    Result *rres=lexpr->evaluate();
    if(lres->getNodeType()!=NodeType::_INTEGER||rres->getNodeType()!=NodeType::_INTEGER)
        throw ExecutiveError(enclosingmodule, line);
    if(getInteger(rres)<0) throw ExecutiveError(enclosingmodule, line);
    if(opname=="<<")
        return new ResInteger(getInteger(lres)<<getInteger(rres));
    else if(opname==">>")
        return new ResInteger(getInteger(lres)>>getInteger(rres));
    else
        throw ExecutiveError(enclosingmodule, line);
}

ExprCompare::ExprCompare(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Result *ExprCompare::evaluate(){
    Result *lres=lexpr->evaluate();
    Result *rres=rexpr->evaluate();
    if(lres->getNodeType()==NodeType::_STRING&&rres->getNodeType()==NodeType::_STRING){
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
            throw ExecutiveError(enclosingmodule, line);
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
            throw ExecutiveError(enclosingmodule, line);
    }
    else
        throw ExecutiveError(enclosingmodule, line);
}

ExprLogic::ExprLogic(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Result *ExprLogic::evaluate(){
    Result *lres=lexpr->evaluate();
    Result *rres=rexpr->evaluate();
    if(lres->getNodeType()==NodeType::_STRING&&rres->getNodeType()==NodeType::_STRING){
        if(opname=="and")
            return new ResBoolean(!getString(lres).empty()&&!getString(rres).empty());
        else if(opname=="or")
            return new ResBoolean(!getString(lres).empty()||!getString(rres).empty());
        else
            throw ExecutiveError(enclosingmodule, line);
    }
    else if(isNumeric(lres->getNodeType())&&isNumeric(rres->getNodeType())){
        if(opname=="and")
            return new ResBoolean(getNumeric(lres)&&getNumeric(rres));
        else if(opname=="or")
            return new ResBoolean(getNumeric(lres)||getNumeric(rres));
        else
            throw ExecutiveError(enclosingmodule, line);
    }
    else
        throw ExecutiveError(enclosingmodule, line);
}

/****************************************************************/
/*************************左值变量节点类定义*************************/

ExprLValue::ExprLValue(const string &varname):varname(varname){
    enclosingmethod=curmethod;
}

int ExprLValue::getExprType(){return ExprType::LVALUE;}

ExprID::ExprID(const string &varname):ExprLValue(varname){}

Result *ExprID::evaluate(){//
    Result *result=runtimestack.get(varname)->result;
    return result;
}

void ExprID::setResult(Result *result){
    if(runtimestack.exists(varname)){
        Variable *variable=runtimestack.get(varname);
        variable->result=result;
    }
    else{
        Variable *variable=new Variable(varname,result);
        runtimestack.put(varname, variable);
    }
}

ExprArray::ExprArray(const string &varname,Expr *index):ExprLValue(varname),index(index){}

Result *ExprArray::evaluate(){
    Result *result=runtimestack.get(varname)->result;
    //ResArray *resarray=dynamic_cast<ResArray *>(result);
    if(result->getNodeType()==NodeType::ARRAY){
        ResArray *resarray=dynamic_cast<ResArray *>(result);
        Result *resindex=index->evaluate();
        if(resindex->getNodeType()==NodeType::_INTEGER){
            ResInteger *resinteger=dynamic_cast<ResInteger *>(resindex);
            if(resinteger->value<resarray->value.size()&&resinteger->value>=0)
                return resarray->value[resinteger->value];
            else
                throw ExecutiveError(enclosingmodule, line);
        }
        else
            throw ExecutiveError(enclosingmodule, line);
    }
    else
        throw SemanticError(curmodname, curline);
}

void ExprArray::setResult(Result *result){
    if(runtimestack.exists(varname)){
        Variable *variable=runtimestack.get(varname);
        //?要不要进行类型检查
        if(variable->result->getNodeType()==NodeType::ARRAY){
            ResArray *resarray=dynamic_cast<ResArray *>(variable->result);
            Result *resindex=index->evaluate();
            if(resindex->getNodeType()==NodeType::_INTEGER){
                ResInteger *resinteger=dynamic_cast<ResInteger *>(variable->result);
                if(resinteger->value<resarray->value.size()&&resinteger->value>=0){
                    if(isNumeric(resarray->value[0]->getNodeType())&&isNumeric(result->getNodeType()))
                        resarray->value[resinteger->value]=result;
                    else if(resarray->value[0]->getNodeType()==NodeType::_STRING
                            &&result->getNodeType()==NodeType::_STRING)
                        resarray->value[resinteger->value]=result;
                    else
                        throw ExecutiveError(enclosingmodule, line);
                }
                else
                    throw ExecutiveError(enclosingmodule, line);
            }
            else
                throw ExecutiveError(enclosingmodule, line);
        }
        else
            throw ExecutiveError(enclosingmodule, line);
    }
    else
        throw ExecutiveError(enclosingmodule, curline);
}

/****************************************************************/
/*************************常量运算节点类定义*************************/

int ExprConstant::getExprType(){return ExprType::CONST;}

ExprInteger::ExprInteger(int value):value(value){}

Result *ExprInteger::evaluate(){
    return new ResInteger(value);
}

ExprFloat::ExprFloat(double value):value(value){}

Result *ExprFloat::evaluate(){
    return new ResFloat(value);
}

ExprBoolean::ExprBoolean(bool value):value(value){}

Result *ExprBoolean::evaluate(){
    return new ResBoolean(value);
}

ExprString::ExprString(const string &value):value(value){}

Result *ExprString::evaluate(){
    return new ResString(value);
}

Result *ExprArrayInit::evaluate(){
    ResArray *resarray=new ResArray();
    int type=0;
    for(int i=0;i<initlist.size();++i){
        Result *result=initlist[i]->evaluate();
        if(i==0) type=result->getNodeType();
        else if(type!=result->getNodeType())
            throw ExecutiveError(enclosingmodule, line);
        resarray->value.push_back(initlist[i]->evaluate());
    }
    return resarray;
}

/****************************************************************/
/*************************函数调用节点类定义*************************/

ExprMethodCall::ExprMethodCall(const string &methodname):methodname(methodname){}

Result *ExprMethodCall::evaluate(){
    //通过调用函数名寻找函数定义，未来这块要修改
    if(symboltable.exists(methodname)){
        Declaration *decl=symboltable.getDeclaration(methodname);
        if(decl->getDeclType()==DeclType::DECLCLASS){
            ResClass *resclass=new ResClass();
            
        }
        else if(decl->getDeclType()==DeclType::DECLMETHOD){
            StackFrame *newstackframe=new StackFrame();
            for(int i=0;i<arglist.size();++i){
                Result *res=arglist[i]->evaluate();
            }
            runtimestack.push(newstackframe);
            
            runtimestack.pop();
        }
    }
        
    if(runtimestack.exists(methodname)){
        Result *result=runtimestack.get(methodname)->result;
        if(result->getNodeType()==NodeType::METHOD){
            Procedure *proc=procedures[methodname];
            StackFrame *newstackframe=new StackFrame();
            for(int i=0;i<arglist.size();++i){
                Result *res=arglist[i]->evaluate();
                Variable *variable=new Variable(proc->method->paralist[i],result);//参数传递
                newstackframe->variabletable[proc->method->paralist[i]]=variable;
            }
            runtimestack.push(newstackframe);
            proc->method->methodblock->execute();
            runtimestack.pop();
            
            Result *resreturn=proc->method->resret;
            proc->method->resret=NULL;//??????????
            return resreturn;
        }
        else if(result->getNodeType()==NodeType::_CLASS){
            ResClass *resclass=dynamic_cast<ResClass *>(result);
            for(int i=0;i<arglist.size();++i){
                Result *res=arglist[i]->evaluate();
                Variable *variable=new Variable(resclass->paralist[i],res);
                resclass->member.insert(make_pair(resclass->paralist[i], variable));
            }
            return resclass;
        }
        else
            throw ExecutiveError(enclosingmodule, line);
    }
    else
        throw ExecutiveError(enclosingmodule, line);
}

/****************************************************************/
/*************************语句节点类定义*************************/

Statement::Statement():enclosingmethod(curmethod){}

StmtBlock::StmtBlock():symboltable(symboltable),continuepoint(false),breakpoint(false){}

void StmtBlock::execute(){
    for(int i=0;i<statements.size();++i){
        if(enclosingmethod->resret!=NULL) break;
        statements[i]->execute();
        if(breakpoint==true) break;
        else if(continuepoint==true){
            continuepoint=false;
            continue;
        }
    }
}

StmtAssign::StmtAssign(Expr *lexpr,Expr *rexpr):lexpr(lexpr),rexpr(rexpr){}

void StmtAssign::execute(){
    /*肯定是左值表达式，转化为基类，根据实际类型动态选择setResult函数*/
    ExprLValue *exprlvalue=dynamic_cast<ExprLValue *>(lexpr);
    Result *result=rexpr->evaluate();
    exprlvalue->setResult(result->getValue());
}

StmtMethodCall::StmtMethodCall(ExprMethodCall *methodcall):methodcall(methodcall){}

void StmtMethodCall::execute(){methodcall->evaluate();}

StmtIf::StmtIf():elseblock(NULL){}

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
        throw ExecutiveError(enclosingmodule, line);
}

StmtElif::StmtElif(Expr *condition,StmtBlock *elifblock)
    :condition(condition),elifblock(elifblock),executed(false){}

void StmtElif::execute(){
    Result *result=condition->evaluate();
    if(isNumeric(result->getNodeType()))
    {
        if(getNumeric(result))
            elifblock->execute();
        else
            throw ExecutiveError(enclosingmodule, line);
    }
    else
        throw ExecutiveError(enclosingmodule, line);
}

StmtWhile::StmtWhile(Expr *condition,StmtBlock *whileblock):condition(condition),whileblock(whileblock){}

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
            throw ExecutiveError(enclosingmodule, line);
    }
}

void StmtFor::execute(){
    
}

StmtReturn::StmtReturn(Expr *ret){
    Type *type=new TypeVoid();
    if(ret) type=ret->analyzeSemantic();
    if(enclosingmethod){
        Type *type=enclosingmethod->methodblock->symboltable->get(enclosingmethod->methodname);
        
    }
}

void StmtReturn::execute(){
    Result *result=dynamic_cast<Result *>(ret->evaluate());
    enclosingmethod->resret=result;
}

void StmtBreak::execute(){
    enclosingmethod->methodblock->breakpoint=true;
}

void StmtContinue::execute(){
    enclosingmethod->methodblock->continuepoint=true;
}

StmtInput::StmtInput(Expr *lvalue):lvalue(lvalue){}

void StmtInput::execute(){
    
}

void StmtPrint::execute(){
    for(int i=0;i<printlist.size();++i){
        printlist[i]->evaluate()->print();
    }
}

/****************************************************************/
/*************************声明节点类定义*************************/

Declaration::Declaration(){}

DeclModule::DeclModule(const string &modname):modname(modname){}

int DeclModule::getDeclType(){return DeclType::DECLMODULE;}

void DeclModule::intepret(){
    runtimestack.push(new StackFrame());
    if(entry)
        entry->intepret();
    else if(runtimestack.stackframelist.size()==1)
        throw ExecutiveError(modname, line);
    runtimestack.pop();
}

DeclClass::DeclClass(const string &classname):classname(classname){}

int DeclClass::getDeclType(){return DeclType::DECLMODULE;}

void DeclClass::intepret(){
    for(auto field:fieldlist)
        field->intepret();
}

DeclMethod::DeclMethod(const string &methodname):methodname(methodname){}

int DeclMethod::getDeclType(){return DeclType::DECLMODULE;}

void DeclMethod::intepret(){methodblock->execute();}

DeclField::DeclField(StmtAssign *assign):assign(assign){}

void DeclField::intepret(){assign->execute();}

void DeclEntry::intepret(){
    for(auto statement:statements)
        statement->execute();
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

int ResClass::getNodeType(){return NodeType::_CLASS;}

/****************************************************************/
/*************************环境变量节点类定义*************************/

int SymbolTable::getDeclType(const string &key){
    vector<string> keyarray=nameSplit(key, ".");
    auto iter=keyarray.begin();
    DeclModule *declmodule=program;
    DeclClass *declclass=NULL;
    DeclMethod *declmethod=NULL;
    int state=1;
    while(iter!=keyarray.end()){
        switch(state){
            case 1:{
                if(declmodule->modulelist.count(*iter)){
                    declmodule=declmodule->modulelist[*iter];
                    ++iter;
                    state=1;
                }
                else if(declmodule->classlist.count(*iter)){
                    declclass=declmodule->classlist[*iter];
                    ++iter;
                    state=2;
                }
                else if(declmodule->methodlist.count(*iter)){
                    declmethod=declmodule->methodlist[*iter];
                    ++iter;
                    state=3;
                }
                else return -1;
            }
            break;
            case 2:{
                if(declclass->methodlist.count(*iter)){
                    declmethod=declclass->methodlist[*iter];
                    ++iter;
                    state=3;
                }
                else return -1;
            }
            break;
            case 3:
                return -1;
            default:
                break;
        }
    }
    if(iter==key.end()) return true;
    else return false;
}

Declaration *SymbolTable::getDeclaration(const string &key){
    vector<string> keyarray=nameSplit(key, ".");
    DeclModule *declmodule=program;
    DeclClass *declclass=NULL;
    DeclMethod *declmethod=NULL;
    auto iter=keyarray.begin();
    int state=1;
    while(iter!=keyarray.end()){
        switch(state){
            case 1:{
                if(declmodule->modulelist.count(*iter)){
                    declmodule=declmodule->modulelist[*iter];
                    ++iter;
                    state=1;
                }
                else if(declmodule->classlist.count(*iter)){
                    declclass=declmodule->classlist[*iter];
                    ++iter;
                    state=2;
                }
                else if(declmodule->methodlist.count(*iter)){
                    declmethod=declmodule->methodlist[*iter];
                    ++iter;
                    state=3;
                }
            }
            break;
            case 2:{
                if(declclass->methodlist.count(*iter)){
                    declmethod=declclass->methodlist[*iter];
                    ++iter;
                    state=3;
                }
            }
            break;
            default:
                break;
        }
        if(state==3) break;
    }
    switch(state){
        case 1:
            return declmodule;
        case 2:
            return declclass;
        case 3:
            return declmethod;
    }
}


Variable::Variable(const string &varname,Result *value):varname(varname),result(result){}

bool RuntimeStack::exists(const string &key){
    vector<string> keyarray=nameSplit(key,".");
    auto iter=keyarray.begin();
    bool flag=false;
    vector<StackFrame *>::reverse_iterator stackframe;
    for(auto stackframe=stackframelist.rbegin();stackframe!=stackframelist.rend();++stackframe){
        if((*stackframe)->variabletable.count(*iter)){
            flag=true;
            break;
        }
    }
    if(!flag) return false;
    Result *keytype=(*stackframe)->variabletable[*iter]->result;
    ++iter;
    DeclModule *declmodule=program;
    DeclClass *declclass;
    DeclMethod *declmethod;
    int state=1;
    while(iter!=keyarray.end()){
        switch(state){
            case 1:{
                if(declmodule->modulelist.count(*iter)){
                    declmodule=declmodule->modulelist[*iter];
                    ++iter;
                    state=1;
                }
                else if(declmodule->classlist.count(*iter)){
                    declclass=declmodule->classlist[*iter];
                    ++iter;
                    state=2;
                }
                else if(declmodule->methodlist.count(*iter)){
                    declmethod=declmodule->methodlist[*iter];
                    ++iter;
                    state=3;
                }
                else return false;
            }
            break;
            case 2:{
                if(declclass->methodlist.count(*iter)){
                    declmethod=declclass->methodlist[*iter];
                    ++iter;
                    state=3;
                }
                else return false;
            }
            default:
                break;
        }
        if(state==3) break;
    }
    if(iter==keyarray.end()) return true;
    else return false;
}

Variable *RuntimeStack::get(const string &key){
    if(key.find(".")!=key.npos)
        throw ExecutiveError(curmodname, curline);
    vector<StackFrame *>::reverse_iterator stackframe;
    for(stackframe=stackframelist.rbegin();stackframe!=stackframelist.rend();++stackframe){
        if((*stackframe)->variabletable.count(key))
            break;
    }
    if(stackframe==stackframelist.rend())
        throw ExecutiveError(curmodname, curline);
    return (*stackframe)->variabletable[key];
}

void RuntimeStack::put(const string &key, Variable *variable){
    
}

