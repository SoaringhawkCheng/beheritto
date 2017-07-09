#include "astree.h"

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

static vector<string> nameSplit(const string &name,const string &pattern){
    vector<string> namearray;
    string namep=name+pattern;
    int pos=namep.find(pattern);
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
        throw RuntimeError(object->modname,object->line);
}

static int getInteger(Object *object){
    ObjInteger *objinteger=dynamic_cast<ObjInteger *>(object);
    return objinteger->value;
}

static string getString(Object *object){
    ObjString *objstring=dynamic_cast<ObjString *>(object);
    return objstring->value;
}

/****************************************************************/
/*************************运算表达式节点类定义*************************/

int Expression::getNodeType(){return NodeType::NODEEXPR;}

/****************************************************************/
/*************************一元运算符节点类定义*************************/

ExprOpUnary::ExprOpUnary(Expression *expr):expr(expr){}

int ExprOpUnary::getExprType(){return ExprType::EXPROPUN;}

ExprOpposite::ExprOpposite(Expression *expr):ExprOpUnary(expr){}

Object *ExprOpposite::evaluate(){
    curmodname=modname;
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
    throw RuntimeError(modname,line);
}

ExprNot::ExprNot(Expression *expr):ExprOpUnary(expr){}

Object *ExprNot::evaluate(){
    curmodname=modname;
    curline=line;
    Object *object=expr->evaluate();
    return new ObjBoolean(getNumeric(object)!=0);
}

/****************************************************************/
/*************************二元运算节点类定义*************************/

int ExprOpBinary::getExprType(){return ExprType::EXROPBIN;}

ExprOpBinary::ExprOpBinary(const string &opname,Expression *lexpr,Expression *rexpr)
    :opname(opname),lexpr(lexpr),rexpr(rexpr){}

