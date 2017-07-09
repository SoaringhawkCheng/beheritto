#include "parser.h"

//extern DeclModule *program;
extern DeclModule *curmodule;
extern DeclClass *curdeclclass;
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
    curmodule=program;
    if(lexer->nextLine()){
        while(true){
            token=lexer->nextToken();
            switch(token.type){
                case TokenType::IMPORT:{
                    DeclModule *declmodule=importParser();
                    program->modulelist[declmodule->modname]=declmodule;
                    break;
                }
                case TokenType::CLASS:{
                    DeclClass *declclass=classParser();
                    program->classlist[declclass->classname]=declclass;
                    break;
                }
                case TokenType::DEF:{
                    token=lexer->nextToken();
                    DeclMethod *declmethod=methodParser();
                    program->methodlist[declmethod->methodname]=declmethod;
                    break;
                }
                case TokenType::IF:
                    program->entry=entryParser();
                    return;
                default:
                    throw SyntaxError(lexer->modname,token);
            }
        }
    }
}

DeclModule *Parser::importParser(){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        string modname=token.lexeme;
        token=lexer->nextToken();
        if(token.type==EOL){
            if(lexer->nextLine()){
                DeclModule *declmodule=moduleParser(lexer->pathname,modname);
                return declmodule;
            }
            else throw SyntaxError(lexer->modname,token);
        }
        else throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
}

/****************************************************************/
/*************************模块处理级别函数*************************/

DeclModule *Parser::moduleParser(const string &pathname,const string &modname){
    lexerlist.push(lexer);
    lexer=new Lexer(pathname+modname+".be");
    DeclModule *declmodule=new DeclModule(modname);
    DeclModule *save=curmodule;
    curmodule=declmodule;
    declmodule->modname=lexer->modname;
    declmodule->line=token.row;
    if(lexer->nextLine()){
        token=lexer->nextToken();
        while(token.type!=TokenType::IF){
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
                    token=lexer->nextToken();
                    DeclMethod *declmethod=methodParser();
                    declmodule->methodlist[declmethod->methodname]=declmethod;
                }
                    break;
                default:
                    throw SyntaxError(lexer->modname,token);
            }
            token=lexer->nextToken();
        }
    }
    curmodule=save;
    lexer=lexerlist.top();
    lexerlist.pop();
    return declmodule;
}

/****************************************************************/
/*************************类型处理级别函数*************************/

DeclClass *Parser::classParser(){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        string classname=token.lexeme;
        DeclClass *declclass=new DeclClass(classname);
        curclass=declclass;
        declclass->modname=lexer->modname;
        declclass->line=token.row;
        token=lexer->nextToken();
        if(token.type==TokenType::COLON){
            token=lexer->nextToken();
            if(token.type==EOL){
                if(lexer->nextLine()){
                    token=lexer->nextToken();
                    if(token.type==TokenType::INDENT){
                        bool hasconstructor=false;
                        token=lexer->nextToken();
                        while(token.type!=TokenType::DEDENT){
                            if(token.type==TokenType::DEF){
                                DeclMethod *declmethod;
                                token=lexer->nextToken();
                                switch(token.type){
                                    case TokenType::INIT:{
                                        if(!hasconstructor){
                                            constructorParser(declclass);
                                            hasconstructor=true;
                                        }
                                        else
                                            throw SyntaxError(lexer->modname,token);
                                        break;
                                    }
                                    case TokenType::ID:{
                                        declmethod=methodParser();
                                        declclass->methodlist[declmethod->methodname]=declmethod;
                                        break;
                                    }
                                default:
                                    throw SyntaxError(lexer->modname,token);
                                }
                                token=lexer->nextToken();
                            }
                            else throw SyntaxError(lexer->modname,token);
                        }
                        curclass=NULL;
                        return declclass;
                    }
                    else throw SyntaxError(lexer->modname,token);
                }
                else throw SyntaxError(lexer->modname,token);
            }
            else throw SyntaxError(lexer->modname,token);
        }
        else throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
}

