#include "astree.h"

/****************************************************************/
/*************************外部变量引用声明*************************/

//extern unordered_map<string,Procedure *>procedures;
//extern RuntimeStack runtimestack;
//extern DeclMethod *curmethod;
//extern StmtLoop *curloop;
//extern string curmodname;
//extern int curline;

/****************************************************************/
/*************************全局静态函数定义*************************/

static bool isNumeric(int objtype){
    switch(objtype){
    case ObjType::OBJINTEGER:
    case ObjType::OBJFLOAT:
    case ObjType::OBJBOOLEAN:
        return true;
    default:
        return false;
    }
}

static bool isConstant(int objtype){
    switch(objtype){
    case ObjType::OBJINTEGER:
    case ObjType::OBJFLOAT:
    case ObjType::OBJBOOLEAN:
    case ObjType::OBJSTRING:
        return true;
    default:
        return false;
    }
}

static vector<string> nameSplit(const string &name,const string &pattern){
    vector<string> namearray;
    string namep=name+pattern;
    int pos=name.find(pattern);
    int size=namep.size();
    while(pos!=namep.npos)
    {
        string subname=namep.substr(0,pos);
        namearray.push_back(subname);
        namep=namep.substr(pos+1);
        pos=namep.find(pattern);
    }
    return namearray;
}

static double getNumeric(Object *object){
    if(object->getObjType()==ObjType::OBJINTEGER){
        ObjInteger *objinteger=dynamic_cast<ObjInteger *>(object);
        return objinteger->value;
    }
    else if(object->getObjType()==ObjType::OBJFLOAT){
        ObjFloat *objfloat=dynamic_cast<ObjFloat *>(object);
        return objfloat->value;
    }
    else if(object->getObjType()==ObjType::OBJBOOLEAN){
        ObjBoolean *objboolean=dynamic_cast<ObjBoolean *>(object);
        return objboolean->value;
    }
    else
        throw RuntimeError(object->enclosingmodule,object->line);
}

static int getInteger(Object *object){
    ObjInteger *objinteger=dynamic_cast<ObjInteger *>(object);
    return objinteger->value;
}

static double getFloat(Object *object){
    ObjFloat *objfloat=dynamic_cast<ObjFloat *>(object);
    return objfloat->value;
}

static bool getBoolean(Object *object){
    ObjBoolean *objboolean=dynamic_cast<ObjBoolean *>(object);
    return objboolean->value;
}

static string getString(Object *object){
    ObjString *objstring=dynamic_cast<ObjString *>(object);
    return objstring->value;
}

/****************************************************************/
/*************************表达式节点类定义*************************/

/****************************************************************/
/*************************一元运算符节点类定义*************************/

ExprOpUnary::ExprOpUnary(Expr *expr):expr(expr){}

int ExprOpUnary::getExprType(){return ExprType::OPBIN;}

ExprOpposite::ExprOpposite(Expr *expr):ExprOpUnary(expr){}

Object *ExprOpposite::evaluate(){
    curmodname=enclosingmodule;
    curline=line;
    Object *object=expr->evaluate();
    if(object->getObjType()==ObjType::OBJINTEGER){
        ObjInteger *objinteger=dynamic_cast<ObjInteger *>(object);
        return new ObjInteger(-1*objinteger->value);
    }
    if(object->getObjType()==ObjType::OBJFLOAT){
        ObjFloat *objfloat=dynamic_cast<ObjFloat *>(object);
        return new ObjFloat(-1*objfloat->value);
    }
    if(object->getObjType()==ObjType::OBJBOOLEAN){
        ObjBoolean *objboolean=dynamic_cast<ObjBoolean *>(object);
        return new ObjBoolean(-1*objboolean->value);
    }
    throw RuntimeError(enclosingmodule,line);
}

ExprNot::ExprNot(Expr *expr):ExprOpUnary(expr){}

Object *ExprNot::evaluate(){
    curmodname=enclosingmodule;
    curline=line;
    Object *object=expr->evaluate();
    return new ObjBoolean(getNumeric(object)!=0);
}

/****************************************************************/
/*************************二元运算节点类定义*************************/

ExprOpBinary::ExprOpBinary(const string &opname,Expr *lexpr,Expr *rexpr)
    :opname(opname),lexpr(lexpr),rexpr(rexpr){}

//int ExprOpBinary::getExprType(){
//    return ExprType::OPBIN;
//}