ExprArith::ExprArith(const string &opname,Expression *lexpr,Expression *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Object *ExprArith::evaluate(){
    curmodname=modname;
    curline=line;
    Object *lobject=lexpr->evaluate();
    Object *robject=rexpr->evaluate();
    if(lobject->getObjType()==ObjType::OBJSTRING){
        if(robject->getObjType()==ObjType::OBJSTRING&&opname=="+")
            return new ObjString(getString(lobject)+getString(robject));
        else if(robject->getObjType()==ObjType::OBJINTEGER&&opname=="*"){
            int objinteger=getInteger(robject);
            if(objinteger>0){
                string objstring;
                for(int i=0;i<objinteger;++i)
                    objstring+=getString(lobject);
                return new ObjString(objstring);
            }
            else
                throw RuntimeError(modname,line);
        }
        else
            throw RuntimeError(modname,line);
    }
    else if(isNumeric(lobject->getObjType())&&isNumeric(robject->getObjType())){
        ObjType objtype;
        if(lobject->getObjType()==ObjType::OBJFLOAT||robject->getObjType()==ObjType::OBJFLOAT)
            objtype=ObjType::OBJFLOAT;
        else objtype=ObjType::OBJINTEGER;
        if(opname=="+"){
            if(objtype==ObjType::OBJFLOAT)
                return new ObjFloat(getNumeric(lobject)+getNumeric(robject));
            else
                return new ObjInteger(getNumeric(lobject)+getNumeric(robject));
        }
        else if(opname=="-"){
            if(objtype==ObjType::OBJFLOAT)
                return new ObjFloat(getNumeric(lobject)-getNumeric(robject));
            else
                return new ObjInteger(getNumeric(lobject)-getNumeric(robject));
        }
        else if(opname=="*"){
            if(objtype==ObjType::OBJFLOAT)
                return new ObjFloat(getNumeric(lobject)*getNumeric(robject));
            else
                return new ObjInteger(getNumeric(lobject)*getNumeric(robject));
        }
        else if(opname=="/"){
            if(getNumeric(robject)){
                if(objtype==ObjType::OBJFLOAT)
                    return new ObjFloat(getNumeric(lobject)/getNumeric(robject));
                else
                    return new ObjInteger(getNumeric(lobject)/getNumeric(robject));
            }
            else
                throw RuntimeError(modname,line);
                
        }
        else if(opname=="%"){
            if(lobject->getObjType()==ObjType::OBJINTEGER
               &&robject->getObjType()==ObjType::OBJINTEGER
               &&getNumeric(robject))
                return new ObjInteger(getInteger(lobject)%getInteger(robject));
            else
                throw RuntimeError(modname,line);
        }
        else
            throw RuntimeError(modname,line);
    }
    else
        throw RuntimeError(modname,line);
}

ExprBitwise::ExprBitwise(const string &opname,Expression *lexpr,Expression *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Object *ExprBitwise::evaluate(){
    curmodname=modname;
    curline=line;
    Object *lobject=lexpr->evaluate();
    Object *robject=rexpr->evaluate();
    if(lobject->getObjType()!=ObjType::OBJINTEGER||robject->getObjType()!=ObjType::OBJINTEGER)
        throw RuntimeError(curmodname,  curline);
    if(getInteger(robject)<0) throw RuntimeError(curmodname,  curline);
    if(opname=="<<")
        return new ObjInteger(getInteger(lobject)<<getInteger(robject));
    else if(opname==">>")
        return new ObjInteger(getInteger(lobject)>>getInteger(robject));
    else
        throw RuntimeError(modname,line);
}

ExprCompare::ExprCompare(const string &opname,Expression *lexpr,Expression *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Object *ExprCompare::evaluate(){
    curmodname=modname;
    curline=line;
    Object *lobject=lexpr->evaluate();
    Object *robject=rexpr->evaluate();
    if(lobject->getObjType()==ObjType::OBJSTRING&&robject->getObjType()==ObjType::OBJSTRING){
        if(opname=="==")
            return new ObjBoolean(getString(lobject)==getString(robject));
        else if(opname=="!=")
            return new ObjBoolean(getString(lobject)==getString(robject));
        else if(opname==">")
            return new ObjBoolean(getString(lobject)==getString(robject));
        else if(opname==">=")
            return new ObjBoolean(getString(lobject)==getString(robject));
        else if(opname=="<")
            return new ObjBoolean(getString(lobject)==getString(robject));
        else if(opname=="<=")
            return new ObjBoolean(getString(lobject)==getString(robject));
        else
            throw RuntimeError(modname,line);
    }
    else if(isNumeric(lobject->getObjType())&&isNumeric(robject->getObjType())){
        if(opname=="==")
            return new ObjBoolean(getNumeric(lobject)==getNumeric(robject));
        else if(opname=="!=")
            return new ObjBoolean(getNumeric(lobject)!=getNumeric(robject));
        else if(opname==">")
            return new ObjBoolean(getNumeric(lobject)>getNumeric(robject));
        else if(opname==">=")
            return new ObjBoolean(getNumeric(lobject)>=getNumeric(robject));
        else if(opname=="<")
            return new ObjBoolean(getNumeric(lobject)<getNumeric(robject));
        else if(opname=="<=")
            return new ObjBoolean(getNumeric(lobject)<=getNumeric(robject));
        else
            throw RuntimeError(modname,line);
    }
    else
        throw RuntimeError(modname,line);
}

ExprLogic::ExprLogic(const string &opname,Expression *lexpr,Expression *rexpr)
    :ExprOpBinary(opname,lexpr,rexpr){}

Object *ExprLogic::evaluate(){
    curmodname=modname;
    curline=line;
    Object *lobject=lexpr->evaluate();
    Object *robject=rexpr->evaluate();
    if(lobject->getObjType()==ObjType::OBJSTRING&&robject->getObjType()==ObjType::OBJSTRING){
        if(opname=="and")
            return new ObjBoolean(!getString(lobject).empty()&&!getString(robject).empty());
        else if(opname=="or")
            return new ObjBoolean(!getString(lobject).empty()||!getString(robject).empty());
        else
            throw RuntimeError(modname,line);
    }
    else if(isNumeric(lobject->getObjType())&&isNumeric(robject->getObjType())){
        if(opname=="and")
            return new ObjBoolean(getNumeric(lobject)&&getNumeric(robject));
        else if(opname=="or")
            return new ObjBoolean(getNumeric(lobject)||getNumeric(robject));
        else
            throw RuntimeError(modname,line);
    }
    else
        throw RuntimeError(modname,line);
}

/****************************************************************/
/*************************左值变量节点类定义*************************/

ExprLValue::ExprLValue(const string &varname):varname(varname),enclosingmethod(curmethod){}

int ExprLValue::getExprType(){return ExprType::EXPRLVALUE;}

ExprID::ExprID(const string &varname):ExprLValue(varname){}

Object *ExprID::evaluate(){//
    curmodname=modname;
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

ExprArray::ExprArray(const string &varname,Expression *index):ExprLValue(varname),index(index){}

Object *ExprArray::evaluate(){
    curmodname=modname;
    curline=line;
    if(runtimestack.exists(varname)){
        Object *object=runtimestack.get(varname)->object;
        if(object->getObjType()==ObjType::OBJARRAY){
            ObjArray *objarray=dynamic_cast<ObjArray *>(object);
            Object *objindex=index->evaluate();
            if(objindex->getObjType()==ObjType::OBJINTEGER){
                ObjInteger *objinteger=dynamic_cast<ObjInteger *>(objindex);
                if(objinteger->value<objarray->value.size()&&objinteger->value>=0)
                    return objarray->value[objinteger->value];
                else
                    throw RuntimeError(modname,line);
            }
            else
                throw RuntimeError(modname,line);
        }
        else
            throw RuntimeError(modname,line);
    }
    else
        throw RuntimeError(modname,line);
}

void ExprArray::setObject(Object *object){
    curmodname=modname;
    curline=line;
    if(runtimestack.exists(varname)){
        Variable *variable=runtimestack.get(varname);
        if(variable->object->getObjType()==ObjType::OBJARRAY){
            ObjArray *objarray=dynamic_cast<ObjArray *>(variable->object);
            Object *objindex=index->evaluate();
            if(objindex->getObjType()==ObjType::OBJINTEGER){
                ObjInteger *objintindex=dynamic_cast<ObjInteger *>(objindex);
                if(objintindex->value>=0&&objintindex->value<objarray->value.size())
                    objarray->value[objintindex->value]=object;
                else
                    throw RuntimeError(modname,line);
            }
            else
                throw RuntimeError(modname,line);
        }
        else
            throw RuntimeError(modname,line);
    }
    else
        throw RuntimeError(modname,line);
}

/****************************************************************/
/*************************常量运算节点类定义*************************/

int ExprConstant::getExprType(){return ExprType::EXPRCONSTANT;}

ExprInteger::ExprInteger(int value):value(value){}

Object *ExprInteger::evaluate(){
    curmodname=modname;
    curline=line;
    return new ObjInteger(value);
}

ExprFloat::ExprFloat(double value):value(value){}

Object *ExprFloat::evaluate(){
    curmodname=modname;
    curline=line;
    return new ObjFloat(value);
}

ExprBoolean::ExprBoolean(bool value):value(value){}

Object *ExprBoolean::evaluate(){
    curmodname=modname;
    curline=line;
    return new ObjBoolean(value);
}

ExprString::ExprString(const string &value):value(value){}

Object *ExprString::evaluate(){
    curmodname=modname;
    curline=line;
    return new ObjString(value);
}

int ExprArrayInit::getExprType(){return ExprType::EXPRARRAYINIT;}

Object *ExprArrayInit::evaluate(){
    curmodname=modname;
    curline=line;
    ObjArray *objarray=new ObjArray();
    for(int i=0;i<initlist.size();++i){
        objarray->value.push_back(initlist[i]->evaluate());
    }
    return objarray;
}

/****************************************************************/
/*************************函数调用节点类定义*************************/

int ExprMethodCall::getExprType(){return ExprType::EXPRMETHODCALL;}

ExprMethodCall::ExprMethodCall(const string &methodname)
    :methodname(methodname),enclosingmodule(curmodule),enclosingclass(curclass){}

Object *ExprMethodCall::evaluate(){
    //通过调用函数名寻找函数定义，未来这块要修改
    curmodname=modname;
    curline=line;
    int pos=methodname.find(".");
    string firstname=methodname.substr(0,pos);
    if(runtimestack.exists(firstname)){//名字首部分是变量
        //vector<string> namearray=nameSplit(methodname, ".");
        if(methodname!=firstname){//名字不只有一部分
            Object *object=runtimestack.get(firstname)->object;
            if(object->getObjType()==ObjType::OBJCLASS){//名字引用类对象
                ObjClass *objclass=dynamic_cast<ObjClass *>(object);//转化为类对象
                string namep=methodname.substr(pos+1);
                if(namep.find(".")==namep.npos){
                    if(objclass->methodlist.count(namep)){
                        StackFrame *newstackframe=new StackFrame();
                        DeclMethod *declmethod=dynamic_cast<DeclMethod *>(objclass->methodlist[namep]);
                        if(arglist.size()==declmethod->paralist.size()){
                            for(int i=0;i<arglist.size();++i){
                                Object *obj=arglist[i]->evaluate();
                                Variable *variable=new Variable(declmethod->paralist[i],obj);//参数传递
                                newstackframe->variabletable[declmethod->paralist[i]]=variable;
                            }
                            for(auto iter=(objclass->fieldlist).begin();iter!=(objclass->fieldlist).end();++iter){
                                Variable *variable=new Variable(iter->first,iter->second);
                                newstackframe->variabletable[iter->first]=variable;
                            }
                            runtimestack.push(newstackframe);
                            declmethod->intepret();
                            runtimestack.pop();
                            Object *returnobj=declmethod->returnobj;
                            declmethod->returnobj=NULL;
                            return returnobj;
                        }
                        else throw RuntimeError(modname, line);
                    }
                    else throw RuntimeError(modname, line);
                }
                else throw RuntimeError(modname, line);
            }
            else throw RuntimeError(modname, line);
        }
        else throw RuntimeError(modname, line);
    }
    else{//名字首部分不是变量
        Declaration *declaration=symboltable.get(methodname, enclosingmodule, enclosingclass);
        if(declaration->getDeclType()==DeclType::DECLCLASS){
            DeclClass *declclass=dynamic_cast<DeclClass *>(declaration);
            ObjClass *objclass=new ObjClass();
            if(arglist.size()==declclass->paralist.size()){
                objclass->paralist=declclass->paralist;
                objclass->methodlist=declclass->methodlist;
                StackFrame *newstackframe=new StackFrame();
                for(int i=0;i<arglist.size();++i){
                    Variable *variable=new Variable(objclass->paralist[i],arglist[i]->evaluate());
                    newstackframe->variabletable.insert(make_pair(objclass->paralist[i],variable));
                }
                runtimestack.push(newstackframe);
                for(int i=0;i<declclass->fieldlist.size();++i){
                    StmtAssign *stmtassign=declclass->fieldlist[i]->assign;
                    if(stmtassign->lexpr->getExprType()==ExprType::EXPRLVALUE){
                        ExprLValue *exprlvalue=dynamic_cast<ExprLValue *>(stmtassign->lexpr);
                        if(exprlvalue->varname[0]=='_'||exprlvalue->varname.substr(1).find("_")==string::npos)
                            objclass->fieldlist.insert(make_pair(exprlvalue->varname, stmtassign->rexpr->evaluate()));
                        else throw RuntimeError(modname, line);
                    }
                    else throw RuntimeError(modname, line);
                }
                runtimestack.pop();
                return objclass;
            }
            else throw RuntimeError(modname, line);
        }
        else if(declaration->getDeclType()==DeclType::DECLMETHOD){
            DeclMethod *declmethod=dynamic_cast<DeclMethod *>(declaration);
            if(arglist.size()==declmethod->paralist.size()){
                StackFrame *newstackframe=new StackFrame();
                runtimestack.push(newstackframe);
                for(int i=0;i<arglist.size();++i){
                    if(declmethod->paralist[i].find("_")==string::npos){
                        Variable *variable=new Variable(declmethod->paralist[i],arglist[i]->evaluate());
                        runtimestack.put(declmethod->paralist[i], variable);
                    }
                    else throw RuntimeError(modname, line);
                }
                declmethod->intepret();
                Object *returnobj=declmethod->returnobj;
                declmethod->returnobj=NULL;
                runtimestack.pop();
                return returnobj;
            }
            else throw RuntimeError(modname, line);
        }
        else throw RuntimeError(modname, line);
    }
}

ExprInput::ExprInput(const string &tip):tip(tip){}

int ExprInput::getExprType(){return ExprType::EXPRINPUT;}

Object *ExprInput::evaluate(){
    cout<<tip;
    string input;
    getline(cin,input);
    if(input.empty()) throw RuntimeError(modname, line);
    else{
        if(input.size()==2&&input[0]=='\"'&&input[1]=='\"')
            return new ObjString("");
        if(input.size()>2&&input[0]=='\"'&&input[input.size()-1]=='\"')
            return new ObjString(input.substr(1,input.size()-1));
        if(input[0]=='-'&&input.size()<2)
            throw RuntimeError(modname, line);
        int sign=1;
        if(input[0]=='-'){
            sign=-1;
            input=input.substr(1);
        }
        int countstop=0;
        for(auto ch:input){
            if(ch=='.') ++countstop;
            else if(!isdigit(ch)) throw RuntimeError(modname, line);
        }
        if(countstop>1||countstop==input.size()) throw RuntimeError(modname, line);
        if(countstop) return new ObjFloat(sign*atof(input.c_str()));
        else return new ObjInteger(sign*atoi(input.c_str()));
    }
    return new ObjString(input);
}

int ExprRange::getExprType(){return ExprType::EXPRRANGE;}

Object *ExprRange::evaluate(){
    ObjArray *objarray=new ObjArray();
    Object *objbegin=begin->evaluate();
    Object *objend=end->evaluate();
    Object *objstep=step->evaluate();
    if(objbegin->getObjType()==ObjType::OBJINTEGER){
        if(objend->getObjType()==ObjType::OBJINTEGER){
            if(objstep->getObjType()==ObjType::OBJINTEGER){
                ObjInteger *objintbegin=dynamic_cast<ObjInteger *>(objbegin);
                ObjInteger *objintend=dynamic_cast<ObjInteger *>(objend);
                ObjInteger *objintstep=dynamic_cast<ObjInteger *>(objstep);
                if(objintstep->value>0){
                    for(int i=objintbegin->value;i<objintend->value;i+=objintstep->value)
                        objarray->value.push_back(new ObjInteger(i));
                    return objarray;
                }
                else if(objintstep->value<0){
                    for(int i=objintbegin->value;i>objintend->value;i+=objintstep->value)
                        objarray->value.push_back(new ObjInteger(i));
                    return objarray;
                }
                else throw RuntimeError(curmodname, curline);
            }
            else throw RuntimeError(curmodname, curline);
        }
        else throw RuntimeError(curmodname, curline);
    }
    else throw RuntimeError(curmodname, curline);
}

/****************************************************************/
/*************************语句节点类定义*************************/

Statement::Statement():enclosingmethod(curmethod){}

int Statement::getNodeType(){return NodeType::NODESTMT;}

StmtBlock::StmtBlock():continuepoint(false),breakpoint(false){}

int StmtBlock::getStmtType(){return StmtType::STMTBLOCK;}

void StmtBlock::execute(){
    for(int i=0;i<statements.size();++i){
        if(enclosingmethod&&enclosingmethod->returnobj!=NULL) break;
        statements[i]->execute();
        if(breakpoint==true) break;
        else if(continuepoint==true){
            continuepoint=false;
            continue;
        }
    }
}

StmtAssign::StmtAssign(){}

StmtAssign::StmtAssign(Expression *lexpr,Expression *rexpr):lexpr(lexpr),rexpr(rexpr){}

int StmtAssign::getStmtType(){return StmtType::STMTASSIGN;}

void StmtAssign::execute(){
    /*肯定是左值表达式，转化为基类，根据实际类型动态选择setObject函数*/
    ExprLValue *exprlvalue=dynamic_cast<ExprLValue *>(lexpr);
    Object *object=rexpr->evaluate();
    exprlvalue->setObject(object);
}

StmtMethodCall::StmtMethodCall(ExprMethodCall *methodcall):methodcall(methodcall){}

int StmtMethodCall::getStmtType(){return StmtType::STMTMETHODCALL;}

void StmtMethodCall::execute(){methodcall->evaluate();}

StmtIf::StmtIf():elseblock(NULL){}

int StmtIf::getStmtType(){return StmtType::STMTIF;}

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
        throw RuntimeError(modname,curline);
}

StmtElif::StmtElif(Expression *condition,StmtBlock *elifblock)
    :condition(condition),elifblock(elifblock),executed(false){}

int StmtElif::getStmtType(){return StmtType::STMTELIF;}

void StmtElif::execute(){
    Object *object=condition->evaluate();
    if(isNumeric(object->getObjType()))
    {
        if(getNumeric(object))
            elifblock->execute();
    }
    else
        throw RuntimeError(modname,curline);
}

StmtWhile::StmtWhile(Expression *condition,StmtBlock *whileblock):condition(condition),whileblock(whileblock){}

int StmtWhile::getStmtType(){return StmtType::STMTWHILE;}

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
            throw RuntimeError(modname,curline);
    }
}

int StmtFor::getStmtType(){return StmtType::STMTFOR;}

void StmtFor::execute(){
    if(targetname.find(".")==targetname.npos){
        if(range){
            ObjArray *objarray=dynamic_cast<ObjArray *>(range->evaluate());
            for(auto objinteger:objarray->value){
                Variable *variable=new Variable(targetname,objinteger);
                runtimestack.put(targetname, variable);
                forblock->execute();
            }
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
                    throw RuntimeError(modname,line);
            }
            else
                throw RuntimeError(modname,line);
        }
    }
    else
        throw RuntimeError(modname,line);
}

