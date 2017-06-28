#include "parser.h"

Parser::Parser(Lexer *lexer,ASTree *program)
    :lexer(lexer),program(program){}
~Parser::Parser(){}
/*
bool Parser::isNum(){
    switch(token.type){
        case TokenType::INT:
    case TokenType::FLOAT:
        return true;
    default:
        return false;
    }
}

}*/

bool Parser::isStatement(){
    switch(token.type){
        case TokenType::IF:
        case TokenType::ELIF:
        case TokenType::ELSE:
        case TokenType::WHILE:
        case TokenType::FOR:
        case TokenType::RETURN:
        case TokenType::BREAK:
        case TokenType::CONTINUE:
        case TokenType::INPUT:
        case TokenType::PRINT:
        case TokenType::ID:
            return true;
        default:
            return false;
    }
}

bool Parser::isExpression(){
    switch(token.type){
    case TokenType::ADD:
    case TokenType::SUB:
    case TokenType::MUL:
    case TokenType::DIV:
    case TokenType::MOD:
    case TokenType::SLEFT:
    case TokenType::SRIGHT:
    case TokenType::NEGATION:
    case TokenType::ASSIGN:
    case TokenType::GT:
    case TokenType::LT:
    case TokenType::GE:
    case TokenType::LE:
    case TokenType::EQ:
    case TokenType::DE:
    case TokenType::AND:
    case TokenType::OR:
    case TokenType::NOT:
    case TokenType::ID:
    case TokenType::INT:
    case TokenType::FLOAT:
    case TokenType::STRING:
    case TokenType::TRUE:
    case TokenType::FLASE:
        return true;
    default:
        return false;
    }
}

bool Parser::isNumeric(){
    switch(token.type){
    case TokenType::INT
    case TokenType::FLOAT:
        return true;
    default:
        return false;
    }
}

bool Parser::isCompare(){
    switch(token.type){
    case TokenType::GT:
    case TokenType::LT:
    case TokenType::GE:
    case TokenType::LE:
    case TokenType::EQ:
    case TokenType::DE:
        return true;
    default:
        return false;
    }
}

bool Parser::isarithmetic(){
    switch(token.type){
    case TokenType::ADD:
    case TokenType::SUB:
    case TokenType::MUL:
    case TokenType::DIV:
    case TokenType::MOD:
    case TokenType::SLEFT:
    case TokenType::SRIGHT:
        return true;
    default:
        return false;
    }
}

bool Parser::isBoolean(){
    switch(token.type){
    case TokenType::TRUE:
    case TokenType::FALSE:
        return true;
    default:
        return false;
    }
}

bool Parser::isConstant(){
    switch(token.type){
    case TokenType::INT:
    case TokenType::FLOAT:
    case TokenType::STRING:
    case TokenType::TRUE:
    case TokenType::FALSE:
        return true;
    default:
        return false;
    }
}

/****************************************************************/
/****************语法分析器接口函数****************/

void Parser::process(){
    programParser();
}

/****************************************************************/
/****************程序处理级别函数****************/

void Parser::programParser(){
    while(lexer->nextLine()){
        token=lexer->nextToken();
        switch(token.type){
        case TokenType::FROM:
            fromParser(program);
            break;
        case TokenType::IMPORT:
            DeclModule *declmodule=importParser();
            program->modulelist.push_back(declmodule);
            break;
        case TokenType::CLASS:
            DeclClass *declclass=classParser();
            program->classlist.push_back(declclass);
            break;
        case TokenType::DEF:
            DeclMethod *declmethod=methodParser();
            program->methodlist.push_back(methodlist);
            break;
        case TokenType::IF:
            DeclEntry *declentry=entryParser();
            program->entrylist.push_back(entrylist);
            break;
        default:
            throw SyntacticError(lexer->modname,token.lexeme);
        }
    }
}

DeclModule *Parser::importParser(DeclProgram *program){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        string modname=token.lexeme;
        DeclModule *declmodule=moduleParser();
    }
    else throw SyntacticError(lexer->modname,token.lexeme);
    token=lexer->nextToken();
    if(token.type!=TokenType::EOL)
        throw SyntacticError(lexer->modname,token.lexeme);
    return declmodule;
}

