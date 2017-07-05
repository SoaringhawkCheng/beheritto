#include "parser.h"

//extern DeclModule *program;
extern DeclMethod *curmethod;
//extern string curmodname;
//extern int curline;

Parser::Parser(Lexer *lexer)
    :lexer(lexer){}
Parser::~Parser(){}

/****************************************************************/
/**************************语法分析器接口函数**************************/

void Parser::process(){
    programParser();
}

/****************************************************************/
/**************************程序处理级别函数**************************/

void Parser::programParser(){
    while(lexer->nextLine()){
        token=lexer->nextToken();
        switch(token.type){
            case TokenType::IMPORT:{
                DeclModule *declmodule=importParser();
                program->modulelist[declmodule->modname]=declmodule;
            }
            break;
            case TokenType::CLASS:{
                DeclClass *declclass=classParser();
                program->classlist[declclass->classname]=declclass;
            }
            break;
            case TokenType::DEF:{
                DeclMethod *declmethod=methodParser();
                program->methodlist[declmethod->methodname]=declmethod;
            }
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

//void Parser::fromParser(){
//    token=lexer->nextToken();
//    if(token.type==TokenType::ID){
//        string modname=token.lexeme;
//        token=lexer->nextToken();
//        if(token.type==TokenType::IMPORT){
//            token=lexer->nextToken();
//            if(token.type==TokenType::ID){
//                string name=token.lexeme;
//                if(token.type==TokenType::AS){
//                    token=lexer->nextToken();
//                    if(token.type==TokenType::ID){
//                        string alias=token.lexeme;
//                        token=lexer->nextToken();
//                        if(token.type==EOL){
//                            DeclClass *declclass=modClassParser(modname,name);
//                            if(declclass==NULL){
//                                DeclMethod *declmethod=modMethodParser(modname,name);
//                                if(declmethod==NULL) throw LoadingError(modname);
//                                declmethod->methodname=alias;
//                                program->methodlist.push_back(declmethod);
//                            }
//                            else{
//                                declclass->classname=alias;
//                                program->classlist.push_back(declclass);
//                            }
//                        }
//                        else throw SyntacticError(lexer->modname,token);
//                    }
//                    else throw SyntacticError(lexer->modname,token);
//                }
//                else if(token.type==EOL){
//                    DeclClass *declclass=modClassParser(modname,name);
//                    if(declclass==NULL){
//                        DeclMethod *declmethod=modMethodParser(modname,name);
//                        if(declmethod==NULL) throw LoadingError(modname);
//                        program->methodlist.push_back(declmethod);
//                    }
//                    else program->classlist.push_back(declclass);
//                }
//                else throw SyntacticError(lexer->modname,token);
//            }
//            else throw SyntacticError(lexer->modname,token);
//        }
//        else throw SyntacticError(lexer->modname,token);
//    }
//    else throw SyntacticError(lexer->modname,token);
//}

/****************************************************************/
/*************************模块处理级别函数*************************/

DeclModule *Parser::moduleParser(const string &modname){
    lexerlist.push(lexer);
    lexer=new Lexer(modname+".be");
    DeclModule *declmodule=new DeclModule(modname);
    declmodule->line=token.row;
    declmodule->enclosingmodule=lexer->modname;
    while(lexer->nextLine()){
        token=lexer->nextToken();
        switch(token.type){
            case TokenType::IMPORT:{
                DeclModule *declmodule=importParser();
                declmodule->modulelist[declmodule->modname]=declmodule;
            }
                break;
            case TokenType::CLASS:{
                DeclClass *declclass=classParser();
                declmodule->classlist[declclass->classname]=declclass;
            }
                break;
            case TokenType::DEF:{
                DeclMethod *declmethod=methodParser();
                declmodule->methodlist[declmethod->methodname]=declmethod;
            }
                break;
            case TokenType::IF:
                break;
            default:
                throw SyntacticError(lexer->modname,token);
        }
    }
    lexer=lexerlist.top();
    lexerlist.pop();
    return declmodule;
}

//DeclClass *Parser::modClassParser(const string &modname,const string &classname){
//    lexerlist.push(lexer);
//    lexer=new Lexer(modname+".be");
//    while(lexer->nextLine()){
//        stringstream scin(lexer->line);
//        string str1,str2;
//        scin>>str1>>str2;
//        if(str1=="class"&&str2==classname){
//            token=lexer->nextToken();
//            DeclClass *declclass=classParser();
//            lexer=lexerlist.top();
//            lexerlist.pop();
//            return declclass;
//        }
//    }
//    lexer=lexerlist.top();
//    lexerlist.pop();
//    return NULL;
//}
//
//DeclMethod *Parser::modMethodParser(const string &modname,const string &methodname){
//    lexerlist.push(lexer);
//    lexer=new Lexer(modname+".be");
//    while(lexer->nextLine()){
//        stringstream scin(lexer->line);
//        string str1,str2;
//        scin>>str1>>str2;
//        if(str1=="def"&&str2==methodname){
//            token=lexer->nextToken();
//            DeclMethod *declmethod=methodParser();
//            lexer=lexerlist.top();
//            lexerlist.pop();
//            return declmethod;
//        }
//    }
//    lexer=lexerlist.top();
//    lexerlist.pop();
//    return NULL;
//}

/****************************************************************/
/*************************类型处理级别函数*************************/

DeclClass *Parser::classParser(){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        string classname=token.lexeme;
        DeclClass *declclass=new DeclClass(classname);
        declclass->enclosingmodule=lexer->modname;
        declclass->line=token.row;
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
                                declclass->methodlist[declmethod->methodname]=declmethod;
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
    declentry->enclosingmodule=lexer->modname;
    declentry->line=token.row;
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
/*************************函数处理级别函数*************************/

void Parser::constructorParser(DeclClass *declclass){
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
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

DeclMethod *Parser::methodParser(){
    DeclMethod *declmethod=new DeclMethod(token.lexeme);
    declmethod->enclosingmodule=lexer->modname;
    declmethod->line=token.row;
    curmethod=declmethod;
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
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
                            curmethod=declmethod;
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

/****************************************************************/
/*************************块处理级别函数*************************/

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

void Parser::constructorBlockParser(DeclClass *declclass){
    while(true){
        declclass->fieldlist.push_back(fieldParser());
        if(lexer->nextLine()) token=lexer->nextToken();
        else throw SyntacticError(lexer->modname,token);
        if(token.type==TokenType::DEDENT) break;
    }
}

StmtBlock *Parser::blockParser(){
    StmtBlock *stmtblock=new StmtBlock();
    stmtblock->enclosingmodule=lexer->modname;
    stmtblock->line=token.row;
    token=lexer->nextToken();
    while(token.type!=TokenType::DEDENT){
        stmtblock->statements.push_back(statementParser());
        if(lexer->nextLine()) token=lexer->nextToken();
        else throw SyntacticError(lexer->modname,token);
    }
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
/*************************语句处理级别函数*************************/

DeclField *Parser::fieldParser(){
    StmtAssign *stmtassign=assignParser();
    DeclField *declfield=new DeclField(stmtassign);
    declfield->enclosingmodule=lexer->modname;
    declfield->line=token.row;
    return declfield;
}

StmtAssign *Parser::assignParser(){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        ExprID *exprid=new ExprID(token.lexeme);
        exprid->enclosingmodule=lexer->modname;
        exprid->line=token.row;
        token=lexer->nextToken();
        if(token.type==TokenType::ASSIGN){
            StmtAssign *stmtassign=new StmtAssign();
            stmtassign->lexpr=exprid;
            stmtassign->rexpr=exprParser();
            stmtassign->enclosingmodule=lexer->modname;
            stmtassign->line=token.row;
            if(token.type!=EOL) throw SyntacticError(lexer->modname,token);
            return stmtassign;
        }
        else if(token.type==TokenType::LBRACK){
            Expr *index=exprParser();
            if(token.type==TokenType::RBRACK){
                ExprArray *exprarray=new ExprArray(exprid->varname,index);
                exprarray->enclosingmodule=lexer->modname;
                exprarray->line=token.type;
                token=lexer->nextToken();
                if(token.type==TokenType::ASSIGN){
                    Expr *rexpr=exprParser();
                    StmtAssign *stmtassign=new StmtAssign();
                    stmtassign->lexpr=exprarray;
                    stmtassign->rexpr=rexpr;
                    stmtassign->enclosingmodule=lexer->modname;
                    stmtassign->line=token.row;
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
    stmtif->enclosingmodule=lexer->modname;
    stmtif->line=token.row;
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
                stmtelif->enclosingmodule=lexer->modname;
                stmtelif->line=token.row-1;
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
                stmtwhile->enclosingmodule=lexer->modname;
                stmtwhile->line=token.row-1;
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
    stmtfor->enclosingmodule=lexer->modname;
    stmtfor->line=token.row;
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
    stmtrange->enclosingmodule=lexer->modname;
    stmtrange->line=token.row;
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        token=lexer->nextToken();
        if(token.type==TokenType::INTEGER)
            stmtrange->begin=atoi(token.lexeme.c_str());
        else if(token.type==TokenType::SUB){
            token=lexer->nextToken();
            if(token.type==TokenType::INTEGER)
                stmtrange->begin=atoi(token.lexeme.c_str())*(-1);
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
        token=lexer->nextToken();
        if(token.type==TokenType::COLON){
            token=lexer->nextToken();
            if(token.type==TokenType::INTEGER)
                stmtrange->end=atoi(token.lexeme.c_str());
            else if(token.type==TokenType::SUB){
                token=lexer->nextToken();
                if(token.type==TokenType::INTEGER)
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
            if(token.type==TokenType::INTEGER)
                stmtrange->step=atoi(token.lexeme.c_str());
            else if(token.type==TokenType::SUB){
                token=lexer->nextToken();
                if(token.type==TokenType::INTEGER)
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
    stmtprint->enclosingmodule=lexer->modname;
    stmtprint->line=token.row;
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
    Expr *exprret=exprParser();
    if(token.type!=EOL)
        throw SyntacticError(lexer->modname,token);
    StmtReturn *stmtreturn=new StmtReturn(exprret);
    stmtreturn->enclosingmodule=lexer->modname;
    stmtreturn->line=token.row;
    return stmtreturn;
}

Statement *Parser::statementPParser(){
    ExprID *exprid=new ExprID(token.lexeme);
    exprid->enclosingmodule=lexer->modname;
    exprid->line=token.row;
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        ExprMethodCall *exprmethodcall=new ExprMethodCall(exprid->varname);
        exprmethodcall->enclosingmodule=lexer->modname;
        exprmethodcall->line=token.row;
        StmtMethodCall *stmtmethodcall=new StmtMethodCall(exprmethodcall);
        stmtmethodcall->enclosingmodule=lexer->modname;
        stmtmethodcall->line=token.row;
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
        if(exprid->varname.find(".")==exprid->varname.npos){
            StmtAssign *stmtassign=new StmtAssign();
            stmtassign->lexpr=exprid;
            stmtassign->rexpr=exprParser();
            stmtassign->enclosingmodule=lexer->modname;
            stmtassign->line=token.row;
            return stmtassign;
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else if(token.type==TokenType::LBRACK){
        if(exprid->varname.find(".")==exprid->varname.npos){
            Expr *index=exprParser();
            index->enclosingmodule=lexer->modname;
            index->line=token.row;
            if(token.type==TokenType::RBRACK){
                ExprArray *exprarray=new ExprArray(exprid->varname,index);
                exprarray->enclosingmodule=lexer->modname;
                exprarray->line=token.row;
                token=lexer->nextToken();
                if(token.type==TokenType::ASSIGN){
                    Expr *rexpr=exprParser();
                    rexpr->enclosingmodule=lexer->modname;
                    rexpr->line=token.row;
                    StmtAssign *stmtassign=new StmtAssign(exprarray,rexpr);
                    stmtassign->enclosingmodule=lexer->modname;
                    stmtassign->line=token.row;
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

/****************************************************************/
/*************************运算处理级别函数*************************/

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
    while(token.type==TokenType::OR){
        exprlogicor=new ExprLogic(token.lexeme,exprlogicor,logicAndParser());
        exprlogicor->enclosingmodule=lexer->modname;
        exprlogicor->line=token.row;
    }
    return exprlogicor;
}

Expr *Parser::logicAndParser(){
    Expr *exprlogicand=relationParser();
    while(token.type==TokenType::AND){
        exprlogicand=new ExprLogic(token.lexeme,exprlogicand,relationParser());
        exprlogicand->enclosingmodule=lexer->modname;
        exprlogicand->line=token.row;
    }
    return exprlogicand;
}

Expr *Parser::relationParser(){
    Expr *exprrelation=summaryParser();
    while(token.isCompare()){
        exprrelation=new ExprCompare(token.lexeme,exprrelation,summaryParser());
        exprrelation->enclosingmodule=lexer->modname;
        exprrelation->line=token.row;
    }
    return exprrelation;
}

Expr *Parser::summaryParser(){
    Expr *exprsummary=productParser();
    while(token.type==TokenType::ADD||token.type==TokenType::SUB){
        exprsummary=new ExprArith(token.lexeme,exprsummary,productParser());
        exprsummary->enclosingmodule=lexer->modname;
        exprsummary->line=token.row;
    }
    return exprsummary;
}

Expr *Parser::productParser(){
    Expr *exprproduct=bitwiseParser();
    //Expr *exprproduct=productParser();
    while(token.type==TokenType::MUL||token.type==TokenType::DIV){
        exprproduct=new ExprArith(token.lexeme,exprproduct,bitwiseParser());
        exprproduct->enclosingmodule=lexer->modname;
        exprproduct->line=token.row;
    }
    return exprproduct;
}

Expr *Parser::bitwiseParser(){
    Expr *exprbitwise=termParser();
    while(token.type==TokenType::SLEFT||token.type==TokenType::SRIGHT){
        exprbitwise=new ExprBitwise(token.lexeme,exprbitwise,termParser());
        exprbitwise->enclosingmodule=lexer->modname;
        exprbitwise->line=token.row;
    }
    return exprbitwise;
}

Expr *Parser::termParser(){
    if(token.type==TokenType::ID) return exprPParser();
    else if(token.type==TokenType::LPAR){
        Expr *term=exprParser();
        term->enclosingmodule=lexer->modname;
        term->line=token.row;
        if(token.type==TokenType::RPAR) token=lexer->nextToken();
        else throw SyntacticError(lexer->modname,token);
        return term;
    }
    else if(token.type==TokenType::LBRACK){
        ExprArrayInit *exprarrayinit=new ExprArrayInit();
        exprarrayinit->enclosingmodule=lexer->modname;
        exprarrayinit->line=token.row;
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
        exprarray->enclosingmodule=lexer->modname;
        exprarray->line=token.row;
        if(token.type==TokenType::RBRACK) token=lexer->nextToken();
        else throw SyntacticError(lexer->modname,token);
        return exprarray;
    }
    else if(token.type==TokenType::LPAR){
        ExprMethodCall *exprmethodcall=new ExprMethodCall(name);
        exprmethodcall->enclosingmodule=lexer->modname;
        exprmethodcall->line=token.row;
        while(true){
            exprmethodcall->arglist.push_back(exprParser());
            if(token.type!=TokenType::COMMA) break;
        }
        if(token.type==TokenType::RPAR) token=lexer->nextToken();
        else throw SyntacticError(lexer->modname,token);
        return exprmethodcall;
    }
    else{
        ExprID *ret=new ExprID(name);
        ret->enclosingmodule=lexer->modname;
        ret->line=token.row;
        return ret;
    }
}

Expr *Parser::constantParser(){
    if(token.type==TokenType::INTEGER){
        ExprInteger *exprinteger=new ExprInteger(atoi(token.lexeme.c_str()));
        exprinteger->enclosingmodule=lexer->modname;
        exprinteger->line=token.row;
        token=lexer->nextToken();
        return exprinteger;
    }
    if(token.type==TokenType::FLOAT){
        ExprFloat *exprfloat=new ExprFloat(atof(token.lexeme.c_str()));
        exprfloat->enclosingmodule=lexer->modname;
        exprfloat->line=token.row;
        token=lexer->nextToken();
        return exprfloat;
    }
    if(token.isBoolean()){
        bool val;
        if(token.type==TokenType::TRUE) val=true;
        else val=false;
        ExprBoolean *exprboolean=new ExprBoolean(val);
        exprboolean->enclosingmodule=lexer->modname;
        exprboolean->line=token.row;
        return exprboolean;
    }
    if(token.type==TokenType::STRING){
        ExprString *exprstring=new ExprString(token.lexeme);
        exprstring->enclosingmodule=lexer->modname;
        exprstring->line=token.row;
        token=lexer->nextToken();
        return exprstring;
    }
    return NULL;
}