StmtReturn::StmtReturn(Expression *ret):ret(ret){}

int StmtReturn::getStmtType(){return StmtType::STMTRETURN;}

void StmtReturn::execute(){
    Object *object=dynamic_cast<Object *>(ret->evaluate());
    enclosingmethod->returnobj=object;
}

int StmtBreak::getStmtType(){return StmtType::STMTBREAK;}

void StmtBreak::execute(){
    enclosingmethod->methodblock->breakpoint=true;
}

void StmtContinue::execute(){
    enclosingmethod->methodblock->continuepoint=true;
}

int StmtContinue::getStmtType(){return StmtType::STMTCONTINUE;}

void StmtPrint::execute(){
    for(int i=0;i<printlist.size();++i){
        printlist[i]->evaluate()->print();
    }
}

int StmtPrint::getStmtType(){return StmtType::STMTPRINT;}

void StmtPrintLn::execute(){
    for(int i=0;i<printlist.size();++i){
        printlist[i]->evaluate()->print();
    }
    cout<<endl;
}

int StmtPrintLn::getStmtType(){return StmtType::STMTPRINTLN;}

/****************************************************************/
/*************************声明节点类定义*************************/

int Declaration::getNodeType(){return NodeType::NODEEXPR;}

DeclModule::DeclModule(const string &modname):modname(modname),entry(NULL){}