void Parser::fromParser(DeclProgram *program){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        string modname=token.lexeme;
        token==lexer->nextToken();
        if(token.type==TokenType::IMPORT){
            token==lexer->nextToken();
            if(token.type==TokenType::ID){
                string name=token.lexeme;
                if(token.type==TokenType::AS){
                    token=lexer->nextToken;
                    if(token.type==TokenType::ID){
                        string alias=token.lexeme;
                        token=lexer->nextToken();
                        if(token.type==TokenType::EOL){
                            DeclClass *declclass=modClassParser(modname,name);
                            if(declclass==NULL){
                                DeclMethod *declmethod=modMethodParser(modname,name);
                                if(declmethod==NULL) throw LoadingError(modname);
                                declmethod->methodname=alias;
                                program->methodlist.push_back(declmethod);
                            }
                            else{
                                declclass->methodname=alias;
                                program->classlist.push_back(declclass);
                            }
                        }
                        else throw SyntacticError(lexer->modname,token.lexeme);
                    }
                    else throw SyntacticError(lexer->modname,token.lexeme);
                }
                else if(token.type==TokenType::EOL){
                    DeclClass *declclass=modClassParser(modname,name);
                    if(declclass==NULL){
                        DeclMethod *declmethod=modMethodParser(modname,name);
                        if(declmethod==NULL) throw LoadingError(modname);
                        program->methodlist.push_back(declmethod);
                    }
                    else program->classlist.push_back(declclass);
                }
                else throw SyntacticError(lexer->modname,token.lexeme);
            }
            else throw SyntacticError(lexer->modname,token.lexeme);
        }
        else throw SyntacticError(lexer->modname,token.lexeme);
    }
    else throw SyntacticError(lexer->modname,token.lexeme);
}

/****************************************************************/
/***************模块处理级别函数***************/

DeclModule *Parser::moduleParser(String &modname){
    lexerlist.push(lexer);
    lexer=new Lexer(modname+".be");
    DeclModule *declmodule=new DeclModule(modname);
    while(lexer->nextLine()){
        token=lexer->nextToken();
        switch(token.type){
        case TokenType::FROM:
            fromParser(declmodule);
            break;
        case TokenType::IMPORT:
            DeclModule *subdeclmodule=importParser();
            declmodule->modulelist.push_back(submodule);
            break;
        case TokenType::CLASS:
            DeclClass *declclass=classParser();
            declmodule->classlist.push_back(declclass);
            break;
        case TokenType::DEF:
            DeclMethod *declmethod=methodParser();
            declmodule->methodlist.push_back(declmethod);
            break;
        default:break;
        }
    }
    lexer=lexerlist.top();
    lexerlist.pop();
    return declmodule;
}

DeclClass *Parser::modClassParser(const string &modname,const string &classname){
    lexerlist.push(lexer);
    lexer=new Lexer(modname+".be");
    while(lexer->nextLine()){
        if(!line.empty){
            stringstream scin(line);
            string str1,str2;
            scin>>str1>>str2;
            if(str1=="class"&&str2==classname){
                token=lexer->nextToken();
                DeclClass *declclass=classParser();
                lexer=lexerlist.top();
                lexerlist.pop();
                return declclass;
            }
        }
    }
    lexer=lexerlist.top();
    lexerlist.pop();
    return NULL;
}

DeclMethod *Parser::modMethodParser(const string &modname,const string &methodname){
    lexerlist.push(lexer);
    lexer(modname+".be");
    while(lexer->nextLine()){
        if(!line.empty){
            stringstream scin(line);
            string str1,str2;
            scin>>str1>>str2;
            if(str1=="def"&&str2==methodname){
                token=lexer->nextToken();
                DeclMethod *declmethod=methodParser();
                lexer=lexerlist.top();
                lexerlist.pop();
                return declmethod;
            }
        }
    }
    lexer=lexerlist.top();
    lexerlist.pop();
    return NULL;
}

/****************************************************************/
/***************类型处理级别函数***************/