DeclEntry *Parser::entryParser(){
    DeclEntry *declentry=new DeclEntry();
    declentry->modname=lexer->modname;
    declentry->line=token.row;
    if(lexer->line=="if __name__==\"__main__\":"){
        lexer->printline="if __name__==\"__main__\":";
        if(lexer->nextLine()){
            token=lexer->nextToken();
            if(token.type==TokenType::INDENT){
                token=lexer->nextToken();
                while(token.type!=TokenType::DEDENT){
                    bool stmttype=false;
                    switch(token.type){
                        case TokenType::IF:
                        case TokenType::WHILE:
                        case TokenType::FOR:
                        case TokenType::DEF:
                            stmttype=true;
                            break;
                        case TokenType::DONE:
                            return declentry;
                        default:
                            break;
                    }
                    declentry->statements.push_back(statementParser());
                    if(stmttype) token=lexer->nextToken();
                    else{
                        if(lexer->nextLine()) token=lexer->nextToken();
                        else throw SyntaxError(lexer->modname,token);
                    }
                }
            }
            else throw SyntaxError(lexer->modname,token);
        }
        else throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
    return declentry;
}

/****************************************************************/
/*************************函数处理级别函数*************************/

void Parser::constructorParser(DeclClass *declclass){
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        declclass->paralist=paraListParser();
        if(token.type==TokenType::RPAR){
            token=lexer->nextToken();
            if(token.type==TokenType::COLON){
                token=lexer->nextToken();
                if(token.type==EOL){
                    if(lexer->nextLine()){
                        token=lexer->nextToken();
                        if(token.type==TokenType::INDENT)
                            constructorBlockParser(declclass);
                        else throw SyntaxError(lexer->modname,token);
                    }
                    else throw SyntaxError(lexer->modname,token);
                }
                else throw SyntaxError(lexer->modname,token);
            }
            else throw SyntaxError(lexer->modname,token);
        }
        else throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
}

DeclMethod *Parser::methodParser(){
    if(token.type==TokenType::ID){
        DeclMethod *declmethod=new DeclMethod(token.lexeme);
//        declmethod->enclosingclass=curclass;
        declmethod->modname=lexer->modname;
        declmethod->line=token.row;
        curmethod=declmethod;
        token=lexer->nextToken();
        if(token.type==TokenType::LPAR){
            declmethod->paralist=paraListParser();
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
                            else throw SyntaxError(lexer->modname,token);
                        }
                        else throw SyntaxError(lexer->modname,token);
                    }
                    else throw SyntaxError(lexer->modname,token);
                }
                else throw SyntaxError(lexer->modname,token);
            }
            else throw SyntaxError(lexer->modname,token);
        }
        else throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
}

/****************************************************************/
/*************************块处理级别函数*************************/

vector<string> Parser::paraListParser(){
    vector<string> paralist;
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        while(true){
            if(token.type==TokenType::ID){
                if(token.lexeme.find('.')==token.lexeme.npos)
                    paralist.push_back(token.lexeme);
                else throw SyntaxError(lexer->modname,token);
                token=lexer->nextToken();
                if(token.type!=TokenType::COMMA) break;
            }
            else throw SyntaxError(lexer->modname,token);
            token=lexer->nextToken();
        }
    }
    return paralist;
}

void Parser::constructorBlockParser(DeclClass *declclass){
    token=lexer->nextToken();
    while(true){
        declclass->fieldlist.push_back(fieldParser());
        if(lexer->nextLine()) token=lexer->nextToken();
        else throw SyntaxError(lexer->modname,token);
        if(token.type==TokenType::DEDENT) break;
    }
}