int DeclModule::getDeclType(){return DeclType::DECLMODULE;}

void DeclModule::intepret(){
    runtimestack.push(new StackFrame());
    if(entry)
        entry->intepret();
    else if(runtimestack.stackframelist.size()==1)
        throw RuntimeError(modname, line);
    runtimestack.pop();
}

DeclClass::DeclClass(const string &classname):classname(classname),enclosingmodule(curmodule),enclosingclass(curclass){}

int DeclClass::getDeclType(){return DeclType::DECLCLASS;}

void DeclClass::intepret(){
    for(auto field:fieldlist)
        field->intepret();
}

DeclMethod::DeclMethod(const string &methodname)
    :methodname(methodname),enclosingmodule(curmodule),enclosingclass(curclass),returnobj(NULL){}

int DeclMethod::getDeclType(){return DeclType::DECLMETHOD;}

void DeclMethod::intepret(){methodblock->execute();}

DeclField::DeclField(StmtAssign *assign):assign(assign){}

int DeclField::getDeclType(){return DeclType::DECLFIELD;}

void DeclField::intepret(){assign->execute();}

int DeclEntry::getDeclType(){return DeclType::DECLENTRY;}

void DeclEntry::intepret(){
    for(auto statement:statements)
        statement->execute();
}

/****************************************************************/
/**************************运算结果节点类定义*************************/

