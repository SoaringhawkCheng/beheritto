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

bool Parser::isArithmetic(){
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
/****************************************************************/
/****************语法分析器接口函数****************/

void Parser::process(){
    astree=new DeclProgram();
    programParser(astree);
}

/****************************************************************/
/****************程序处理级别函数****************/

void Parser::programParser(ASTree *node){
    while(lexer->nextLine()){
        token=lexer->nextToken();
        switch(token.type){
        case TokenType::FROM:
            fromParser(node);
            break;
        case TokenType::IMPORT:
            importParser(node);
            break;
        case TokenType::CLASS:
            classParser(node);
            break;
        case TokenType::DEF:
            methodParser(node);
            break;
        case TokenType::IF:
            entryParser(node);
            break;
        default:
            throw SyntacticError(lexer->modname,token);
            break;
        }
    }
}

void Parser::importParser(ASTree *node){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        string modname=token.lexeme;
        node->modulelist.push_back(moduleParser(modname));
    }
    else throw SyntacticError(lexer->modname,token);
    token=lexer->nextToken();
    if(token.type!=TokenType::EOL)
        throw SyntacticError(lexer->modname,token);
}

void Parser::fromParser(ASTree *node){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        string modname=token.lexeme;
        token==lexer->nextToken();
        if(token.type==TokenType::IMPORT){
            token==lexer->nextToken();
            if(token.type==TokenType::ID){
                //string classname=token.lexeme;
                //lexerlist.push(lexer);
                //lexer(modulename+".be");
                DeclClass *declclass
                    =modClassParser(modname,token.lexeme);
                if(declclass!=NULL){
                    string classname=modname+token.lexeme;
                    token==lexer->nextToken;
                    if(token.type==TokenType::AS){
                        token==lexer->nextToken;
                        if(token==TokenType::ID){
                            classname=token.lexeme;
                            token=lexer->nextToken();
                        }
                        else
                            throw SyntacticError(lexer->modname,token;)
                    }
                    if(token.type!=TokenType::EOL)
                        throw SyntacticError(lexer->modname,token);
                    declclass->classname=classname;
                    node->classlist.push_back(declclass);
                    return;
                }
                DeclMethod *declmethod
                    =modMethodParser(modname,token.lexeme);
                if(declmethod!=NULL){
                    string methodname=modname+token.lexeme;
                    token==lexer->nextToken;
                    if(token.type==TokenType::AS){
                        token==lexer->nextToken;
                        if(token==TokenType::ID){
                            methodname=token.lexeme;
                            token=lexer->nextToken();
                        }
                        else
                            throw SyntacticError(lexer->modname,token;)
                    }
                    if(token.type!=TokenType::EOL)
                        throw SyntacticError(lexer->modname,token);
                    declmethod->methodname=methodname;
                    node->methodlist.push_back(declmethod);
                    return;
                }
                throw LoadingError(modname);
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
    DeclModule *declmodule=new DeclModule(modname);
    lexerlist.push(lexer);
    lexer=new Lexer(modname+".be");
    while(lexer->nextLine()){
        token=lexer->nextToken();
        switch(token.type){
        case TokenType::FROM:
            fromParser(declmodule);
            break;
        case TokenType::IMPORT:
            importParser(declmodule);
            break;
        case TokenType::CLASS:
            classParser(declmodule);
            break;
        case TokenType::DEF:
            methodParser(declmodule);
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

void Parser::classParser(ASTree *node){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        string classname=token.lexeme;
        DeclClass *declclass=new DeclClass(classname);
        node->classlist.push_back(declclass);
        token=lexer->nextToken();
        if(token.type==TokenType::COlON){
            token=lexer->nextToken();
            if(token.type==TokenType::EOL){
                if(lexer->nextLine()){
                    token=lexer->nextToken();
                    if(token.type==TokenType::INDENT)
                        classParserP(declclass);
                    else throw SyntacticError(lexer->modname,token);
                }
                else throw SyntacticError(lexer->modname,token);
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}

void Parser::classParserP(ASTree *node){
    token=lexer->nextToken();
    if(token.type==TokenType::PASS){
        token=lexer->nextToken();
        if(token.type==TokenType::EOL){
            lexer->nextLine();
            token=lexer->nextToken();
            if(token.type!=TokenType::DEDENT)
                throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else if(token.type==TokenType::DEF){
        do{
            token=lexer->nextToken();
            switch(token.type){
            case TokenType::INIT:
                constructorParser(node);
                break;
            case TokenType::ID:
                methodParser(node);
                break;
            default:
                throw SyntacticError(lexer->modname,token);
            }
            token=lexer->nextToken();
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}

/****************************************************************/
/***************函数处理级别函数***************/

void Parser::constructorParser(ASTree *node){
    DeclConstructor *declconstructor=new DeclConstructor();
    node->constructor=constructor;
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        paralistParser(declconstructor);
        token=lexer->nextToken();
        if(token.type==TokenType::SELF){
            token=lexer->nextToken();
            if(token.type==TokenType::COMMA){
                paralistParser(declconstructor);
                if(token.type==TokenType::COLON){
                    token=lexer->nextToken();
                    if(token.type==TokenType::EOL){
                        if(lexer->nextLine()){
                            token=lexer->nextToken();
                            if(token.type==TokenType::INDENT)
                                constructorBlockParser(declconstructor);
                            else throw SyntacticError(lexeme->modname,token);
                        }
                        else throw SyntacticError(lexeme->modname,token);
                    }
                    else throw SyntacticError(lexeme->modname,token);
                }
                else throw SyntacticError(lexeme->modname,token);
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}

void Parser::methodParser(ASTnode *node){
    DeclMethod *declmethod=new DeclMethod(token.lexeme);
    node->methodlist.push_back(declmethod);
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
                            blockParser(declmethod);
                        else throw SyntacticError(lexeme->modname,token);
                    }
                    else throw SyntacticError(lexeme->modname,token);
                }
                else throw SyntacticError(lexeme->modname,token);
            }
            else throw SyntacticError(lexeme->modname,token);
        }
        else throw SyntacticError(lexeme->modname,token);
    }
    else throw SyntacticError(lexeme->modname,token);
}

/****************************************************************/
/***************块处理级别函数***************/

void Parser::constructBlockParser(ASTree *node){//???构造函数机制还是没搞懂
    StackFrame *savedstack=curstack;
    curstack=new StackFrame(curstack);
    DeclConstructor *declconstructor=new DeclConstructor();
    node->constructor=declconstructor;
    token=lexer->nextToken();
    while(token.type==TokenType::SELF){
        token=lexer->nextToken();
        if(token.type==TokenType::STOP){
            token=lexer->nextToken();
            if(token.type==TokenType::ID)

    }
    throw SyntacticError(lexeme->modname,token);
}

void Parser::blockParser(ASTree *node){
    StackFrame *savedstack=curstack;
    curstack=new StackFrame(curstack);
    StmtBlock *stmtblock=new StmtBlock();
    node->block=stmtblock;
    token=lexer->nextToken;
    while(token.type!=TokenType::DEDENT){
        statementParser(block);
        lexer->nextLine();
        token=lexer->nextToken();
    }
    curstack=savedStack;
}

void Parser::statementParser(ASTree *node){
    token=lexer->nextToken();
    switch(token.type){
    case TokenType::IF:
        ifParser(node);
        break;
    case TokenType::WHILE:
        whileParser(node);
        break;
    case TokenType::FOR:
        forParser(node);
        break;
    case TokenType::INPUT:
        inputParser(node);
        break;
    case TokenType::PRINT:
        printParser(node);
        break;
    case TokenType::ID:
        statementPParser(node);
        break;
    case TokenType::RETURN:
        returnParser(node);
        break;
    default:break;
    }
}

/****************************************************************/
/***************语句处理级别函数***************/

void Parser::ifParser(ASTree *node){
    StmtIf *stmtif=new StmtIf();
    node->statements.push_back(stmtif);
    exprParser(stmtif);
    if(token.type==TokenType::COLON){
        token=lexer->nextToken();
        if(token.type==TokenType::EOL){
            lexer->nextLine();
            token=lexer->nextToken();
            if(token.type==TokenType::INDENT)
                blockParser();
            else throw SyntacticError(token.lexeme,token);
            token=lexer->nextToken();
            while(token.type==TokenType::ELIF){
                elifParser(stmtif);
                token=lexer->nextToken();
            }
            if(token.type==TokenType::ELSE)
                elseParser(stmtif);
            else throw SyntacticError(token.lexeme,token);
        }
        else throw SyntacticError(token.lexeme,token);
    }
    else throw SyntacticError(token.lexeme,token);
}

void Parser::elifParser(ASTree *node){
    StmtElif *stmtelif=new StmtElif();
    node->eliflist.push_back(stmtelif);
    exprParser(stmtelif);
    if(token.type==TokenType::COLON){
        token=lexer->nextToken();
        if(token.type==TokenType::EOL){
            if(token.type==TokenType::INDENT)
                blockParser(stmtelif);
        }
        else throw SyntacticError(token.lexeme,token);
    }
    else throw SyntacticError(token.lexeme,token);
}

void Parser::elseParser(ASTree *node){
    StmtElse *stmtelse=new stmtElse();
    node->else=stmtelse;
    token=lexer->nextToken();
    if(token.type==TokenType::EOL){
        lexer->nextLine();
        token=lexer->nextToken();
        if(token.type==TokenType::INDENT)
            blockParser(stmtelse);
        else throw SyntacticError(token.lexeme,token);
    }
    else throw SyntacticError(token.lexeme,token);
}

void Parser::whileParser(ASTree *node){
    StmtWhile *stmtwhile=new StmtWhile();
    node->statements.push_back(stmtwhile);
    exprParser(stmtwhile);
    if(token.type==TokenType::COLON){
        token=lexer->nextToken();
        if(token.type==TokenType::EOL){
            lexer->nextLine();
            token=lexer->nextToken();
            if(token.type==TokenType::INDENT)
                blockParser(stmtwhile);
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}

void Parser::forParser(ASTree *node){
    StmtFor *stmtfor=new StmtFor();
    node->statements.push_back(stmtfor);
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        stmtfor.varname=token.lexeme;
        token=token->NextToken();
        if(token.type==TokenType::IN){
            token=lexer->nextToken();
            if(token.type==TokenType::RANGE)
                rangeParser(stmtfor);
            else if(token.type==TokenType::ID){
                stmtfor.range=NULL;
                stmtfor.objectname=token.lexeme;
            }
            else throw SyntacticError(lexer->modname,token);
            token=lexer->nextToken();
            if(token.type==TokenType::COLON){
                token=lexer->nextToken();
                if(token.type==TokenType::EOL){
                    lexer->nextLine();
                    token=lexer->nextToken();
                    if(token.type==TokenType::INDENT)
                        blockParser(stmtfor);
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

void Parser::rangeParser(ASTree *node){
    StmtRange *stmtrange=new StmtRange();
    node->range=stmtrange;
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR){
        token=lexer->nextToken();
        if(token.type==TokenType::INT)
            stmtrange.begin=atoi(token.lexeme.c_str());
        else if(token.Type==TokenType::SUB){
            token=lexer->nextToken();
            if(token.Type==TokenType::INT)
                stmtrange.begin=atoi(token.lexeme.c_str())*(-1);
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
        token=lexer->nextToken();
        if(token.type==TokenType::COLON){
            token=lexer->nextToken();
            if(token.type==TokenType::INT)
                stmtrange.end=atoi(token.lexeme.c_str());
            else if(token.type==TokenType::SUB){
                token=lexer->nextToken();
                if(token.type=TokenType::INT)
                    stmtrange.end=atoi(token.lexeme.c_str())*(-1);
                else throw SyntacticError(lexer->modname,token);
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
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
                else throw SyntacticError(lexer->modname,token);
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
        token=lexer->nextToken();
        if(token.type==TokenType::RPAR) return;
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}

void Parser::printParser(ASTree *node){
    StmtPrint *stmtprint=new StmtPrint();
    node->statements.push_back(stmtprint);
    token=lexer->nextToken();
    if(token.type==TokenType::LPAR)
        exprlistParser(stmtprint);
    else throw SyntacticError(lexer->modname,token);
    if(token.type==TokenType::RPAR){
        token=lexer->nextToken();
        if(token.type==TokenType::EOL) return;
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}

void Parser::exprlistParser(ASTree *node){
    StmtExprList *stmtexprlist=new StmtExprList();
    node->exprlist=stmtexprlist;
    while(true){
        exprParser(stmtexprlist);
        if(token.type!=TokenType::COMMA)
            break;
    }
}

void returnParser(ASTree *node){
    Stmtreturn *stmtreturn=new StmtReturn();
    node->returnp=stmtreturn;

}

/****************************************************************/
/***************运算处理级别函数***************/

void exprParser(){
    if(isExpression()){

    }
}