DeclClass *Parser::classParser(){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        string classname=token.lexeme;
        DeclClass *declclass=new DeclClass(classname);
        token=lexer->nextToken();
        if(token.type==TokenType::COlON){
            token=lexer->nextToken();
            if(token.type==TokenType::EOL){
                if(lexer->nextLine()){
                    token=lexer->nextToken();
                    if(token.type==TokenType::INDENT){
                        token=lexer->nextToken();
                        while(token.type!=TokenType::DEDENT){
                            switch(token.type){
                            case TokenType::INIT:
                                declclass->constructor=constructorParser();
                                break;
                            case TokenType::ID:
                                declclass->methodlist.push_back(methodParser());
                                break;
                            default:
                                throw SyntacticError(lexer->modname,token.lexeme);
                            }
                        }
                    }
                    else throw SyntacticError(lexer->modname,token.lexeme);
                }
                else throw SyntacticError(lexer->modname,token.lexeme);
            }
            else throw SyntacticError(lexer->modname,token.lexeme);
        }
        else throw SyntacticError(lexer->modname,token.lexeme);
    }
    else throw SyntacticError(lexer->modname,token.lexeme);
    return declclass;
}

/****************************************************************/
/***************函数处理级别函数***************/

DeclConstructor *Parser::constructorParser(){
    DeclConstructor *declconstructor=new DeclConstructor();
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        paralistParser(declconstructor);
        token=lexer->nextToken();
        if(token.type==TokenType::RPAR){
            token=lexer->nextToken();
            if(token.type==TokenType::COLON){
                token=lexer->nextToken();
                if(token.type==TokenType::EOL){
                    if(lexer->nextLine()){
                        token=lexer->nextToken();
                        if(token.type==TokenType::INDENT)
                            declconstructor->block=constructorBlockParser();
                        else throw SyntacticError(lexer->modname,token.lexeme);
                    }
                    else throw SyntacticError(lexer->modname,token.lexeme);
                }
                else throw SyntacticError(lexer->modname,token,lexeme);
            }
            else throw SyntacticError(lexer->modname,token.lexeme);
        }
        else throw SyntacticError(lexer->modname,token.lexeme);
    }
    else throw SyntacticError(lexer->modname,token.lexeme);
    return declcontructor;
}

DeclMethod *Parser::methodParser(){
    DeclMethod *declmethod=new DeclMethod(token.lexeme);
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        paralistParser(declmethod);
        if(token.type==TokenType::RPAR){
            token=lexer->nextToken();
            if(token.type==TokenType::COLON){
                token=lexer->nextToken();
                if(token.type==TokenType::EOL){
                    if(lexer->nextLine()){
                        token=lexer->nextToken();
                        if(token.type==TokenType::INDENT)
                            declmethod->block=blockParser();
                        else throw SyntacticError(lexer->modname,token.lexeme);
                    }
                    else throw SyntacticError(lexer->modname,token.lexeme);
                }
                else throw SyntacticError(lexer->modname,token.lexeme);
            }
            else throw SyntacticError(lexer->modname,token.lexeme);
        }
        else throw SyntacticError(lexer->modname,token.lexeme);
    }
    else throw SyntacticError(lexer->modname,token.lexeme);
    return declmethod;
}

/****************************************************************/
/***************块处理级别函数***************/

StmtBlock *Parser::constructBlockParser(){//???构造函数机制还是没搞懂
    StackFrame *savedstack=curstack;
    curstack=new StackFrame(curstack);
    StmtBlock *stmtblock=new StmtBlock();
    token=lexer->nextToken();
    while(token.type!=TokenType::DEDENT){
        stmtblock->statements.push_back(statementParser());
        if(lexer->nextLine()) token=lexer->nextToken();
        else throw SyntacticError(lexeme->modname,token.lexeme);
    }
    curstack=savedstack;
    return stmtblock;
}

StmtBlock *Parser::blockParser(){
    StackFrame *savedstack=curstack;
    curstack=new StackFrame(curstack);
    StmtBlock *stmtblock=new StmtBlock();
    token=lexer->nextToken();
    while(token.type!=TokenType::DEDENT){
        stmtblock->statements.push_back(statementParser());
        if(lexer->nextLine()) token=lexer->nextToken();
        else throw SyntacticError(lexeme->modname,token.lexeme);
    }
    curstack=savedstack;
    return stmtblock;
}