Object::Object(){
    modname=curmodname;
    line=curline;
}

int Object::getNodeType(){return NodeType::NODEOBJ;}

ObjInteger::ObjInteger(int value):value(value){}

void ObjInteger::toString(){
    cout<<value;
}

int ObjInteger::getObjType(){return ObjType::OBJINTEGER;}

void ObjInteger::print(){cout<<value;}


ObjFloat::ObjFloat(double value):value(value){}

void ObjFloat::toString(){
    cout<<value;
}

int ObjFloat::getObjType(){return ObjType::OBJFLOAT;}

void ObjFloat::print(){cout<<value;}

ObjBoolean::ObjBoolean(bool value):value(value){}

void ObjBoolean::toString(){
    cout<<value;
}

int ObjBoolean::getObjType(){return ObjType::OBJBOOLEAN;}

void ObjBoolean::print(){
    if(value) cout<<"true";
    else cout<<"false";
}

ObjString::ObjString(string value):value(value){}

void ObjString::toString(){
    cout<<value;
}

int ObjString::getObjType(){return ObjType::OBJSTRING;}

void ObjString::print(){cout<<value;}

ObjArray::ObjArray(){}

void ObjArray::toString(){
    for(auto obj:value){
        obj->toString();
        cout<<" ";
    }
    cout<<endl;
}

