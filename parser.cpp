#include "parser.h"

Parser::Parser(Lexer *lexer,DeclModule *program)
    :lexer(lexer),program(program){}
Parser::~Parser(){}

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
            fromParser();
            break;
        case TokenType::IMPORT:
            program->modulelist.push_back(importParser());
            break;
        case TokenType::CLASS:
            program->classlist.push_back(classParser());
            break;
        case TokenType::DEF:
            program->methodlist.push_back(methodParser());
            break;
        case TokenType::IF:
            program->entry=entryParser();
            break;
        default:
            throw SyntacticError(lexer->modname,token);
        }
    }
}

DeclModule *Parser::importParser(){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        string modname=token.lexeme;
        token=lexer->nextToken();
        if(token.type==EOL){
            DeclModule *declmodule=moduleParser(token.lexeme);
            return declmodule;
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}

void Parser::fromParser(){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        string modname=token.lexeme;
        token=lexer->nextToken();
        if(token.type==TokenType::IMPORT){
            token=lexer->nextToken();
            if(token.type==TokenType::ID){
                string name=token.lexeme;
                if(token.type==TokenType::AS){
                    token=lexer->nextToken();
                    if(token.type==TokenType::ID){
                        string alias=token.lexeme;
                        token=lexer->nextToken();
                        if(token.type==EOL){
                            DeclClass *declclass=modClassParser(modname,name);
                            if(declclass==NULL){
                                DeclMethod *declmethod=modMethodParser(modname,name);
                                if(declmethod==NULL) throw LoadingError(modname);
                                declmethod->methodname=alias;
                                program->methodlist.push_back(declmethod);
                            }
                            else{
                                declclass->classname=alias;
                                program->classlist.push_back(declclass);
                            }
                        }
                        else throw SyntacticError(lexer->modname,token);
                    }
                    else throw SyntacticError(lexer->modname,token);
                }
                else if(token.type==EOL){
                    DeclClass *declclass=modClassParser(modname,name);
                    if(declclass==NULL){
                        DeclMethod *declmethod=modMethodParser(modname,name);
                        if(declmethod==NULL) throw LoadingError(modname);
                        program->methodlist.push_back(declmethod);
                    }
                    else program->classlist.push_back(declclass);
                }
                else throw SyntacticError(lexer->modname,token);
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}

/****************************************************************/
/***************模块处理级别函数***************/

DeclModule *Parser::moduleParser(const string &modname){
    lexerlist.push(lexer);
    lexer=new Lexer(modname+".be");
    DeclModule *declmodule=new DeclModule(modname);
    bool flag=false;
    while(lexer->nextLine()&&!flag){
        token=lexer->nextToken();
        switch(token.type){
        case TokenType::FROM:
            fromParser();
            break;
        case TokenType::IMPORT:
            declmodule->modulelist.push_back(importParser());
            break;
        case TokenType::CLASS:
            declmodule->classlist.push_back(classParser());
            break;
        case TokenType::DEF:
            declmodule->methodlist.push_back(methodParser());
            break;
        case TokenType::IF:
            flag=true;
            break;
        default:
            throw SyntacticError(lexer->modname,token);
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
        stringstream scin(lexer->line);
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
    lexer=lexerlist.top();
    lexerlist.pop();
    return NULL;
}

DeclMethod *Parser::modMethodParser(const string &modname,const string &methodname){
    lexerlist.push(lexer);
    lexer=new Lexer(modname+".be");
    while(lexer->nextLine()){
        stringstream scin(lexer->line);
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
        if(token.type==TokenType::COLON){
            token=lexer->nextToken();
            if(token.type==EOL){
                if(lexer->nextLine()){
                    token=lexer->nextToken();
                    if(token.type==TokenType::INDENT){
                        token=lexer->nextToken();
                        bool hasconstructor=false;
                        DeclMethod *declmethod;
                        while(token.type!=TokenType::DEDENT){
                            switch(token.type){
                            case TokenType::INIT:
                                if(!hasconstructor){
                                    constructorParser(declclass);
                                    hasconstructor=true;
                                }
                                else
                                    throw SyntacticError(lexer->modname,token);
                                break;
                            case TokenType::ID:
                                declmethod=methodParser();
                                declmethod->self=declclass;
                                declclass->methodlist.push_back(declmethod);
                                break;
                            default:
                                throw SyntacticError(lexer->modname,token);
                            }
                        }
                        return declclass;
                    }
                    else throw SyntacticError(lexer->modname,token);
                }
                else throw SyntacticError(lexer->modname,token);
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}
/*
DeclFunction *Parser::functionParser(){
    DeclFunction *declfunction=new DeclFunction(token.lexeme);
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        declfunction->paralist=paraListParser();
        if(token.type==TokenType::RPAR){
            token=lexer->nextToken();
            if(token.type==TokenType::COLON){
                token=lexer->nextToken();
                if(token.type==EOL){
                    if(lexer->nextLine()){
                        token=lexer->nextToken();
                        if(token.type==TokenType::INDENT){
                            declfunction->functionblock=blockParser();
                            return declfunction;
                        }
                        else throw SyntacticError(lexer->modname,token);
                    }
                    else throw SyntacticError(lexer->modname,token);
                }
                else throw SyntacticError(lexer->modname,token);
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}*/

DeclEntry *Parser::entryParser(){
    DeclEntry *declentry=new DeclEntry();
    if(lexer->line=="if __name__==\"__main__\":"){
        if(lexer->nextLine()){
            token=lexer->nextToken();
            if(token.type==TokenType::INDENT){
                if(lexer->nextLine()){
                    token=lexer->nextToken();
                    if(token.type==TokenType::DEDENT){
                        while(true){
                            declentry->statements.push_back(statementParser());
                            if(!(lexer->nextLine())) break;
                        }
                    }
                    else throw SyntacticError(lexer->modname,token);
                }
                else throw SyntacticError(lexer->modname,token);
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
    return declentry;
}

/****************************************************************/
/***************函数处理级别函数***************/

void Parser::constructorParser(DeclClass *declclass){
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        token=lexer->nextToken();
        if(token.type==TokenType::SELF){
            token=lexer->nextToken();
            if(token.type==TokenType::COMMA){
                declclass->paralist=paraListParser();
                token=lexer->nextToken();
                if(token.type==TokenType::RPAR){
                    token=lexer->nextToken();
                    if(token.type==TokenType::COLON){
                        token=lexer->nextToken();
                        if(token.type==EOL){
                            if(lexer->nextLine()){
                                token=lexer->nextToken();
                                if(token.type==TokenType::INDENT)
                                    constructorBlockParser(declclass);
                                else throw SyntacticError(lexer->modname,token);
                            }
                            else throw SyntacticError(lexer->modname,token);
                        }
                        else throw SyntacticError(lexer->modname,token);
                    }
                    else throw SyntacticError(lexer->modname,token);
                }
                else throw SyntacticError(lexer->modname,token);
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}

DeclMethod *Parser::methodParser(){
    DeclMethod *declmethod=new DeclMethod(token.lexeme);
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        token=lexer->nextToken();
        if(token.type==TokenType::SELF){
            token=lexer->nextToken();
            if(token.type==TokenType::COMMA){
                declmethod->paralist=paraListParser();
                token=lexer->nextToken();
                if(token.type==TokenType::RPAR){
                    token=lexer->nextToken();
                    if(token.type==TokenType::COLON){
                        token=lexer->nextToken();
                        if(token.type==EOL){
                            if(lexer->nextLine()){
                                token=lexer->nextToken();
                                if(token.type==TokenType::INDENT){
                                    declmethod->methodblock=blockParser();
                                    return declmethod;
                                }
                                else throw SyntacticError(lexer->modname,token);
                            }
                            else throw SyntacticError(lexer->modname,token);
                        }
                        else throw SyntacticError(lexer->modname,token);
                    }
                    else throw SyntacticError(lexer->modname,token);
                }
                else throw SyntacticError(lexer->modname,token);
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}

/****************************************************************/
/***************块处理级别函数***************/

vector<string> Parser::paraListParser(){
    vector<string> paralist;
    while(true){
        token=lexer->nextToken();
        if(token.type==TokenType::ID&&token.lexeme.find('.')==token.lexeme.npos)
            paralist.push_back(token.lexeme);
        else throw SyntacticError(lexer->modname,token);
        token=lexer->nextToken();
        if(token.type!=TokenType::COMMA) break;
    }
    return paralist;
}

void Parser::constructorBlockParser(DeclClass *declclass){//???构造函数机制还是没搞懂
    //StackFrame *savedstack=curstack;
    //curstack=new StackFrame(curstack);
    while(true){
        declclass->fieldlist.push_back(fieldParser());
        if(lexer->nextLine()) token=lexer->nextToken();
        else throw SyntacticError(lexer->modname,token);
        if(token.type==TokenType::DEDENT) break;
    }
    //curstack=savedstack;
}

StmtBlock *Parser::blockParser(){
    //StackFrame *savedstack=curstackframe;
    //curstack=new StackFrame(curstackframe);
    StmtBlock *stmtblock=new StmtBlock();
    token=lexer->nextToken();
    while(token.type!=TokenType::DEDENT){
        stmtblock->statements.push_back(statementParser());
        if(lexer->nextLine()) token=lexer->nextToken();
        else throw SyntacticError(lexer->modname,token);
    }
    //curstack=savedstackframe;
    return stmtblock;
}

Statement *Parser::statementParser(){
    token=lexer->nextToken();
    switch(token.type){
    case TokenType::IF:
        return(ifParser());
    case TokenType::WHILE:
        return(whileParser());
    case TokenType::FOR:
        return(forParser());
    case TokenType::PRINT:
        return(printParser());
    case TokenType::ID:
        return(statementPParser());
    case TokenType::RETURN:
        return(returnParser());
    default:
            throw SyntacticError(lexer->modname,token);
    }
}

/****************************************************************/
/***************语句处理级别函数***************/

DeclField *Parser::fieldParser(){
    StmtAssign *stmtassign=assignParser();
    return new DeclField(stmtassign);
}

StmtAssign *Parser::assignParser(){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        ExprScalar *exprscalar=new ExprScalar(token.lexeme);
        token=lexer->nextToken();
        if(token.type==TokenType::ASSIGN){
            StmtAssign *stmtassign=new StmtAssign();
            stmtassign->lexpr=exprscalar;
            stmtassign->rexpr=exprParser();
            if(token.type!=EOL) throw SyntacticError(lexer->modname,token);
            return stmtassign;
        }
        else if(token.type==TokenType::LBRACK){
            Expr *index=exprParser();
            if(token.type==TokenType::RBRACK){
                ExprArray *exprarray=new ExprArray(exprscalar->varname,index);
                token=lexer->nextToken();
                if(token.type==TokenType::ASSIGN){
                    Expr *rexpr=exprParser();
                    StmtAssign *stmtassign=new StmtAssign();
                    stmtassign->lexpr=exprarray;
                    stmtassign->rexpr=rexpr;
                    if(token.type!=EOL) throw SyntacticError(lexer->modname,token);
                    return stmtassign;
                }
                else throw SyntacticError(lexer->modname,token);
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}

StmtIf *Parser::ifParser(){
    StmtIf *stmtif=new StmtIf();
    stmtif->condition=exprParser();
    if(token.type==TokenType::COLON){
        token=lexer->nextToken();
        if(token.type==EOL){
            if(lexer->nextLine()) token=lexer->nextToken();
            else throw SyntacticError(lexer->modname,token);
            if(token.type==TokenType::INDENT)
                stmtif->ifblock=blockParser();
            else throw SyntacticError(lexer->modname,token);
            token=lexer->nextToken();
            while(token.type==TokenType::ELIF){
                stmtif->eliflist.push_back(elifParser());
                token=lexer->nextToken();
            }
            if(token.type==TokenType::ELSE){
                token=lexer->nextToken();
                if(token.type==EOL){
                    if(lexer->nextLine()) token=lexer->nextToken();
                    else throw SyntacticError(lexer->modname,token);
                    if(token.type==TokenType::INDENT)
                        stmtif->elseblock=blockParser();
                    else throw SyntacticError(lexer->modname,token);
                }
                else throw SyntacticError(lexer->modname,token);
            }
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
    return stmtif;
}

StmtElif *Parser::elifParser(){
    Expr *condition=exprParser();
    if(token.type==TokenType::COLON){
        token=lexer->nextToken();
        if(token.type==EOL){
            if(lexer->nextLine()) token=lexer->nextToken();
            else throw SyntacticError(lexer->modname,token);
            if(token.type==TokenType::INDENT){
                StmtElif *stmtelif=new StmtElif(condition,blockParser());
                return stmtelif;
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}

StmtWhile *Parser::whileParser(){
    Expr *condition=exprParser();
    if(token.type==TokenType::COLON){
        token=lexer->nextToken();
        if(token.type==EOL){
            if(lexer->nextLine()) token=lexer->nextToken();
            else throw SyntacticError(lexer->modname,token);
            if(token.type==TokenType::INDENT){
                StmtWhile *stmtwhile=new StmtWhile(condition,blockParser());
                return stmtwhile;
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}

StmtFor *Parser::forParser(){
    StmtFor *stmtfor=new StmtFor();
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        stmtfor->targetname=token.lexeme;
        token=lexer->nextToken();
        if(token.type==TokenType::IN){
            token=lexer->nextToken();
            if(token.type==TokenType::RANGE)
                stmtfor->range=rangeParser();
            else if(token.type==TokenType::ID){
                stmtfor->range=NULL;
                stmtfor->objectname=token.lexeme;
            }
            else throw SyntacticError(lexer->modname,token);
            token=lexer->nextToken();
            if(token.type==TokenType::COLON){
                token=lexer->nextToken();
                if(token.type==EOL){
                    if(lexer->nextLine()) token=lexer->nextToken();
                    else throw SyntacticError(lexer->modname,token);
                    if(token.type==TokenType::INDENT)
                        stmtfor->forblock=blockParser();
                    else throw SyntacticError(lexer->modname,token);
                }
                else throw SyntacticError(lexer->modname,token);
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
    return stmtfor;
}

StmtRange *Parser::rangeParser(){
    StmtRange *stmtrange=new StmtRange();
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        token=lexer->nextToken();
        if(token.type==TokenType::INT)
            stmtrange->begin=atoi(token.lexeme.c_str());
        else if(token.type==TokenType::SUB){
            token=lexer->nextToken();
            if(token.type==TokenType::INT)
                stmtrange->begin=atoi(token.lexeme.c_str())*(-1);
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
        token=lexer->nextToken();
        if(token.type==TokenType::COLON){
            token=lexer->nextToken();
            if(token.type==TokenType::INT)
                stmtrange->end=atoi(token.lexeme.c_str());
            else if(token.type==TokenType::SUB){
                token=lexer->nextToken();
                if(token.type==TokenType::INT)
                    stmtrange->end=atoi(token.lexeme.c_str())*(-1);
                else throw SyntacticError(lexer->modname,token);
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
        token=lexer->nextToken();
        if(token.type==TokenType::RPAR){
            stmtrange->step=1;
        }
        else if(token.type==TokenType::COLON){
            token=lexer->nextToken();
            if(token.type==TokenType::INT)
                stmtrange->step=atoi(token.lexeme.c_str());
            else if(token.type==TokenType::SUB){
                token=lexer->nextToken();
                if(token.type==TokenType::INT)
                    stmtrange->step=atoi(token.lexeme.c_str())*(-1);
                else throw SyntacticError(lexer->modname,token);
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
        token=lexer->nextToken();
        if(token.type!=TokenType::RPAR)
            throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
    return stmtrange;
}

StmtPrint *Parser::printParser(){
    StmtPrint *stmtprint=new StmtPrint();
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        while(true){
            stmtprint->printlist.push_back(exprParser());
            if(token.type!=TokenType::COMMA) break;
        }
    }
    else throw SyntacticError(lexer->modname,token);
    if(token.type==TokenType::RPAR){
        token=lexer->nextToken();
        if(token.type!=EOL)
            throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
    return stmtprint;
}

StmtReturn *Parser::returnParser(){
    Expr *exprreturn=exprParser();
    if(token.type!=EOL)
        throw SyntacticError(lexer->modname,token);
    return new StmtReturn(exprreturn);
}

Statement *Parser::statementPParser(){
    ExprScalar *exprscalar=new ExprScalar(token.lexeme);
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        ExprMethodCall *exprmethodcall=new ExprMethodCall(exprscalar->varname);
        StmtMethodCall *stmtmethodcall=new StmtMethodCall(exprmethodcall);
        while(true){
            exprmethodcall->arglist.push_back(exprParser());
            if(token.type!=TokenType::COMMA) break;
        }
        if(token.type==TokenType::RPAR){
            token=lexer->nextToken();
            if(token.type!=EOL) throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
        return stmtmethodcall;
    }
    else if(token.type==TokenType::ASSIGN){
        StmtAssign *stmtassign=new StmtAssign();
        stmtassign->lexpr=exprscalar;
        stmtassign->rexpr=exprParser();
        return stmtassign;
    }
    else if(token.type==TokenType::LBRACK){
        Expr *index=exprParser();
        if(token.type==TokenType::RBRACK){
            ExprArray *exprarray=new ExprArray(exprscalar->varname,index);
            token=lexer->nextToken();
            if(token.type==TokenType::ASSIGN){
                Expr *rexpr=exprParser();
                StmtAssign *stmtassign=new StmtAssign(exprarray,rexpr);
                if(token.type!=EOL) throw SyntacticError(lexer->modname,token);
                return stmtassign;
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}

/****************************************************************/
/***************运算处理级别函数***************/

//Expr *Parser::assignPParser(){
//    Expr *rexpr=new Expr();
//    rexpr=exprParser();
//    if(token.type!=EOL)
//        throw SyntacticError(lexer->modname,token);
//    return rexpr;
//}

Expr *Parser::exprParser(){
    token=lexer->nextToken();
    if(token.isExpr()){
        switch(token.type){
        case TokenType::ID:
        case TokenType::LPAR:
        case TokenType::LBRACE:
            return logicOrParser();
        default:
            if(token.isConstant()) return logicOrParser();
            else throw SyntacticError(lexer->modname,token);
        }
    }
    else throw SyntacticError(lexer->modname,token);
}

Expr *Parser::logicOrParser(){
    Expr *exprlogicor=logicAndParser();
    while(token.type==TokenType::OR)
        exprlogicor=new ExprLogic(token.lexeme,exprlogicor,logicAndParser());
    return exprlogicor;
}

Expr *Parser::logicAndParser(){
    Expr *exprlogicand=relationParser();
    while(token.type==TokenType::AND)
        exprlogicand=new ExprLogic(token.lexeme,exprlogicand,relationParser());
    return exprlogicand;
}

Expr *Parser::relationParser(){
    Expr *exprrelation=summaryParser();
    while(token.isCompare())
        exprrelation=new ExprCompare(token.lexeme,exprrelation,summaryParser());
    return exprrelation;
}

Expr *Parser::summaryParser(){
    Expr *exprsummary=productParser();
    while(token.type==TokenType::ADD||token.type==TokenType::SUB)
        exprsummary=new ExprArith(token.lexeme,exprsummary,productParser());
    return exprsummary;
}

Expr *Parser::productParser(){
    Expr *exprproduct=bitwiseParser();
    //Expr *exprproduct=productParser();
    while(token.type==TokenType::MUL||token.type==TokenType::DIV)
        exprproduct=new ExprArith(token.lexeme,exprproduct,bitwiseParser());
    return exprproduct;
}

Expr *Parser::bitwiseParser(){
    Expr *exprbitwise=termParser();
    while(token.type==TokenType::SLEFT||token.type==TokenType::SRIGHT)
        exprbitwise=new ExprBitwise(token.lexeme,exprbitwise,termParser());
    return exprbitwise;
}

Expr *Parser::termParser(){
    if(token.type==TokenType::ID) return exprPParser();
    else if(token.type==TokenType::LPAR){
        Expr *term=exprParser();
        if(token.type==TokenType::RPAR) token=lexer->nextToken();
        else throw SyntacticError(lexer->modname,token);
        return term;
    }
    else if(token.type==TokenType::LBRACK){
        ExprArrayInit *exprarrayinit=new ExprArrayInit();
        while(true){
            exprarrayinit->initlist.push_back(exprParser());
            if(token.type!=TokenType::COMMA) break;
        }
        return exprarrayinit;
    }
    else if(token.isConstant()) return constantParser();
    else throw SyntacticError(lexer->modname,token);
}

Expr *Parser::exprPParser(){
    string name=token.lexeme;
    token=lexer->nextToken();
    if(token.type==TokenType::LBRACK){
        token=lexer->nextToken();
        ExprArray *exprarray=new ExprArray(name,exprParser());
        if(token.type==TokenType::RBRACK) token=lexer->nextToken();
        else throw SyntacticError(lexer->modname,token);
        return exprarray;
    }
    else if(token.type==TokenType::LPAR){
        ExprMethodCall *exprmethodcall=new ExprMethodCall(name);
        while(true){
            exprmethodcall->arglist.push_back(exprParser());
            if(token.type!=TokenType::COMMA) break;
        }
        if(token.type==TokenType::RPAR) token=lexer->nextToken();
        else throw SyntacticError(lexer->modname,token);
        return exprmethodcall;
    }
    else return new ExprScalar(name);
}

Expr *Parser::constantParser(){
    if(token.isNumeric()){
        ExprNum *exprnum=new ExprNum(atoi(token.lexeme.c_str()));
        token=lexer->nextToken();
        return exprnum;
    }
    if(token.isBoolean()){
        bool val;
        if(token.type==TokenType::TRUE) val=true;
        else val=false;
        ExprBoolean *exprboolean=new ExprBoolean(val);
        return exprboolean;
    }
    if(token.type==TokenType::STRING){
        ExprString *exprstring=new ExprString(token.lexeme);
        token=lexer->nextToken();
        return exprstring;
    }
}