StmtBlock *Parser::blockParser(){
    StmtBlock *stmtblock=new StmtBlock();
    stmtblock->modname=lexer->modname;
    stmtblock->line=token.row;
    token=lexer->nextToken();
    while(token.type!=TokenType::DEDENT){
        int stmttype=0;
        switch(token.type){
            case TokenType::IF:
                stmttype=1;
                break;
            case TokenType::WHILE:
            case TokenType::FOR:
            case TokenType::DEF:
                stmttype=2;
                break;
            default:
                break;
        }
        stmtblock->statements.push_back(statementParser());
        if(stmttype==2) token=lexer->nextToken();
        else if(stmttype==0){
            if(lexer->nextLine()) token=lexer->nextToken();
            else throw SyntaxError(lexer->modname,token);
        }
    }
    return stmtblock;
}

Statement *Parser::statementParser(){
    switch(token.type){
        case TokenType::IF:
            return(ifParser());
        case TokenType::WHILE:
            return(whileParser());
        case TokenType::FOR:
            return(forParser());
        case TokenType::PRINT:
            return(printParser());
        case TokenType::PRINTLN:
            return(printlnParser());
        case TokenType::ID:
            return(statementPParser());
        case TokenType::BREAK:
            return(breakParser());
        case TokenType::CONTINUE:
            return(continueParser());
        case TokenType::RETURN:
            return(returnParser());
        default:
            throw SyntaxError(lexer->modname,token);
    }
}

/****************************************************************/
/*************************语句处理级别函数*************************/

DeclField *Parser::fieldParser(){
    StmtAssign *stmtassign=assignParser();
    DeclField *declfield=new DeclField(stmtassign);
    declfield->modname=lexer->modname;
    declfield->line=token.row;
    return declfield;
}