int ObjArray::getObjType(){return ObjType::OBJARRAY;}

void ObjArray::print(){
    cout<<"[";
    for(int i=0;i<value.size();++i){
        value[i]->print();
        if(i<value.size()-1)
            cout<<",";
    }
    cout<<"]";
}

void ObjClass::toString(){
    
}

int ObjClass::getObjType(){return ObjType::OBJCLASS;}

void ObjClass::print(){throw RuntimeError(modname, line);}

/****************************************************************/
/*************************环境变量节点类定义*************************/

Declaration *SymbolTable::get(const string &key, DeclModule *enclosingmodule, DeclClass *enclosingclass){
    vector<string> keyarray=nameSplit(key, ".");
    if(keyarray.size()==1){//名字只有一个部分
        /*先搜索同类中的类方法*/
        if(enclosingclass&&enclosingclass->methodlist.count(key))
            return enclosingclass->methodlist[key];
        /*再搜索同模块中类名*/
        else if(enclosingmodule&&enclosingmodule->classlist.count(key))
            return enclosingmodule->classlist[key];
        /*再搜索同模块中全局函数*/
        else if(enclosingmodule&&enclosingmodule->methodlist.count(key))
            return enclosingmodule->methodlist[key];
        return NULL;
    }
    else{//名字有多个部分
        string firstkey=keyarray[0];
        DeclModule *declmodule=enclosingmodule;
        DeclClass *declclass=NULL;
        DeclMethod *declmethod=NULL;
        int state=1;
        int i;
        for(i=0;i<keyarray.size();++i){
            switch(state){
                case 1:{
                    if(declmodule->modulelist.count(keyarray[i])){
                        declmodule=declmodule->modulelist[keyarray[i]];
                        state=1;
                    }
                    else if(declmodule->classlist.count(keyarray[i])){
                        declclass=declmodule->classlist[keyarray[i]];
                        state=2;
                    }
                    else if(declmodule->methodlist.count(keyarray[i])){
                        declmethod=declmodule->methodlist[keyarray[i]];
                        state=3;
                    }
                    else return NULL;
                    break;
                }
                case 2:
                    return NULL;
                case 3:
                    return NULL;
                default:
                    break;
            }
        }
        if(i==keyarray.size()){
            switch(state){
                case 2:
                    return declclass;
                case 3:
                    return declmethod;
                default:
                    return NULL;
            }
        }
        else return NULL;
    }
}