Statement *Parser::statementParser(){
    token=lexer->nextToken();
    switch(token.type){
    case TokenType::IF:
        Statement *statement=ifParser();
        break;
    case TokenType::WHILE:
        Statement *statement=whileParser();
        break;
    case TokenType::FOR:
        Statement *statement=forParser();
        break;
    case TokenType::PRINT:
        Statement *statement=printParser();
        break;
    case TokenType::ID:
        Statement *statement=statementPParser();
        break;
    case TokenType::RETURN:
        Statement *statement=returnParser();
        break;
    default:throw SyntacticError(lexer->modname,token.lexeme);
    }
    return statement;
}

/****************************************************************/
/***************语句处理级别函数***************/

Stmtif *Parser::ifParser(){
    StmtIf *stmtif=new StmtIf();
    stmtif->condition=exprParser();
    if(token.type==TokenType::COLON){
        token=lexer->nextToken();
        if(token.type==TokenType::EOL){
            if(lexer->nextLine()) token=lexer->nextToken();
            else throw SyntacticError(lexer->modname,token.lexeme);
            if(token.type==TokenType::INDENT)
                stmtif->block=blockParser();
            else throw SyntacticError(token.lexeme,token.lexeme);
            token=lexer->nextToken();
            while(token.type==TokenType::ELIF){
                stmtif->eliflist.push_back(elifParser());
                token=lexer->nextToken();
            }
            if(token.type==TokenType::ELSE)
                stmtif->else=elseParser();
            else throw SyntacticError(lexer->modname,token.lexeme);
        }
        else throw SyntacticError(lexer->modname,token.lexeme);
    }
    else throw SyntacticError(lexer->modname,token.lexeme);
    return stmtif;
}

StmtElif *Parser::elifParser(){
    StmtElif *stmtelif=new StmtElif();
    stmtelif->condition=exprParser();
    if(token.type==TokenType::COLON){
        token=lexer->nextToken();
        if(token.type==TokenType::EOL){
            if(lexer->nextLine()) token=lexer->nextToken();
            else throw SyntacticError(lexer->modname,token.lexeme);
            if(token.type==TokenType::INDENT)
                stmtelif->block=blockParser();
            else throw SyntacticError(lexer->modname,token.lexeme);
        }
        else throw SyntacticError(lexer->modname,token.lexeme);
    }
    else throw SyntacticError(lexer->modname,token.lexeme);
    return stmtelif;
}

StmtElse *Parser::elseParser(){
    StmtElse *stmtelse=new stmtElse();
    token=lexer->nextToken();
    if(token.type==TokenType::EOL){
        if(lexer->nextLine()) token=lexer->nextToken();
        else throw SyntacticError(lexer->modname,token.lexeme);
        if(token.type==TokenType::INDENT)
            stmtelse->block=blockParser();
        else throw SyntacticError(lexer->modname,token.lexeme);
    }
    else throw SyntacticError(lexer->modname,token.lexeme);
    return stmtelse;
}

StmtWhile *Parser::whileParser(){
    StmtWhile *stmtwhile=new StmtWhile();
    stmtwhile->condition=exprParser();
    if(token.type==TokenType::COLON){
        token=lexer->nextToken();
        if(token.type==TokenType::EOL){
            if(lexer->nextLine()) token=lexer->nextToken();
            else throw SyntacticError(lexer->modname,token.lexeme);
            if(token.type==TokenType::INDENT)
                stmtwhile->block=blockParser();
            else throw SyntacticError(lexer->modname,token.lexeme);
        }
        else throw SyntacticError(lexer->modname,token.lexeme);
    }
    else throw SyntacticError(lexer->modname,token.lexeme);
    return stmtwhile;
}

