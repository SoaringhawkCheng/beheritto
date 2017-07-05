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
    case NodeType::OBJINTEGER:
    case NodeType::_FLOAT:
    case NodeType::BOOLEAN:
        return true;
    default:
        return false;
    }
}

static bool isConstant(int nodetype){
    switch(nodetype){
    case NodeType::OBJINTEGER:
    case NodeType::_FLOAT:
    case NodeType::BOOLEAN:
    case NodeType::OBJSTRING:
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

static double getNumeric(Object *object){
    if(object->getObjType()==NodeType::OBJINTEGER){
        ObjInteger *res=dynamic_cast<ObjInteger *>(object->getValue());
        return res->value;
    }
    if(object->getObjType()==NodeType::_FLOAT){
        ObjFloat *res=dynamic_cast<ObjFloat *>(object->getValue());
        return res->value;
    }
    else{//if(object->getObjType()==NodeType::BOOLEAN)
        ObjBoolean *res=dynamic_cast<ObjBoolean *>(object->getValue());
        return res->value;
    }
}

static int getInteger(Object *object){
    ObjInteger *resint=dynamic_cast<ObjInteger *>(object->getValue());
    return resint->value;
}

static double getFloat(Object *object){
    ObjFloat *resfloat=dynamic_cast<ObjFloat *>(object->getValue());
    return resfloat->value;
}

static bool getBoolean(Object *object){
    ObjBoolean *resboolean=dynamic_cast<ObjBoolean *>(object->getValue());
    return resboolean->value;
}

static string getString(Object *object){
    ObjString *resstring=dynamic_cast<ObjString *>(object->getValue());
    return resstring->value;
}

/****************************************************************/
/*************************表达式节点类定义*************************/

ASTree::ASTree(){}

Expr::Expr():symboltable(cursymboltable){}

/****************************************************************/
/*************************一元运算符节点类定义*************************/

ExprOpUnary::ExprOpUnary(Expr *expr):expr(expr){}

int ExprOpUnary::getExprType(){return ExprType::OPBIN;}

ExprOpposite::ExprOpposite(Expr *expr):ExprOpUnary(expr){}

//Type *ExprOpposite::analyzeSemantic(){
//    Type *type=expr->analyzeSemantic();
//    if(isNumeric(type->getObjType()))
//        return type;
//    else if(type->getObjType()==NodeType::WILDCARD){
//        return new TypeWildcard();
//    }
//    else
//        throw SemanticError(curmodname, curline);
//}

Object *ExprOpposite::evaluate(){
    Object *res=expr->evaluate();
    if(res->getObjType()==NodeType::OBJINTEGER){
        ObjInteger *resinteger=dynamic_cast<ObjInteger *>(res);
        return new ObjInteger(-1*resinteger->value);
    }
    if(res->getObjType()==NodeType::_FLOAT){
        ObjFloat *resinteger=dynamic_cast<ObjFloat *>(res);
        return new ObjFloat(-1*resinteger->value);
    }
    if(res->getObjType()==NodeType::BOOLEAN){
        ObjBoolean *resboolean=dynamic_cast<ObjBoolean *>(res);
        return new ObjBoolean(-1*resboolean->value);
    }
    throw ExecutiveError(curmodname, curline);
}

ExprNot::ExprNot(Expr *expr):ExprOpUnary(expr){}

//Type *ExprNot::analyzeSemantic(){
//    Type *type=expr->analyzeSemantic();
//    if(isNumeric(type->getObjType()))
//        return type;
//    else if(type->getObjType()==NodeType::WILDCARD)
//        return new TypeWildcard();
//    else
//        throw SemanticError(curmodname, curline);
//}

Object *ExprNot::evaluate(){
    Object *res=expr->evaluate();
    return new ObjBoolean(getNumeric(res)!=0);
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

//Type *ExprArith::analyzeSemantic(){
//    Type *ltype=lexpr->analyzeSemantic();
//    Type *rtype=rexpr->analyzeSemantic();
//    /*操作数类型必须都是通配符或者常量*/
//    if(ltype->getObjType()!=NodeType::WILDCARD&&!isConstant(ltype->getObjType()))
//        throw SemanticError(enclosingmodule, line);
//    else if(rtype->getObjType()!=NodeType::WILDCARD&&!isConstant(rtype->getObjType()))
//        throw SemanticError(enclosingmodule, line);
//    /*操作数都是数字*/
//    else if(isNumeric(ltype->getObjType())&&isNumeric(rtype->getObjType()))
//        return new TypeWildcard();
//    /*操作数都是字符串*/
//    else if(ltype->getObjType()==NodeType::OBJSTRING&&rtype->getObjType()==NodeType::OBJSTRING)
//        return new TypeString();
//    /*操作数有通配符*/
//    else if(ltype->getObjType()==NodeType::WILDCARD||rtype->getObjType()==NodeType::WILDCARD)
//        return new TypeWildcard();
//    else
//        throw SemanticError(enclosingmodule, line);
//    //delete ltype;
//    //delete rtype;
//}

Object *ExprArith::evaluate(){
    Object *lres=lexpr->evaluate();
    Object *rres=rexpr->evaluate();
    if(lres->getObjType()==NodeType::OBJSTRING){
        if(rres->getObjType()==NodeType::OBJSTRING&&opname=="+")
            return new ObjString(getString(lres)+getString(rres));
        else if(rres->getObjType()==NodeType::OBJINTEGER&&opname=="*"){
            int resinteger=getInteger(rres);
            if(resinteger>0){
                string resstring;
                for(int i=0;i<resinteger;++i)
                    resstring+=getString(lres);
                return new ObjString(resstring);
            }
            else
                throw ExecutiveError(enclosingmodule, line);
        }
        else
            throw ExecutiveError(enclosingmodule, line);
    }
    else if(isNumeric(lres->getObjType())&&isNumeric(rres->getObjType())){
        NodeType restype;
        if(lres->getObjType()==NodeType::_FLOAT||rres->getObjType()==NodeType::_FLOAT)
            restype=NodeType::_FLOAT;
        else restype=NodeType::OBJINTEGER;
        if(opname=="+"){
            if(restype==NodeType::_FLOAT)
                return new ObjFloat(getNumeric(lres)+getNumeric(rres));
            else
                return new ObjInteger(getNumeric(lres)+getNumeric(rres));
        }
        else if(opname=="-"){
            if(restype==NodeType::_FLOAT)
                return new ObjFloat(getNumeric(lres)-getNumeric(rres));
            else
                return new ObjInteger(getNumeric(lres)-getNumeric(rres));
        }
        else if(opname=="*"){
            if(restype==NodeType::_FLOAT)
                return new ObjFloat(getNumeric(lres)*getNumeric(rres));
            else
                return new ObjInteger(getNumeric(lres)*getNumeric(rres));
        }
        else if(opname=="/"){
            if(getNumeric(rres)){
                if(restype==NodeType::_FLOAT)
                    return new ObjFloat(getNumeric(lres)/getNumeric(rres));
                else
                    return new ObjInteger(getNumeric(lres)/getNumeric(rres));
            }
            else
                throw ExecutiveError(enclosingmodule, line);
                
        }
        else if(opname=="%"){
            if(lres->getObjType()==NodeType::OBJINTEGER
               &&rres->getObjType()==NodeType::OBJINTEGER
               &&getNumeric(rres))
                return new ObjInteger(getInteger(lres)%getInteger(rres));
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

//Type *ExprBitwise::analyzeSemantic(){
//    Type *ltype=lexpr->analyzeSemantic();
//    Type *rtype=rexpr->analyzeSemantic();
//    /*左操作数是整数，右操作数与左操作数等价*/
//    if(ltype->getObjType()==NodeType::OBJINTEGER&&ltype->isEquivalent(rtype))
//        return ltype;
//    /*右操作数是整数，左操作数与右操作数等价*/
//    else if(rtype->getObjType()==NodeType::OBJINTEGER&&rtype->isEquivalent(ltype))
//        return rtype;
//    /*都是通配符*/
//    else if(ltype->getObjType()!=NodeType::WILDCARD&&rtype->getObjType()!=NodeType::WILDCARD)
//        return new TypeInteger();
//    else
//        throw SemanticError(enclosingmodule, line);
//}

Object *ExprBitwise::evaluate(){
    Object *lres=lexpr->evaluate();
    Object *rres=lexpr->evaluate();
    if(lres->getObjType()!=NodeType::OBJINTEGER||rres->getObjType()!=NodeType::OBJINTEGER)
        throw ExecutiveError(enclosingmodule, line);
    if(getInteger(rres)<0) throw ExecutiveError(enclosingmodule, line);
    if(opname=="<<")
        return new ObjInteger(getInteger(lres)<<getInteger(rres));
    else if(opname==">>")
        return new ObjInteger(getInteger(lres)>>getInteger(rres));
    else
        throw ExecutiveError(enclosingmodule, line);
}


ExprCompare::ExprCompare(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}


//Type *ExprCompare::analyzeSemantic(){
//    Type *ltype=lexpr->analyzeSemantic();
//    Type *rtype=rexpr->analyzeSemantic();
//    /*操作数必须是常量或者通配符*/
//    if(ltype->getObjType()!=NodeType::WILDCARD&&!isConstant(ltype->getObjType()))
//        throw SemanticError(enclosingmodule, line);
//    else if(rtype->getObjType()!=NodeType::WILDCARD&&!isConstant(rtype->getObjType()))
//        throw SemanticError(enclosingmodule, line);
//    /*数字与数字比较*/
//    if(isNumeric(ltype->getObjType())&&isNumeric(rtype->getObjType()))
//        return new TypeBoolean();
//    /*字符串与字符串比较*/
//    else if(ltype->getObjType()==NodeType::OBJSTRING&&rtype->getObjType()==NodeType::OBJSTRING)
//        return new TypeBoolean();
//    /*至少有一个通配符*/
//    else if(ltype->getObjType()==NodeType::WILDCARD||rtype->getObjType()==NodeType::WILDCARD)
//        return new TypeBoolean();
//    else
//        throw SemanticError(enclosingmodule, line);
//}

Object *ExprCompare::evaluate(){
    Object *lres=lexpr->evaluate();
    Object *rres=rexpr->evaluate();
    if(lres->getObjType()==NodeType::OBJSTRING&&rres->getObjType()==NodeType::OBJSTRING){
        if(opname=="==")
            return new ObjBoolean(getString(lres)==getString(rres));
        else if(opname=="!=")
            return new ObjBoolean(getString(lres)==getString(rres));
        else if(opname==">")
            return new ObjBoolean(getString(lres)==getString(rres));
        else if(opname==">=")
            return new ObjBoolean(getString(lres)==getString(rres));
        else if(opname=="<")
            return new ObjBoolean(getString(lres)==getString(rres));
        else if(opname=="<=")
            return new ObjBoolean(getString(lres)==getString(rres));
        else
            throw ExecutiveError(enclosingmodule, line);
    }
    else if(isNumeric(lres->getObjType())&&isNumeric(rres->getObjType())){
        if(opname=="==")
            return new ObjBoolean(getNumeric(lres)==getNumeric(rres));
        else if(opname=="!=")
            return new ObjBoolean(getNumeric(lres)!=getNumeric(rres));
        else if(opname==">")
            return new ObjBoolean(getNumeric(lres)>getNumeric(rres));
        else if(opname==">=")
            return new ObjBoolean(getNumeric(lres)>=getNumeric(rres));
        else if(opname=="<")
            return new ObjBoolean(getNumeric(lres)<getNumeric(rres));
        else if(opname=="<=")
            return new ObjBoolean(getNumeric(lres)<=getNumeric(rres));
        else
            throw ExecutiveError(enclosingmodule, line);
    }
    else
        throw ExecutiveError(enclosingmodule, line);
}

ExprLogic::ExprLogic(const string &opname,Expr *lexpr,Expr *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

//Type *ExprLogic::analyzeSemantic(){
//    Type *ltype=lexpr->analyzeSemantic();
//    Type *rtype=rexpr->analyzeSemantic();
//    //左操作数是常量，右操作数与左操作数等价
//    if(isConstant(ltype->getObjType())&&ltype->isEquivalent(rtype))
//        return new TypeBoolean();
//    //右操作数是常量，左操作数与右操作数等价
//    else if(isConstant(rtype->getObjType())&&rtype->isEquivalent(ltype))
//        return new TypeBoolean();
//    //都是通配符
//    else if(ltype->getObjType()==NodeType::WILDCARD&&rtype->getObjType()==NodeType::WILDCARD)
//        return new TypeBoolean();
//    else
//        throw ExecutiveError(enclosingmodule, line);
//}

Object *ExprLogic::evaluate(){
    Object *lres=lexpr->evaluate();
    Object *rres=rexpr->evaluate();
    if(lres->getObjType()==NodeType::OBJSTRING&&rres->getObjType()==NodeType::OBJSTRING){
        if(opname=="and")
            return new ObjBoolean(!getString(lres).empty()&&!getString(rres).empty());
        else if(opname=="or")
            return new ObjBoolean(!getString(lres).empty()||!getString(rres).empty());
        else
            throw ExecutiveError(enclosingmodule, line);
    }
    else if(isNumeric(lres->getObjType())&&isNumeric(rres->getObjType())){
        if(opname=="and")
            return new ObjBoolean(getNumeric(lres)&&getNumeric(rres));
        else if(opname=="or")
            return new ObjBoolean(getNumeric(lres)||getNumeric(rres));
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

//Type *ExprID::analyzeSemantic(){
//    Type *type=symboltable->get(varname);
//    return type;
//}

//void ExprID::setType(Type *type){
//    if(enclosingmethod){//是函数中的变量
//        Type *t=symboltable->get(enclosingmethod->methodname);
//        TypeMethod *typemethod=dynamic_cast<TypeMethod *>(t);
//        //是函数形参名，设置形参类型
//        if(typemethod->paramap.count(varname))
//            typemethod->paramap[varname]=type;
//    }
//    if(symboltable->exists(varname))
//        symboltable->set(varname, type);
//    else
//        symboltable->put(varname, type);
//}

Object *ExprID::evaluate(){//
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

//Type *ExprArray::analyzeSemantic(){
//    Type *type=symboltable->get(varname);
//    if(type->getObjType()==NodeType::ARRAY){
//        TypeArray *typearray=dynamic_cast<TypeArray *>(type);
//        if(isNumeric(index->analyzeSemantic()->getObjType())
//           ||index->analyzeSemantic()->getObjType()==NodeType::WILDCARD)
//           return typearray->arraytype;
//        else
//            throw SemanticError(enclosingmodule, line);
//    }
//    else
//        throw SemanticError(enclosingmodule, line);
//}

Object *ExprArray::evaluate(){
    Object *object=runtimestack.get(varname)->object;
    //ObjArray *resarray=dynamic_cast<ObjArray *>(object);
    if(object->getObjType()==NodeType::ARRAY){
        ObjArray *resarray=dynamic_cast<ObjArray *>(object);
        Object *resindex=index->evaluate();
        if(resindex->getObjType()==NodeType::OBJINTEGER){
            ObjInteger *resinteger=dynamic_cast<ObjInteger *>(resindex);
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

//void ExprArray::setType(Type *type){
//    //?
//}

void ExprArray::setObject(Object *object){
    if(runtimestack.exists(varname)){
        Variable *variable=runtimestack.get(varname);
        //?要不要进行类型检查
        if(variable->object->getObjType()==NodeType::ARRAY){
            ObjArray *resarray=dynamic_cast<ObjArray *>(variable->object);
            Object *resindex=index->evaluate();
            if(resindex->getObjType()==NodeType::OBJINTEGER){
                ObjInteger *resinteger=dynamic_cast<ObjInteger *>(variable->object);
                if(resinteger->value<resarray->value.size()&&resinteger->value>=0){
                    if(isNumeric(resarray->value[0]->getObjType())&&isNumeric(object->getObjType()))
                        resarray->value[resinteger->value]=object;
                    else if(resarray->value[0]->getObjType()==NodeType::OBJSTRING
                            &&object->getObjType()==NodeType::OBJSTRING)
                        resarray->value[resinteger->value]=object;
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

//Type *ExprInteger::analyzeSemantic(){
//    return new TypeInteger();
//}

Object *ExprInteger::evaluate(){
    return new ObjInteger(value);
}

ExprFloat::ExprFloat(double value):value(value){}

//Type *ExprFloat::analyzeSemantic(){
//    return new TypeFloat();
//}

Object *ExprFloat::evaluate(){
    return new ObjFloat(value);
}

ExprBoolean::ExprBoolean(bool value):value(value){}

//Type *ExprBoolean::analyzeSemantic(){
//    return new TypeBoolean();
//}

Object *ExprBoolean::evaluate(){
    return new ObjBoolean(value);
}

ExprString::ExprString(const string &value):value(value){}

//Type *ExprString::analyzeSemantic(){
//    return new TypeString();
//}

Object *ExprString::evaluate(){
    return new ObjString(value);
}

//Type *ExprArrayInit::analyzeSemantic(){
//    Type *type1=initlist[0]->analyzeSemantic();
//    TypeArray *typearray=new TypeArray();
//    for(int i=0;i<initlist.size();++i){
//        Type *type2=initlist[i]->analyzeSemantic();
//        if(!type1->isEquivalent(type2))
//            throw SemanticError(enclosingmodule, line);
//    }
//    typearray->arraytype=type1;
//    typearray->size=initlist.size();
//    return typearray;
//}

Object *ExprArrayInit::evaluate(){
    ObjArray *resarray=new ObjArray();
    int type=0;
    for(int i=0;i<initlist.size();++i){
        Object *object=initlist[i]->evaluate();
        if(i==0) type=object->getObjType();
        else if(type!=object->getObjType())
            throw ExecutiveError(enclosingmodule, line);
        resarray->value.push_back(initlist[i]->evaluate());
    }
    return resarray;
}

/****************************************************************/
/*************************函数调用节点类定义*************************/

ExprMethodCall::ExprMethodCall(const string &methodname):methodname(methodname){}

//Type *ExprMethodCall::analyzeSemantic(){
//    Type *type=symboltable->get(methodname);
//    if(type->getObjType()==NodeType::METHOD){
//        TypeMethod *typemethod=dynamic_cast<TypeMethod *>(type);
//        if(arglist.size()==typemethod->paramap.size()){
//            unordered_map<string, Type *>::iterator iter;
//            int index=0;
//            for(auto iter=typemethod->paramap.begin();iter!=typemethod->paramap.end();++iter,++index){
//                Type *type1=arglist[index]->analyzeSemantic();//实参类型
//                Type *type2=iter->second;//形参类型
//                //if(type2->getObjType()==NodeType::WILDCARD){
//                type2=type1;//设置形参类型
//                typemethod->paramap[iter->first]=type1;//更新实参类型
//                //if(type1->isEquivalent(t2))
//                //    throw SemanticError(enclosingmodule, line);
//                
//            }
//            return typemethod->returntype;
//        }
//        else
//            throw SemanticError(enclosingmodule, line);
//    }
//    else if(type->getObjType()==NodeType::_CLASS){
//        TypeClass *typeclass=dynamic_cast<TypeClass *>(type);
//        if(arglist.size()==typeclass->fieldmap.size()){
//            unordered_map<string, Type *>::iterator iter;
//            int index=0;
//            for(auto iter=typeclass->paramap.begin();iter!=typeclass->paramap.end();++iter,++index){
//                Type *type1=arglist[index]->analyzeSemantic();
//                Type *type2=iter->second;
//                type2=type1;//设置形参类型
//                typeclass->paramap[iter->first]=type1;//更新实参类型
//            }
//            return new TypeClass();
//            program
//        }
//        else
//            throw SemanticError(enclosingmodule, line);
//            
//    }
//    else
//        throw SemanticError(enclosingmodule, line);
//    return NULL;
//}

Object *ExprMethodCall::evaluate(){
    //通过调用函数名寻找函数定义，未来这块要修改
    if(runtimestack.exists(methodname)){
        Object *object=runtimestack.get(methodname)->object;
        if(object->getObjType()==NodeType::METHOD){
            Procedure *proc=procedures[methodname];
            StackFrame *newstackframe=new StackFrame();
            for(int i=0;i<arglist.size();++i){
                Object *res=arglist[i]->evaluate();
                Variable *variable=new Variable(proc->method->paralist[i],object);//参数传递
                newstackframe->variabletable[proc->method->paralist[i]]=variable;
            }
            runtimestack.push(newstackframe);
            proc->method->methodblock->execute();
            runtimestack.pop();
            
            Object *resreturn=proc->method->resret;
            proc->method->resret=NULL;//??????????
            return resreturn;
        }
        else if(object->getObjType()==NodeType::_CLASS){
            ObjectObject *resobject=dynamic_cast<ObjectObject *>(object);
            for(int i=0;i<arglist.size();++i){
                Object *res=arglist[i]->evaluate();
                Variable *variable=new Variable(resobject->paralist[i],res);
                resobject->member.insert(make_pair(resobject->paralist[i], variable));
            }
            return resobject;
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

//void StmtBlock::analyzeSemantic(){
//    for(int i=0;i<statements.size();++i)
//        statements[i]->analyzeSemantic();
//}

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

//void StmtAssign::analyzeSemantic(){
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
    Object *object=rexpr->evaluate();
    exprlvalue->setObject(object->getValue());
}

StmtMethodCall::StmtMethodCall(ExprMethodCall *methodcall):methodcall(methodcall){}

//void StmtMethodCall::analyzeSemantic(){methodcall->analyzeSemantic();}

void StmtMethodCall::execute(){methodcall->evaluate();}

StmtIf::StmtIf():elseblock(NULL){}

//void StmtIf::analyzeSemantic(){
//    Type *type=condition->analyzeSemantic();
//    if(type->isEquivalent(new TypeBoolean())){
//        ifblock->analyzeSemantic();
//        for(int i=0;i<eliflist.size();++i)
//            eliflist[i]->analyzeSemantic();
//        if(elseblock)
//            elseblock->analyzeSemantic();
//    }
//}

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

//void StmtElif::analyzeSemantic(){
//    Type *type=condition->analyzeSemantic();
//    if(isNumeric(type->getObjType())&&type->getObjType()==NodeType::WILDCARD)
//    
//}

void StmtElif::execute(){
    Object *object=condition->evaluate();
    if(isNumeric(object->getObjType()))
    {
        if(getNumeric(object))
            elifblock->execute();
        else
            throw ExecutiveError(enclosingmodule, line);
    }
    else
        throw ExecutiveError(enclosingmodule, line);
}

StmtWhile::StmtWhile(Expr *condition,StmtBlock *whileblock):condition(condition),whileblock(whileblock){}

//void StmtWhile::analyzeSemantic(){
//    Type *type=condition->analyzeSemantic();
//    if(type->isEquivalent(new TypeBoolean()))
//        whileblock->analyzeSemantic();
//}

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
            throw ExecutiveError(enclosingmodule, line);
    }
}

//void StmtFor::analyzeSemantic(){

//}

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
    Object *object=dynamic_cast<Object *>(ret->evaluate());
    enclosingmethod->resret=object;
}

//void StmtBreak::analyzeSemantic(){
//    if(enclosingloop==NULL)
//        throw SemanticError(curmodname, curline);
//}

void StmtBreak::execute(){
    enclosingmethod->methodblock->breakpoint=true;
}

//void StmtContinue::analyzeSemantic(){
//    if(enclosingloop==NULL)
//        throw SemanticError(curmodname, curline);
//}

void StmtContinue::execute(){
    enclosingmethod->methodblock->continuepoint=true;
}

StmtInput::StmtInput(Expr *lvalue):lvalue(lvalue){}

//void StmtInput::analyzeSemantic(){
//    if(lvalue->getExprType()!=ExprType::LVALUE)
//        throw SemanticError(curmodname, curline);
//}

void StmtInput::execute(){
    
}

//void StmtPrint::analyzeSemantic(){
//    if(printlist.size()>0){
//        for(int i=0;i<printlist.size();++i)
//            printlist[i]->analyzeSemantic();
//    }
//    else
//        throw SemanticError(curmodname, curline);
//}

void StmtPrint::execute(){
    for(int i=0;i<printlist.size();++i){
        printlist[i]->evaluate()->print();
    }
}
/****************************************************************/
/*************************声明节点类定义*************************/

Declaration::Declaration():symboltable(symboltable){}

DeclModule::DeclModule(const string &modname):modname(modname){}

//void DeclModule::analyzeSemantic(){
//    for(auto iter=modulelist.begin();iter!=modulelist.end();++iter)
//        iter->second->analyzeSemantic();
//    for(auto iter=classlist.begin();iter!=classlist.end();++iter)
//        iter->second->analyzeSemantic();
//    for(auto iter=classlist.begin();iter!=classlist.end();++iter)
//        iter->second->analyzeSemantic();
//}

void DeclModule::intepret(){
    runtimestack.push(new StackFrame());
    if(entry)
        entry->intepret();
    else
        throw ExecutiveError(modname, line);
    runtimestack.pop();
}

DeclClass::DeclClass(const string &classname):classname(classname){}

//void DeclClass::analyzeSemantic(){
//    for(int i=0;i<fieldlist.size();++i)
//        fieldlist[i]->analyzeSemantic();
//    for(auto iter=methodlist.begin();iter!=methodlist.end();++iter)
//        iter->second->analyzeSemantic();
//}

void DeclClass::intepret(){
    //stackframe.push(new Runtimestack());
    for(int i=0;i<fieldlist.size();++i)
        fieldlist[i]->intepret();
    //stackframe.pop();
}

DeclMethod::DeclMethod(const string &methodname):methodname(methodname){}

//void DeclMethod::analyzeSemantic(){
//    TypeMethod *typemethod=new TypeMethod();
//    typemethod->returntype=new TypeWildcard();
//    for(int i=0;i<paralist.size();++i){
//        if(!typemethod->paramap.count(paralist[i])){
//            typemethod->paramap[paralist[i]]=new TypeWildcard();
//            methodblock->symboltable->put(paralist[i], new TypeWildcard());
//        }
//        else throw SemanticError(enclosingmodule, line);
//    }
//    symboltable->put(methodname, typemethod);
//    methodblock->analyzeSemantic();
//}

void DeclMethod::intepret(){methodblock->execute();}

DeclField::DeclField(StmtAssign *assign):assign(assign){}

//void DeclField::analyzeSemantic(){assign->analyzeSemantic();}

void DeclField::intepret(){assign->execute();}

/****************************************************************/
/*************************类型类节点类定义*************************/

//int TypeWildcard::getObjType(){return NodeType::WILDCARD;}
//
//bool TypeWildcard::isEquivalent(Type *type){return true;}
//
//int TypeInteger::getObjType(){return NodeType::OBJINTEGER;}
//
//bool TypeInteger::isEquivalent(Type *type){
//    return type->getObjType()==NodeType::OBJINTEGER
//    ||type->getObjType()==NodeType::WILDCARD;
//}
//
//int TypeFloat::getObjType(){return NodeType::_FLOAT;}
//
//bool TypeFloat::isEquivalent(Type *type){
//    return type->getObjType()==NodeType::_FLOAT
//    ||type->getObjType()==NodeType::WILDCARD;
//}
//
//int TypeBoolean::getObjType(){return NodeType::BOOLEAN;}
//
//bool TypeBoolean::isEquivalent(Type *type){
//    return type->getObjType()==NodeType::BOOLEAN
//    ||type->getObjType()==NodeType::WILDCARD;
//}
//
//int TypeString::getObjType(){return NodeType::OBJSTRING;}
//
//bool TypeString::isEquivalent(Type *type){
//    return type->getObjType()==NodeType::OBJSTRING
//    ||type->getObjType()==NodeType::WILDCARD;
//}
//
//int TypeMethod::getObjType(){return NodeType::METHOD;}
//
//bool TypeMethod::isEquivalent(Type *type){
//    if(type->getObjType()==NodeType::METHOD){
//        TypeMethod *typemethod=dynamic_cast<TypeMethod *>(type);
//        if(typemethod->returntype->isEquivalent(returntype))
//            return true;
//        else return false;
//    }
//    else return false;
//}
//
//int TypeClass::getObjType(){return NodeType::_CLASS;}
//
//bool TypeClass::isEquivalent(Type *type){
////    if(type->getObjType()==NodeType::_CLASS){
////        TypeClass *typeclass=dynamic_cast<TypeClass *>(type);
////        
////    }
//    return true;
//}
//
//int TypeModule::getObjType(){return NodeType::MODULE;}

/****************************************************************/
/**************************运算结果节点类定义*************************/

ObjInteger::ObjInteger(int value):value(value){}

int ObjInteger::getObjType(){return NodeType::OBJINTEGER;}

Object *ObjInteger::getValue(){return new ObjInteger(value);}

void ObjInteger::print(){cout<<value;}


ObjFloat::ObjFloat(int value):value(value){}

int ObjFloat::getObjType(){return NodeType::_FLOAT;}

Object *ObjFloat::getValue(){return new ObjFloat(value);}

void ObjFloat::print(){cout<<value;}


ObjBoolean::ObjBoolean(bool value):value(value){}

int ObjBoolean::getObjType(){return NodeType::BOOLEAN;}

Object *ObjBoolean::getValue(){return new ObjBoolean(value);}

void ObjBoolean::print(){cout<<value;}


ObjString::ObjString(string value):value(value){}

int ObjString::getObjType(){return NodeType::OBJSTRING;}

Object *ObjString::getValue(){return new ObjString(value);}

void ObjString::print(){cout<<value;}


int ObjArray::getObjType(){return NodeType::ARRAY;}

Object *ObjArray::getValue(){
    ObjArray *arr=new ObjArray();
    arr->value=value;
}

void ObjArray::print(){
    cout<<"[";
    for(int i=0;i<value.size();++i){
        value[i]->getValue()->print();
        if(i<value.size()-1)
            cout<<",";
    }
    cout<<"]";
}

int ObjectObject::getObjType(){return NodeType::_CLASS;}

//Object *ObjectObject::getValue(){
//    ObjectObject *resobject=new ObjectObject();
//    resobject->paralist=paralist;
//    resobject->member=member;
//    return resobject;
//}

//void ObjectObject::print(){}
/****************************************************************/
/*************************环境变量节点类定义*************************/
    
//SymbolTable::SymbolTable(SymbolTable *prev):prev(prev){}
//
//bool SymbolTable::exists(const string& key){
//    vector<string> keyarray=nameSplit(key,".");
//    SymbolTable *table=this;
//    auto iter=keyarray.begin();
//    bool flag=false;
//    for(table=this;table!=NULL;table=table->prev){
//        if(table->symbolmap.count(*iter)){
//            flag=true;
//            break;
//        }
//    }
//    if(!flag) return false;
//    Type *keytype=table->symbolmap[*iter];
//    ++iter;
//    while(iter!=keyarray.end()){
//        if(keytype->getObjType()==NodeType::MODULE){
//            TypeModule *typemodule=dynamic_cast<TypeModule *>(keytype);
//            if(typemodule->modulemap.count(*iter)){
//                keytype=typemodule->modulemap[*iter];
//                ++iter;
//                continue;
//            }
//            if(typemodule->classmap.count(*iter)){
//                keytype=typemodule->classmap[*iter];
//                ++iter;
//                continue;
//            }
//            if(typemodule->methodmap.count(*iter)){
//                keytype=typemodule->methodmap[*iter];
//                ++iter;
//                break;
//            }
//            return false;
//        }
//        else if(keytype->getObjType()==NodeType::_CLASS){
//            TypeClass *typeclass=dynamic_cast<TypeClass *>(keytype);
//            if(typeclass->methodmap.count(*iter)){
//                keytype=typeclass->methodmap[*iter];
//                ++iter;
//                break;
//            }
//            if(typeclass->methodmap.count(*iter)){
//                keytype=typeclass->fieldmap[*iter];
//                ++iter;
//                break;
//            }
//            return false;
//        }
//    }
//    if(iter==keyarray.end()) return keytype;
//    else throw SemanticError(curmodname, curline);
//
//}
//
//void SymbolTable::put(const string &key, Type *type){
//    if(key.find(".")!=key.npos)
//        throw SemanticError(curmodname, curline);
//    if(symbolmap.count(key))
//        throw SemanticError(curmodname, curline);
//    else
//        symbolmap[key]=type;
//}
//
//Type *SymbolTable::get(const string &key){
//    vector<string> keyarray=nameSplit(key,".");
//    SymbolTable *table=this;
//    auto iter=keyarray.begin();
//    bool flag=false;
//    for(table=this;table!=NULL;table=table->prev){
//        if(table->symbolmap.count(*iter)){
//            flag=true;
//            break;
//        }
//    }
//    if(!flag) throw SemanticError(curmodname,curline);
//    Type *keytype=table->symbolmap[*iter];
//    ++iter;
//    while(iter!=keyarray.end()){
//        if(keytype->getObjType()==NodeType::MODULE){
//            TypeModule *typemodule=dynamic_cast<TypeModule *>(keytype);
//            if(typemodule->modulemap.count(*iter)){
//                keytype=typemodule->modulemap[*iter];
//                iter++;
//                continue;
//            }
//            if(typemodule->classmap.count(*iter)){
//                keytype=typemodule->classmap[*iter];
//                iter++;
//                continue;
//            }
//            if(typemodule->methodmap.count(*iter)){
//                keytype=typemodule->methodmap[*iter];
//                iter++;
//                break;
//            }
//            throw SemanticError(curmodname,curline);
//        }
//        else if(keytype->getObjType()==NodeType::_CLASS){
//            TypeClass *typeclass=dynamic_cast<TypeClass *>(keytype);
//            if(typeclass->methodmap.count(*iter)){
//                keytype=typeclass->methodmap[*iter];
//                iter++;
//                break;
//            }
//            if(typeclass->methodmap.count(*iter)){
//                keytype=typeclass->fieldmap[*iter];
//                ++iter;
//                break;
//            }
//            throw SemanticError(curmodname,curline);
//        }
//    }
//    if(iter==keyarray.end()) return keytype;
//    else throw SemanticError(curmodname, curline);
//}
//
//void SymbolTable::set(const string &key, Type *type){
//    //for(auto table=this;table!=NULL;table=table->prev){
//    if(key.find(".")!=key.npos)
//        throw SemanticError(curmodname, curline);
//    else
//        symbolmap[key]=type;
//}

Variable::Variable(const string &varname,Object *value):varname(varname),object(object){}

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
//        for(auto iter=)
        /*
        if(keytype->getObjType()==NodeType::MODULE){
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
        if(keytype->getObjType()==NodeType::_CLASS){
            TypeClass *typeclass=dynamic_cast<TypeClass *>(keytype);
            if(typeclass->methodmap.count(*iter)){
                keytype=typeclass->methodmap[*iter];
                iter++;
                break;
            }
            if(typeclass->methodmap.count(*iter)){
                keytype=typeclass->fieldmap[*iter];
                ++iter;
                break;
            }
            throw SemanticError(curmodname,curline);*/