Variable::Variable(const string &varname,Object *object):varname(varname),object(object){}

void RuntimeStack::toString(){
    for(auto stackframe=stackframelist.rbegin();stackframe!=stackframelist.rend();++stackframe){
        //for(auto iter=((*stackframe)->variabletable).begin());iter!=((*stackframe)->variabletable).end());iter++){
        for(auto iter=((*stackframe)->variabletable).begin();iter!=((*stackframe)->variabletable).end();++iter){
            cout<<iter->first<<" ";
            iter->second->object->toString();
            cout<<",";
        }
        cout<<endl;
    }
}

bool RuntimeStack::exists(const string &key){
    if(key.find(".")==key.npos){
        for(auto stackframe=stackframelist.rbegin();stackframe!=stackframelist.rend();++stackframe){
            if((*stackframe)->variabletable.count(key))
                return true;
        }
        return false;
    }
    else
        throw RuntimeError(curmodname, curline);
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
        throw RuntimeError(curmodname, curline);
    return (*stackframe)->variabletable[key];
}

void RuntimeStack::put(const string &key, Variable *variable){
    if(key.find(".")!=key.npos)
        throw RuntimeError(curmodname, curline);
    (stackframelist.back())->variabletable[key]=variable;
}

void RuntimeStack::push(StackFrame *newstackframe){
    stackframelist.push_back(newstackframe);
}

void RuntimeStack::pop(){
    stackframelist.pop_back();
}