ExprArith::ExprArith(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Object *ExprArith::evaluate(){
    curmodname=enclosingmodule;
    curline=line;
    Object *lobj=lexpr->evaluate();
    Object *robj=rexpr->evaluate();
    if(lobj->getObjType()==ObjType::OBJSTRING){
        if(robj->getObjType()==ObjType::OBJSTRING&&opname=="+")
            return new ObjString(getString(lobj)+getString(robj));
        else if(robj->getObjType()==ObjType::OBJINTEGER&&opname=="*"){
            int objinteger=getInteger(robj);
            if(objinteger>0){
                string objstring;
                for(int i=0;i<objinteger;++i)
                    objstring+=getString(lobj);
                return new ObjString(objstring);
            }
            else
                throw RuntimeError(enclosingmodule,line);
        }
        else
            throw RuntimeError(enclosingmodule,line);
    }
    else if(isNumeric(lobj->getObjType())&&isNumeric(robj->getObjType())){
        ObjType objtype;
        if(lobj->getObjType()==ObjType::OBJFLOAT||robj->getObjType()==ObjType::OBJFLOAT)
            objtype=ObjType::OBJFLOAT;
        else objtype=ObjType::OBJINTEGER;
        if(opname=="+"){
            if(objtype==ObjType::OBJFLOAT)
                return new ObjFloat(getNumeric(lobj)+getNumeric(robj));
            else
                return new ObjInteger(getNumeric(lobj)+getNumeric(robj));
        }
        else if(opname=="-"){
            if(objtype==ObjType::OBJFLOAT)
                return new ObjFloat(getNumeric(lobj)-getNumeric(robj));
            else
                return new ObjInteger(getNumeric(lobj)-getNumeric(robj));
        }
        else if(opname=="*"){
            if(objtype==ObjType::OBJFLOAT)
                return new ObjFloat(getNumeric(lobj)*getNumeric(robj));
            else
                return new ObjInteger(getNumeric(lobj)*getNumeric(robj));
        }
        else if(opname=="/"){
            if(getNumeric(robj)){
                if(objtype==ObjType::OBJFLOAT)
                    return new ObjFloat(getNumeric(lobj)/getNumeric(robj));
                else
                    return new ObjInteger(getNumeric(lobj)/getNumeric(robj));
            }
            else
                throw RuntimeError(enclosingmodule,line);
                
        }
        else if(opname=="%"){
            if(lobj->getObjType()==ObjType::OBJINTEGER
               &&robj->getObjType()==ObjType::OBJINTEGER
               &&getNumeric(robj))
                return new ObjInteger(getInteger(lobj)%getInteger(robj));
            else
                throw RuntimeError(enclosingmodule,line);
        }
        else
            throw RuntimeError(enclosingmodule,line);
    }
    else
        throw RuntimeError(enclosingmodule,line);
}

ExprBitwise::ExprBitwise(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Object *ExprBitwise::evaluate(){
    curmodname=enclosingmodule;
    curline=line;
    Object *lobj=lexpr->evaluate();
    Object *robj=lexpr->evaluate();
    if(lobj->getObjType()!=ObjType::OBJINTEGER||robj->getObjType()!=ObjType::OBJINTEGER)
        throw RuntimeError(curmodname,  curline);
    if(getInteger(robj)<0) throw RuntimeError(curmodname,  curline);
    if(opname=="<<")
        return new ObjInteger(getInteger(lobj)<<getInteger(robj));
    else if(opname==">>")
        return new ObjInteger(getInteger(lobj)>>getInteger(robj));
    else
        throw RuntimeError(enclosingmodule,line);
}

ExprCompare::ExprCompare(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Object *ExprCompare::evaluate(){
    curmodname=enclosingmodule;
    curline=line;
    Object *lobj=lexpr->evaluate();
    Object *robj=rexpr->evaluate();
    if(lobj->getObjType()==ObjType::OBJSTRING&&robj->getObjType()==ObjType::OBJSTRING){
        if(opname=="==")
            return new ObjBoolean(getString(lobj)==getString(robj));
        else if(opname=="!=")
            return new ObjBoolean(getString(lobj)==getString(robj));
        else if(opname==">")
            return new ObjBoolean(getString(lobj)==getString(robj));
        else if(opname==">=")
            return new ObjBoolean(getString(lobj)==getString(robj));
        else if(opname=="<")
            return new ObjBoolean(getString(lobj)==getString(robj));
        else if(opname=="<=")
            return new ObjBoolean(getString(lobj)==getString(robj));
        else
            throw RuntimeError(enclosingmodule,line);
    }
    else if(isNumeric(lobj->getObjType())&&isNumeric(robj->getObjType())){
        if(opname=="==")
            return new ObjBoolean(getNumeric(lobj)==getNumeric(robj));
        else if(opname=="!=")
            return new ObjBoolean(getNumeric(lobj)!=getNumeric(robj));
        else if(opname==">")
            return new ObjBoolean(getNumeric(lobj)>getNumeric(robj));
        else if(opname==">=")
            return new ObjBoolean(getNumeric(lobj)>=getNumeric(robj));
        else if(opname=="<")
            return new ObjBoolean(getNumeric(lobj)<getNumeric(robj));
        else if(opname=="<=")
            return new ObjBoolean(getNumeric(lobj)<=getNumeric(robj));
        else
            throw RuntimeError(enclosingmodule,line);
    }
    else
        throw RuntimeError(enclosingmodule,line);
}

ExprLogic::ExprLogic(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Object *ExprLogic::evaluate(){
    curmodname=enclosingmodule;
    curline=line;
    Object *lobj=lexpr->evaluate();
    Object *robj=rexpr->evaluate();
    if(lobj->getObjType()==ObjType::OBJSTRING&&robj->getObjType()==ObjType::OBJSTRING){
        if(opname=="and")
            return new ObjBoolean(!getString(lobj).empty()&&!getString(robj).empty());
        else if(opname=="or")
            return new ObjBoolean(!getString(lobj).empty()||!getString(robj).empty());
        else
            throw RuntimeError(enclosingmodule,line);
    }
    else if(isNumeric(lobj->getObjType())&&isNumeric(robj->getObjType())){
        if(opname=="and")
            return new ObjBoolean(getNumeric(lobj)&&getNumeric(robj));
        else if(opname=="or")
            return new ObjBoolean(getNumeric(lobj)||getNumeric(robj));
        else
            throw RuntimeError(enclosingmodule,line);
    }
    else
        throw RuntimeError(enclosingmodule,line);
}

/****************************************************************/
/*************************左值变量节点类定义*************************/

ExprLValue::ExprLValue(const string &varname):varname(varname),enclosingmethod(curmethod){}

//int ExprLValue::getExprType(){return ExprType::LVALUE;}

ExprID::ExprID(const string &varname):ExprLValue(varname){}

Object *ExprID::evaluate(){//
    curmodname=enclosingmodule;
    curline=line;
    Object *object=runtimestack.get(varname)->object;
    return object;
}

void ExprID::setObject(Object *object){
    if(runtimestack.exists(varname)){
        Variable *variable=runtimestack.get(varname);
        variable->object=object;
    }
    else{
        Variable *variable=new Variable(varname,object);
        runtimestack.put(varname, variable);
    }
}

ExprArray::ExprArray(const string &varname,Expr *index):ExprLValue(varname),index(index){}

Object *ExprArray::evaluate(){
    curmodname=enclosingmodule;
    curline=line;
    if(runtimestack.exists(varname)){
        Object *object=runtimestack.get(varname)->object;
    //ObjArray *resarray=dynamic_cast<ObjArray *>(object);
        if(object->getObjType()==ObjType::OBJARRAY){
            ObjArray *objarray=dynamic_cast<ObjArray *>(object);
            Object *objindex=index->evaluate();
            if(objindex->getObjType()==ObjType::OBJINTEGER){
                ObjInteger *objinteger=dynamic_cast<ObjInteger *>(objindex);
                if(objinteger->value<objarray->value.size()&&objinteger->value>=0)
                    return objarray->value[objinteger->value];
                else
                    throw RuntimeError(enclosingmodule,line);
            }
            else
                throw RuntimeError(enclosingmodule,line);
        }
        else
            throw RuntimeError(enclosingmodule,line);
    }
    else
        throw RuntimeError(enclosingmodule,line);
}

void ExprArray::setObject(Object *object){
    curmodname=enclosingmodule;
    curline=line;
    if(runtimestack.exists(varname)){
        Variable *variable=runtimestack.get(varname);
        if(variable->object->getObjType()==ObjType::OBJARRAY){
            ObjArray *objarray=dynamic_cast<ObjArray *>(variable->object);
            Object *objindex=index->evaluate();
            if(objindex->getObjType()==ObjType::OBJINTEGER){
                ObjInteger *objinteger=dynamic_cast<ObjInteger *>(variable->object);
                if(objinteger->value<objarray->value.size()&&objinteger->value>=0)
                    objarray->value[objinteger->value]=object;
                else
                    throw RuntimeError(enclosingmodule,line);
            }
            else
                throw RuntimeError(enclosingmodule,line);
        }
        else
            throw RuntimeError(enclosingmodule,line);
    }
    else
        throw RuntimeError(enclosingmodule,line);
}

/****************************************************************/
/*************************常量运算节点类定义*************************/

int ExprConstant::getExprType(){return ExprType::CONST;}

ExprInteger::ExprInteger(int value):value(value){}

Object *ExprInteger::evaluate(){
    curmodname=enclosingmodule;
    curline=line;
    return new ObjInteger(value);
}

ExprFloat::ExprFloat(double value):value(value){}

Object *ExprFloat::evaluate(){
    curmodname=enclosingmodule;
    curline=line;
    return new ObjFloat(value);
}

ExprBoolean::ExprBoolean(bool value):value(value){}

Object *ExprBoolean::evaluate(){
    curmodname=enclosingmodule;
    curline=line;
    return new ObjBoolean(value);
}

ExprString::ExprString(const string &value):value(value){}

Object *ExprString::evaluate(){
    curmodname=enclosingmodule;
    curline=line;
    return new ObjString(value);
}

Object *ExprArrayInit::evaluate(){
    curmodname=enclosingmodule;
    curline=line;
    ObjArray *objarray=new ObjArray();
    int type=0;
    for(int i=0;i<initlist.size();++i){
        Object *object=initlist[i]->evaluate();
        objarray->value.push_back(initlist[i]->evaluate());
    }
    return objarray;
}

/****************************************************************/
/*************************函数调用节点类定义*************************/

ExprMethodCall::ExprMethodCall(const string &methodname):methodname(methodname){}

Object *ExprMethodCall::evaluate(){
    //通过调用函数名寻找函数定义，未来这块要修改
    curmodname=enclosingmodule;
    curline=line;
    int pos=methodname.find(".");
    string firstname=methodname.substr(0,pos);
    if(runtimestack.exists(firstname)){//名字首部分是变量
        //vector<string> namearray=nameSplit(methodname, ".");
        if(methodname!=firstname){//名字不只有一部分
            Object *object=runtimestack.get(firstname)->object;
            if(object->getObjType()==ObjType::OBJCLASS){//名字引用类对象
                string namep=methodname.substr(pos+1);
                if(namep.find(".")==namep.npos){
                    ObjClass *objclass=dynamic_cast<ObjClass *>(object);//转化为类对象
                    if(objclass->fieldmap.count(namep)){
                        StackFrame *newstackframe=new StackFrame();
                        for(int i=0;i<arglist.size();++i){
                            Object *obj=arglist[i]->evaluate();
                            Variable *variable=new Variable(objclass->paralist[i],obj);//参数传递
                            newstackframe->variabletable[objclass->paralist[i]]=variable;
                        }
                        runtimestack.push(newstackframe);
                        objclass->methodmap[namep]->intepret();
                        runtimestack.pop();
                        Object *returnobj=objclass->methodmap[namep]->returnobj;
                        objclass->methodmap[namep]->returnobj=NULL;
                        return returnobj;
                    }
                    else
                        throw RuntimeError(enclosingmodule, line);
                }
                else
                    throw RuntimeError(enclosingmodule, line);
            }
            else
                throw RuntimeError(enclosingmodule, line);
        }
        else
            throw RuntimeError(enclosingmodule, line);
    }
    else{//名字首部分不是变量
        if(firstname==methodname){//名字只有一个部分
            int type=symboltable.getDeclType(methodname);
            if(type==DeclType::DECLMETHOD){//该名字必须是全局函数
                DeclMethod *declmethod=symboltable.getDeclMethod(methodname);
                StackFrame *newstackframe=new StackFrame();
                for(int i=0;i<arglist.size();++i){
                    Object *object=arglist[i]->evaluate();
                    Variable *variable=new Variable(declmethod->paralist[i],object);//参数传递
                    newstackframe->variabletable[declmethod->paralist[i]]=variable;
                }
                runtimestack.push(newstackframe);
                declmethod->intepret();
                runtimestack.pop();
                
                Object *returnobj=declmethod->returnobj;
                declmethod->returnobj=NULL;
                return returnobj;
            }
            else
                throw RuntimeError(enclosingmodule, line);
        }
        else{//名字不只有一部分
            int rpos=methodname.rfind(".");
            string lastname=methodname.substr(rpos+1);
            string namep=methodname.substr(0,rpos-1);
            int type1=symboltable.getDeclType(namep);
            int type2=symboltable.getDeclType(lastname);
            if(type1==DeclType::DECLMODULE&&type2==DeclType::DECLMETHOD){
                DeclMethod *declmethod=symboltable.getDeclMethod(methodname);
                StackFrame *newstackframe=new StackFrame();
                for(int i=0;i<arglist.size();++i){
                    Object *object=arglist[i]->evaluate();
                    Variable *variable=new Variable(declmethod->paralist[i],object);//参数传递
                    newstackframe->variabletable[declmethod->paralist[i]]=variable;
                }
                runtimestack.push(newstackframe);
                declmethod->intepret();
                runtimestack.pop();
                
                Object *returnobj=declmethod->returnobj;
                declmethod->returnobj=NULL;
                return returnobj;
            }
            else
                throw RuntimeError(enclosingmodule, line);
        }
    }
}


ExprInput::ExprInput(const string &tip):tip(tip){}

Object *ExprInput::evaluate(){
    cout<<tip;
    string input;
    getline(cin,input);
    return new ObjString(input);
}

/****************************************************************/
/*************************语句节点类定义*************************/

Statement::Statement():enclosingmethod(curmethod){}

StmtBlock::StmtBlock():continuepoint(false),breakpoint(false){}

void StmtBlock::execute(){
    for(int i=0;i<statements.size();++i){
        if(enclosingmethod->returnobj!=NULL) break;
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
    /*肯定是左值表达式，转化为基类，根据实际类型动态选择setObject函数*/
    ExprLValue *exprlvalue=dynamic_cast<ExprLValue *>(lexpr);
    Object *object=rexpr->evaluate();
    exprlvalue->setObject(object);
}

StmtMethodCall::StmtMethodCall(ExprMethodCall *methodcall):methodcall(methodcall){}

void StmtMethodCall::execute(){methodcall->evaluate();}

StmtIf::StmtIf():elseblock(NULL){}

void StmtIf::execute(){
    Object *object=condition->evaluate();
    if(isNumeric(object->getObjType())){
        if(getNumeric(object))
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
            if(!flag)
                if(elseblock)
                    elseblock->execute();
        }
    }
    else
        throw RuntimeError(enclosingmodule,curline);
}

StmtElif::StmtElif(Expr *condition,StmtBlock *elifblock)
    :condition(condition),elifblock(elifblock),executed(false){}

void StmtElif::execute(){
    Object *object=condition->evaluate();
    if(isNumeric(object->getObjType()))
    {
        if(getNumeric(object))
            elifblock->execute();
        else
            throw RuntimeError(enclosingmodule,curline);
    }
    else
        throw RuntimeError(enclosingmodule,curline);
}

StmtWhile::StmtWhile(Expr *condition,StmtBlock *whileblock):condition(condition),whileblock(whileblock){}

void StmtWhile::execute(){
    while(true){
        Object *object=condition->evaluate();
        if(isNumeric(object->getObjType())){
            if(getNumeric(object))
                whileblock->execute();
            else
                break;
        }
        else
            throw RuntimeError(enclosingmodule,curline);
    }
}

void StmtFor::execute(){
//    ExprID *exprid=new ExprID(targetname);
//    exprid->enclosingmodule=enclosingmodule;
//    exprid->line=line;
    if(targetname.find(".")==targetname.npos){
        if(range){
            if(range->step>0){
                for(int i=range->begin;i<range->end;i+=range->step){
                    Variable *variable=new Variable(targetname,new ObjInteger(i));
                    runtimestack.put(targetname, variable);
                    forblock->execute();
                }
            }
            else if(range->step<0){
                for(int i=range->begin;i>range->end;i-=range->step){
                    Variable *variable=new Variable(targetname,new ObjInteger(i));
                    runtimestack.put(targetname, variable);
                    forblock->execute();
                }
            }
            else
                throw RuntimeError(enclosingmodule,line);
        }
        else{
            if(runtimestack.exists(objectname)){
                Object *object=runtimestack.get(objectname)->object;
                if(object->getObjType()==ObjType::OBJARRAY){
                    ObjArray *objarray=dynamic_cast<ObjArray *>(object);
                    for(int i=0;i<objarray->value.size();++i){
                        Variable *variable=new Variable(targetname,objarray->value[i]);
                        runtimestack.put(targetname, variable);
                        forblock->execute();
                    }
                }
                else
                    throw RuntimeError(enclosingmodule,line);
            }
            else
                throw RuntimeError(enclosingmodule,line);
        }
    }
    else
        throw RuntimeError(enclosingmodule,line);
}

StmtReturn::StmtReturn(Expr *ret):ret(ret){}

void StmtReturn::execute(){
    Object *object=dynamic_cast<Object *>(ret->evaluate());
    enclosingmethod->returnobj=object;
}

void StmtBreak::execute(){
    enclosingmethod->methodblock->breakpoint=true;
}

void StmtContinue::execute(){
    enclosingmethod->methodblock->continuepoint=true;
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
        throw RuntimeError(enclosingmodule, line);
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

Object::Object(){
    enclosingmodule=curmodname;
    line=curline;
}

ObjInteger::ObjInteger(int value):value(value){}

int ObjInteger::getObjType(){return ObjType::OBJINTEGER;}

//Object *ObjInteger::getValue(){return new ObjInteger(value);}

void ObjInteger::print(){cout<<value;}


ObjFloat::ObjFloat(int value):value(value){}

int ObjFloat::getObjType(){return ObjType::OBJFLOAT;}

//Object *ObjFloat::getValue(){return new ObjFloat(value);}

void ObjFloat::print(){cout<<value;}


ObjBoolean::ObjBoolean(bool value):value(value){}

int ObjBoolean::getObjType(){return ObjType::OBJBOOLEAN;}

//Object *ObjBoolean::getValue(){return new ObjBoolean(value);}

void ObjBoolean::print(){cout<<value;}


ObjString::ObjString(string value):value(value){}

int ObjString::getObjType(){return ObjType::OBJSTRING;}

//Object *ObjString::getValue(){return new ObjString(value);}

void ObjString::print(){cout<<value;}

ObjArray::ObjArray(){}

int ObjArray::getObjType(){return ObjType::OBJARRAY;}

//Object *ObjArray::getValue(){
//    ObjArray *arr=new ObjArray();
//    arr->value=value;
//}

void ObjArray::print(){
    cout<<"[";
    for(int i=0;i<value.size();++i){
        value[i]->getValue()->print();
        if(i<value.size()-1)
            cout<<",";
    }
    cout<<"]";
}

int ObjClass::getObjType(){return ObjType::OBJCLASS;}

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
    if(iter==key.end()){
        switch(state){
            case 1:
                return DeclType::DECLMODULE;
            case 2:
                return DeclType::DECLCLASS;
            case 3:
                return DeclType::DECLMETHOD;
        }
    }
    else return -1;
}

Declaration *SymbolTable::getDeclaration(const string &key){
    vector<string> keyarray=nameSplit(key, ".");
    ObjectClass *
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

DeclClass *SymbolTable::getDeclClass(const string &key){
    vector<string> keyarray=nameSplit(key, ".");
    Object
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
                else
                    throw ExecutiveError(curmodname, curline)；
            }
                break;
            case 2:
                throw ExecutiveError(curmodname, curline)；
                break;
            default:
                break;
        }
    }
}


Variable::Variable(const string &varname,Object *value):varname(varname),object(object){}

bool RuntimeStack::exists(const string &key){
    if(key.find(".")==key.npos){
        for(auto stackframe=stackframelist.rbegin();stackframe!=stackframelist.rend();++stackframe){
            if((*stackframe)->variabletable.count(*iter))
                return true;
        }
        return false;
    }
    else
        throw RuntimeError(curmodname, curline);
}
    /*
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
    Object *keytype=(*stackframe)->variabletable[*iter]->object;
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
     */
}

Variable *RuntimeStack::get(const string &key){
    if(key.find(".")!=key.npos)
        throw RuntimeError(curmodname, curline);
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
    if()
}