void Parser::forParser(){
    StmtFor *stmtfor=new StmtFor();
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        stmtfor.varname=token.lexeme;
        token=token->NextToken();
        if(token.type==TokenType::IN){
            token=lexer->nextToken();
            if(token.type==TokenType::RANGE)
                stmtfor->range=rangeParser();
            else if(token.type==TokenType::ID){
                stmtfor.range=NULL;
                stmtfor.objectname=token.lexeme;
            }
            else throw SyntacticError(lexer->modname,token.lexeme);
            token=lexer->nextToken();
            if(token.type==TokenType::COLON){
                token=lexer->nextToken();
                if(token.type==TokenType::EOL){
                    if(lexer->nextLine()) token=lexer->nextToken();
                    else throw SyntacticError(lexer->modname,token.lexeme);
                    if(token.type==TokenType::INDENT)
                        stmtfor->block=blockParser();
                    else throw SyntacticError(lexer->modname,token.lexeme);
                }
                else throw SyntacticError(lexer->modname,token.lexeme);
            }
            else throw SyntacticError(lexer->modname,token.lexeme);
        }
        else throw SyntacticError(lexer->modname,token.lexeme);
    }
    else throw SyntacticError(lexer->modname,token.lexeme);
    return stmtfor;
}

StmtRange *Parser::rangeParser(){
    StmtRange *stmtrange=new StmtRange();
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        token=lexer->nextToken();
        if(token.type==TokenType::INT)
            stmtrange.begin=atoi(token.lexeme.c_str());
        else if(token.Type==TokenType::SUB){
            token=lexer->nextToken();
            if(token.Type==TokenType::INT)
                stmtrange.begin=atoi(token.lexeme.c_str())*(-1);
            else throw SyntacticError(lexer->modname,token.lexeme);
        }
        else throw SyntacticError(lexer->modname,token.lexeme);
        token=lexer->nextToken();
        if(token.type==TokenType::COLON){
            token=lexer->nextToken();
            if(token.type==TokenType::INT)
                stmtrange.end=atoi(token.lexeme.c_str());
            else if(token.type==TokenType::SUB){
                token=lexer->nextToken();
                if(token.type=TokenType::INT)
                    stmtrange.end=atoi(token.lexeme.c_str())*(-1);
                else throw SyntacticError(lexer->modname,token.lexeme);
            }
            else throw SyntacticError(lexer->modname,token.lexeme);
        }
        else throw SyntacticError(lexer->modname,token.lexeme);
        token=lexer->nextToken();
        if(token.type==TokenType::RPAR){
            stmtrange.step=1;
            return;
        }
        else if(token.type==TokenType::COLON){
            token=lexer->nextToken();
            if(token.type==TokenType::INT)
                stmtrange.step=atoi(token.lexeme.c_str());
            else if(token.type==TokenType::SUB){
                token=lexer->nextToken();
                if(token.type=TokenType::INT)
                    stmtrange.step=atoi(token.lexeme.c_str())*(-1);
                else throw SyntacticError(lexer->modname,token.lexeme);
            }
            else throw SyntacticError(lexer->modname,token.lexeme);
        }
        else throw SyntacticError(lexer->modname,token.lexeme);
        token=lexer->nextToken();
        if(token.type!=TokenType::RPAR)
            throw SyntacticError(lexer->modname,token.lexeme);
    }
    else throw SyntacticError(lexer->modname,token.lexeme);
    return stmtrange;
}

StmtPrint *Parser::printParser(){
    StmtPrint *stmtprint=new StmtPrint();
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR)
        stmtprint->exprlist=exprlistParser();
    else throw SyntacticError(lexer->modname,token.lexeme);
    if(token.type==TokenType::RPAR){
        token=lexer->nextToken();
        if(token.type!=TokenType::EOL)
            throw SyntacticError(lexer->modname,token.lexeme);
    }
    else throw SyntacticError(lexer->modname,token.lexeme);
    return stmtprint;
}

StmtExprList *Parser::exprlistParser(){
    StmtExprList *stmtexprlist=new StmtExprList();
    while(true){
        stmtexprlist->list.push_back(exprParser());
        if(token.type!=TokenType::COMMA)
            break;
    }
    return stmtexprlist;
}

StmtReturn *Parser::returnParser(){
    Stmtreturn *stmtreturn=new StmtReturn();
    stmtreturn->expt=exprParser();
    if(token.type!=TokenType::EOL)
        throw SyntacticError(lexer->modname,token.lexeme);
}