StmtAssign *Parser::assignParser(){
    if(token.type==TokenType::ID){
        ExprID *exprid=new ExprID(token.lexeme);
        exprid->modname=lexer->modname;
        exprid->line=token.row;
        token=lexer->nextToken();
        if(token.type==TokenType::ASSIGN){
            StmtAssign *stmtassign=new StmtAssign();
            stmtassign->lexpr=exprid;
            stmtassign->rexpr=exprParser();
            stmtassign->modname=lexer->modname;
            stmtassign->line=token.row;
            if(token.type!=EOL) throw SyntaxError(lexer->modname,token);
            return stmtassign;
        }
        else if(token.type==TokenType::LBRACK){
            Expr *index=exprParser();
            if(token.type==TokenType::RBRACK){
                ExprArray *exprarray=new ExprArray(exprid->varname,index);
                exprarray->modname=lexer->modname;
                exprarray->line=token.type;
                token=lexer->nextToken();
                if(token.type==TokenType::ASSIGN){
                    Expr *rexpr=exprParser();
                    StmtAssign *stmtassign=new StmtAssign();
                    stmtassign->lexpr=exprarray;
                    stmtassign->rexpr=rexpr;
                    stmtassign->modname=lexer->modname;
                    stmtassign->line=token.row;
                    if(token.type!=EOL) throw SyntaxError(lexer->modname,token);
                    return stmtassign;
                }
                else throw SyntaxError(lexer->modname,token);
            }
            else throw SyntaxError(lexer->modname,token);
        }
        else throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
}

StmtIf *Parser::ifParser(){
    StmtIf *stmtif=new StmtIf();
    stmtif->condition=exprParser();
    stmtif->modname=lexer->modname;
    stmtif->line=token.row;
    if(token.type==TokenType::COLON){
        token=lexer->nextToken();
        if(token.type==EOL){
            if(lexer->nextLine()) token=lexer->nextToken();
            else throw SyntaxError(lexer->modname,token);
            if(token.type==TokenType::INDENT)
                stmtif->ifblock=blockParser();
            else throw SyntaxError(lexer->modname,token);
            token=lexer->nextToken();
            while(token.type==TokenType::ELIF){
                stmtif->eliflist.push_back(elifParser());
                token=lexer->nextToken();
            }
            if(token.type==TokenType::ELSE){
                token=lexer->nextToken();
                if(token.type==TokenType::COLON){
                    token=lexer->nextToken();
                    if(token.type==EOL){
                        if(lexer->nextLine()) token=lexer->nextToken();
                        else throw SyntaxError(lexer->modname,token);
                        if(token.type==TokenType::INDENT)
                            stmtif->elseblock=blockParser();
                        else throw SyntaxError(lexer->modname,token);
                        token=lexer->nextToken();
                    }
                    else throw SyntaxError(lexer->modname,token);
                }
                else throw SyntaxError(lexer->modname,token);
            }
            return stmtif;
        }
        else throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
}

StmtElif *Parser::elifParser(){
    Expr *condition=exprParser();
    if(token.type==TokenType::COLON){
        token=lexer->nextToken();
        if(token.type==EOL){
            if(lexer->nextLine()) token=lexer->nextToken();
            else throw SyntaxError(lexer->modname,token);
            if(token.type==TokenType::INDENT){
                StmtElif *stmtelif=new StmtElif(condition,blockParser());
                stmtelif->modname=lexer->modname;
                stmtelif->line=token.row;
                return stmtelif;
            }
            else throw SyntaxError(lexer->modname,token);
        }
        else throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
}

StmtWhile *Parser::whileParser(){
    Expr *condition=exprParser();
    if(token.type==TokenType::COLON){
        token=lexer->nextToken();
        if(token.type==EOL){
            if(lexer->nextLine()) token=lexer->nextToken();
            else throw SyntaxError(lexer->modname,token);
            if(token.type==TokenType::INDENT){
                StmtWhile *stmtwhile=new StmtWhile(condition,blockParser());
                stmtwhile->modname=lexer->modname;
                stmtwhile->line=token.row-1;
                return stmtwhile;
            }
            else throw SyntaxError(lexer->modname,token);
        }
        else throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
}

StmtFor *Parser::forParser(){
    StmtFor *stmtfor=new StmtFor();
    stmtfor->modname=lexer->modname;
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
            else throw SyntaxError(lexer->modname,token);
            token=lexer->nextToken();
            if(token.type==TokenType::COLON){
                token=lexer->nextToken();
                if(token.type==EOL){
                    if(lexer->nextLine()) token=lexer->nextToken();
                    else throw SyntaxError(lexer->modname,token);
                    if(token.type==TokenType::INDENT){
                        stmtfor->forblock=blockParser();
                        if(token.type==TokenType::DEDENT)
                            return stmtfor;
                        else throw SyntaxError(lexer->modname,token);
                    }
                    else throw SyntaxError(lexer->modname,token);
                }
                else throw SyntaxError(lexer->modname,token);
            }
            else throw SyntaxError(lexer->modname,token);
        }
        else throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
}

StmtPrint *Parser::printParser(){
    StmtPrint *stmtprint=new StmtPrint();
    stmtprint->modname=lexer->modname;
    stmtprint->line=token.row;
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        if(token.col<=lexer->line.size()){
            if(lexer->line[token.col-1]!=')'){
                while(true){
                    stmtprint->printlist.push_back(exprParser());
                    if(token.type!=TokenType::COMMA) break;
                }
            }
            else token=lexer->nextToken();
        }
        else throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
    if(token.type==TokenType::RPAR){
        token=lexer->nextToken();
        if(token.type!=EOL)
            throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
    return stmtprint;
}

StmtPrintLn *Parser::printlnParser(){
    StmtPrintLn *stmtprintln=new StmtPrintLn();
    stmtprintln->modname=lexer->modname;
    stmtprintln->line=token.row;
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        if(token.col<=lexer->line.size()){
            if(lexer->line[token.col-1]!=')'){
                while(true){
                    stmtprintln->printlist.push_back(exprParser());
                    if(token.type!=TokenType::COMMA) break;
                }
            }
            else token=lexer->nextToken();
        }
        else throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
    if(token.type==TokenType::RPAR){
        token=lexer->nextToken();
        if(token.type!=EOL)
            throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
    return stmtprintln;
}

StmtReturn *Parser::returnParser(){
    Expr *exprret=exprParser();
    if(token.type!=EOL)
        throw SyntaxError(lexer->modname,token);
    StmtReturn *stmtreturn=new StmtReturn(exprret);
    stmtreturn->modname=lexer->modname;
    stmtreturn->line=token.row;
    return stmtreturn;
}

StmtBreak *Parser::breakParser(){
    token=lexer->nextToken();
    if(token.type!=EOL)
        throw SyntaxError(lexer->modname,token);
    StmtBreak *stmtbreak=new StmtBreak();
    stmtbreak->modname=lexer->modname;
    stmtbreak->line=token.row;
    return stmtbreak;
}

StmtContinue *Parser::continueParser(){
    token=lexer->nextToken();
    if(token.type!=EOL)
        throw SyntaxError(lexer->modname,token);
    StmtContinue *stmtcontinue=new StmtContinue();
    stmtcontinue->modname=lexer->modname;
    stmtcontinue->line=token.row;
    return stmtcontinue;
}

Statement *Parser::statementPParser(){
    ExprID *exprid=new ExprID(token.lexeme);
    exprid->modname=lexer->modname;
    exprid->line=token.row;
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        ExprMethodCall *exprmethodcall=new ExprMethodCall(exprid->varname);
        exprmethodcall->modname=lexer->modname;
        exprmethodcall->line=token.row;
        StmtMethodCall *stmtmethodcall=new StmtMethodCall(exprmethodcall);
        stmtmethodcall->modname=lexer->modname;
        stmtmethodcall->line=token.row;
        if(token.type==TokenType::LPAR){
            if(token.col<=lexer->line.size()){
                if(lexer->line[token.col-1]!=')'){
                    while(true){
                        exprmethodcall->arglist.push_back(exprParser());
                        if(token.type!=TokenType::COMMA) break;
                    }
                }
                else token=lexer->nextToken();
            }
            else throw SyntaxError(lexer->modname,token);
        }
        else token=lexer->nextToken();
        if(token.type==TokenType::RPAR){
            token=lexer->nextToken();
            if(token.type!=EOL) throw SyntaxError(lexer->modname,token);
        }
        else throw SyntaxError(lexer->modname,token);
        return stmtmethodcall;
    }
    else if(token.type==TokenType::ASSIGN){
        if(exprid->varname.find(".")==exprid->varname.npos){
            StmtAssign *stmtassign=new StmtAssign();
            stmtassign->lexpr=exprid;
            stmtassign->rexpr=exprParser();
            stmtassign->modname=lexer->modname;
            stmtassign->line=token.row;
            return stmtassign;
        }
        else throw SyntaxError(lexer->modname,token);
    }
    else if(token.type==TokenType::LBRACK){
        if(exprid->varname.find(".")==exprid->varname.npos){
            Expr *index=exprParser();
            index->modname=lexer->modname;
            index->line=token.row;
            if(token.type==TokenType::RBRACK){
                ExprArray *exprarray=new ExprArray(exprid->varname,index);
                exprarray->modname=lexer->modname;
                exprarray->line=token.row;
                token=lexer->nextToken();
                if(token.type==TokenType::ASSIGN){
                    Expr *rexpr=exprParser();
                    rexpr->modname=lexer->modname;
                    rexpr->line=token.row;
                    StmtAssign *stmtassign=new StmtAssign(exprarray,rexpr);
                    stmtassign->modname=lexer->modname;
                    stmtassign->line=token.row;
                    if(token.type!=EOL) throw SyntaxError(lexer->modname,token);
                    return stmtassign;
                }
                else throw SyntaxError(lexer->modname,token);
            }
            else throw SyntaxError(lexer->modname,token);
        }
        else throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
}

/****************************************************************/
/*************************运算处理级别函数*************************/

ExprRange *Parser::rangeParser(){
    ExprRange *exprrange=new ExprRange();
    exprrange->modname=lexer->modname;
    exprrange->line=token.row;
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        exprrange->begin=exprParser();
        if(token.type==TokenType::COMMA){
            exprrange->end=exprParser();
            if(token.type==TokenType::COMMA){
                exprrange->step=exprParser();
                if(token.type==TokenType::RPAR)
                    return exprrange;
                else throw SyntaxError(lexer->modname,token);
            }
            else if(token.type==TokenType::RPAR){
                exprrange->step=new ExprInteger(1);
                return exprrange;
            }
            else throw SyntaxError(lexer->modname,token);
        }
        else throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
}

Expr *Parser::exprParser(){
    token=lexer->nextToken();
    if(token.isExpr()||token.type==TokenType::LPAR||token.type==TokenType::LBRACK){
        switch(token.type){
            case TokenType::ID:
            case TokenType::LPAR:
            case TokenType::LBRACK:
            case TokenType::SUB:
                return logicOrParser();
            default:
                if(token.isConstant()) return logicOrParser();
                else throw SyntaxError(lexer->modname,token);
        }
    }
    else if(token.type==TokenType::INPUT){
        token=lexer->nextToken();
        if(token.type==TokenType::LPAR){
            token=lexer->nextToken();
            if(token.type==TokenType::STRING){
                string tip=token.lexeme;
                token=lexer->nextToken();
                if(token.type==TokenType::RPAR){
                    token=lexer->nextToken();
                    if(token.type==EOL){
                        ExprInput *exprinput=new ExprInput(tip);
                        return exprinput;
                    }
                    else throw SyntaxError(lexer->modname,token);
                }
                else throw SyntaxError(lexer->modname,token);
            }
            else throw SyntaxError(lexer->modname,token);
        }
        else throw SyntaxError(lexer->modname,token);
    }
    else throw SyntaxError(lexer->modname,token);
}

Expr *Parser::logicOrParser(){
    Expr *exprlogicor=logicAndParser();
    while(token.type==TokenType::OR){
        string opname=token.lexeme;
        token=lexer->nextToken();
        exprlogicor=new ExprLogic(opname,exprlogicor,logicAndParser());
        exprlogicor->modname=lexer->modname;
        exprlogicor->line=token.row;
    }
    return exprlogicor;
}

Expr *Parser::logicAndParser(){
    Expr *exprlogicand=relationParser();
    while(token.type==TokenType::AND){
        string opname=token.lexeme;
        token=lexer->nextToken();
        exprlogicand=new ExprLogic(opname,exprlogicand,relationParser());
        exprlogicand->modname=lexer->modname;
        exprlogicand->line=token.row;
    }
    return exprlogicand;
}

Expr *Parser::relationParser(){
    Expr *exprrelation=summaryParser();
    while(token.isCompare()){
        string opname=token.lexeme;
        token=lexer->nextToken();
        exprrelation=new ExprCompare(opname,exprrelation,summaryParser());
        exprrelation->modname=lexer->modname;
        exprrelation->line=token.row;
    }
    return exprrelation;
}

Expr *Parser::summaryParser(){
    Expr *exprsummary=productParser();
    while(token.type==TokenType::ADD||token.type==TokenType::SUB){
        string opname=token.lexeme;
        token=lexer->nextToken();
        exprsummary=new ExprArith(opname,exprsummary,productParser());
        exprsummary->modname=lexer->modname;
        exprsummary->line=token.row;
    }
    return exprsummary;
}

Expr *Parser::productParser(){
    Expr *exprproduct=bitwiseParser();
    //Expr *exprproduct=productParser();
    while(token.type==TokenType::MUL||token.type==TokenType::DIV||token.type==TokenType::MOD){
        string opname=token.lexeme;
        token=lexer->nextToken();
        exprproduct=new ExprArith(opname,exprproduct,bitwiseParser());
        exprproduct->modname=lexer->modname;
        exprproduct->line=token.row;
    }
    return exprproduct;
}

Expr *Parser::bitwiseParser(){
    Expr *exprbitwise=oppositeParser();
    while(token.type==TokenType::SLEFT||token.type==TokenType::SRIGHT){
        string opname=token.lexeme;
        token=lexer->nextToken();
        exprbitwise=new ExprBitwise(opname,exprbitwise,oppositeParser());
        exprbitwise->modname=lexer->modname;
        exprbitwise->line=token.row;
    }
    return exprbitwise;
}

Expr *Parser::oppositeParser(){
    int count=0;
    while(token.type==TokenType::SUB){
        token=lexer->nextToken();
        ++count;
    }
    if(count%2){
        ExprOpposite *expropposite=new ExprOpposite(termParser());
        expropposite->modname=lexer->modname;
        expropposite->line=token.row;
        return expropposite;
    }
    else return termParser();
}

Expr *Parser::termParser(){
    if(token.type==TokenType::ID) return exprPParser();
//    else if(token.type==TokenType::SUB) return new ExprOpposite(exprParser());这么写
    else if(token.type==TokenType::LPAR){
        Expr *term=exprParser();
        term->modname=lexer->modname;
        term->line=token.row;
        if(token.type==TokenType::RPAR) token=lexer->nextToken();
        else throw SyntaxError(lexer->modname,token);
        return term;
    }
    else if(token.type==TokenType::LBRACK){
        ExprArrayInit *exprarrayinit=new ExprArrayInit();
        exprarrayinit->modname=lexer->modname;
        exprarrayinit->line=token.row;
        while(true){
            exprarrayinit->initlist.push_back(exprParser());
            if(token.type!=TokenType::COMMA) break;
        }
        if(token.type==TokenType::RBRACK) token=lexer->nextToken();
        else throw SyntaxError(lexer->modname,token);
        return exprarrayinit;
    }
    else if(token.isConstant()) return constantParser();
    else throw SyntaxError(lexer->modname,token);
}

Expr *Parser::exprPParser(){
    string name=token.lexeme;
    token=lexer->nextToken();
    if(token.type==TokenType::LBRACK){
        ExprArray *exprarray=new ExprArray(name,exprParser());
        exprarray->modname=lexer->modname;
        exprarray->line=token.row;
        if(token.type==TokenType::RBRACK) token=lexer->nextToken();
        else throw SyntaxError(lexer->modname,token);
        return exprarray;
    }
    else if(token.type==TokenType::LPAR){
        ExprMethodCall *exprmethodcall=new ExprMethodCall(name);
        exprmethodcall->modname=lexer->modname;
        exprmethodcall->line=token.row;
        if(token.col<=lexer->line.size()){
            if(lexer->line[token.col-1]!=')'){
                while(true){
                    exprmethodcall->arglist.push_back(exprParser());
                    if(token.type!=TokenType::COMMA) break;
                }
            }
            else token=lexer->nextToken();
        }
        else throw SyntaxError(lexer->modname,token);
        if(token.type==TokenType::RPAR) token=lexer->nextToken();
        else throw SyntaxError(lexer->modname,token);
        return exprmethodcall;
    }
    else{
        ExprID *exprid=new ExprID(name);
        exprid->modname=lexer->modname;
        exprid->line=token.row;
        return exprid;
    }
}

Expr *Parser::constantParser(){
    if(token.type==TokenType::INTEGER){
        ExprInteger *exprinteger=new ExprInteger(atoi(token.lexeme.c_str()));
        exprinteger->modname=lexer->modname;
        exprinteger->line=token.row;
        token=lexer->nextToken();
        return exprinteger;
    }
    if(token.type==TokenType::FLOAT){
        ExprFloat *exprfloat=new ExprFloat(atof(token.lexeme.c_str()));
        exprfloat->modname=lexer->modname;
        exprfloat->line=token.row;
        token=lexer->nextToken();
        return exprfloat;
    }
    if(token.isBoolean()){
        bool val;
        if(token.type==TokenType::TRUE) val=true;
        else val=false;
        ExprBoolean *exprboolean=new ExprBoolean(val);
        exprboolean->modname=lexer->modname;
        exprboolean->line=token.row;
        token=lexer->nextToken();
        return exprboolean;
    }
    if(token.type==TokenType::STRING){
        ExprString *exprstring=new ExprString(token.lexeme);
        exprstring->modname=lexer->modname;
        exprstring->line=token.row;
        token=lexer->nextToken();
        return exprstring;
    }
    return NULL;
}