Statement *Parser::statementPParser(){
    ExprVariable *exprvariable=new ExprVariable(token.lexeme);
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        ExprMethodCall *exprmethodcall=new ExprMethodCall(exprvariable->varname);
        StmtMethodCall *stmtmethodcall=new StmtMethodCall(exprmethodcall);
        exprmethodcall->exprlist=exprlistParser();
        if(token.type==TokenType::RPAR){
            token=lexer->nextToken();
            if(token.type==TokenType::EOL) return;
            else throw SyntacticError(lexer->modname,token.lexeme);
        }
        else throw SyntacticError(lexer->modname,token.lexeme);
        return stmtmethodcall;
    }
    else if(token.type==TokenType::ASSIGN){
        StmtAssign *stmtassign=new StmtAssign();
        stmtassign.lexpr=exprvariable;
        stmtassign->repxr=assignPParser();
        return stmtassign;
    }
    else if(token.type==TokenType::LBRACK){
        Expression *index=exprParser();
        if(token.type==TokenType::RBRACK){
            ExprArray *exprarray=new ExprArray(exprvariable->varname,index);
            token=lexer->nextToken();
            if(token.type==TokenType::ASSIGN){
                Expression *rexpr=exprParser();
                StmtAssign *stmtassign=new StmtAssign(exprarray,rexpr);
                if(token.type!=TokenType::EOL) throw SyntacticError(lexer->modname,token.lexeme);
            }
            else throw SyntacticError(lexer->modname,token.lexeme);
        }
        else throw SyntacticError(lexer->modname,token.lexeme);
        return stmtassign;
    }
    else throw SyntacticError(lexer->modname,token.lexeme);
    return NULL;
}

Expression *Parser::assignParser(){
    if(token.type==TokenType::ASSIGN){
        Expression *rexpr=new Expression();
        rexpr=exprParser();
        if(token.type!=TokenType::EOL)
            throw SyntacticError(lexer->modname,token.lexeme);
    }
    else throw SyntacticError(lexer->modname,token.lexeme);
    return rexpr;
}

/****************************************************************/
/***************运算处理级别函数***************/

Expression *exprParser(){
    token=lexer->nextToken();
    if(isExpression()){
        switch(token.type){
        case TokenType::ID:
        case TokenType::LPAR:
        case TokenType::LBRACE:
            return logicParser();
        default:
            if(isConstant()) return logicParser();
            else throw SyntacticError(lexer->modname,token.lexeme);
        }
    }
}

Expression *logicParser(){
    Expression *exprlogic=logicPParser();
    while(token.type==TokenType::OR)
        exprlogic=new ExprLogic(token.lexeme,exprlogic,logicPParser());
    return exprlogic;
}

Expression *logicPParser(){
    Expression *exprlogicP=relationParser();
    while(token.type==TokenType::AND){
        exprlogicP=new ExprLogic(token.lexeme,exprlogicP,logicPParser());
    return exprlogicP;
}

Expression *relationParser(){
    Expression *exprrelation=arithParser();
    while(isCompare())
        exprlogicP=new ExprCompare(token.lexeme,exprrelation,arithParser());
    return exprrelationP;
}

Expression *arithParser(){
    Expression *exprarith=arithPParser();
    while(token.type==TokenType::ADD||token.type==TokenType::SUB)
        exprarith=new ExprArith(token.lexeme,exprarith,arithPParser());
    return exprarith;
}

Expression *arithPParser(){
    Expression *exprarithP=bitwiseParser();
    while(token.type==TokenType::MUL||token.type==TokenType::DIV)
        exprarithP=new ExprArith(token.lexeme,exprarith,arithPParser());
    return exprarithP;
}

Expression *bitwiseParser(){
    Expression *exprbitwise=termParser();
    while(token.type==TokenType::SLEFT||token.type==TokenType::SRIGHT)
        exprbitwise=new ExprBitwise(exprbitwise,termParser());
    return exprbitwise;
}

Expression *termParser(){
    token=lexer->nextToken();
    if(isExpression()){
        switch(token.type){
        case TokenType::ID:
        case TokenType::LPAR:
        case TokenType::LBRACE:
            return logicParser();
        default:
            if(isConstant()) return logicParser();
            else throw SyntacticError(lexer->modname,token.lexeme);
        }
    }
}
}
