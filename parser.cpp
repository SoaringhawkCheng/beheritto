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

bool Parser::isBoolean(){
    switch(token.type){
    case TokenType::TRUE:
    case TokenType::FALSE:
        return true;
    default:
        return false;
    }
}

bool isExpression(){
    switch(token.type){
    case TokenType::ADD:
    case TokenType::SUB:
    case TokenType::MUL:
    case TokenType::DIV:
    case TokenType::MOD:
    case TokenType::SLEFT:
    case TokenType::SRIGHT:
    case TokenType::
    case TokenType::
    case TokenType::
    case TokenType::
    case TokenType::
    case TokenType::
    case TokenType::
    case TokenType::
    case TokenType::
    case TokenType::
    case TokenType::
    case TokenType::
    case TokenType::
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

/****************************************************************/
/****************语法分析器接口函数****************/

void Parser::process(){
    astree=new DeclProgram();
    programParser(astree);
}

/****************************************************************/
/****************程序处理级别函数****************/

void programParser(ASTree *node){
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

void importParser(ASTree *node){
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

void fromParser(ASTree *node){
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

DeclModule *moduleParser(const string &modname){
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

DeclClass *modClassParser(const string &modname,const string &classname){
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

DeclMethod *modMethodParser(const string &modname,const string &methodname){
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

void classParser(ASTree *node){
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

void classParserP(ASTree *node){
    do{
        token=lexer->nextToken();
        if(token.type==TokenType::DEF){
            token=lexer->nextToken();
            switch(token.type){
            case TokenType::INIT:
                constructorParser(node);break;
            case TokenType::ID:
                methodParser(node);break;
            default:
                throw SyntacticError(lexer->modname,token);break;
            }
        }
        else throw SyntacticError(lexer->modname,token);
    }
    while(lexer->nextLine());
}



/****************************************************************/
/***************函数处理级别函数***************/

void constructorParser(ASTree *node){
    DeclConstructor *constructor=new DeclConstructor();
    node->constructor=constructor;
    paralistParser(constructor);
    token=lexer->nextToken();
    if(token.type==TokenType::COLON){
        token=lexer->nextToken();
        if(token.type==TokenType::EOL){
            if(lexer->nextLine()){
                token=lexer->nextToken();
                if(token.type==TokenType::INDENT)
                    blockParser(constructor);
                else throw SyntacticError(lexer->modname,token);
            }
            else throw SyntacticError(lexer->modname,token);
        }
        else throw SyntacticError(lexer->modname,token);
    }
    else throw SyntacticError(lexer->modname,token);
}

void methodParser(ASTnode *node){
    DeclMethod *declmethod=new DeclMethod(token->lexeme);
    node->methodlist.push_back(declmethod);
    paralistParser(declmethod);
    token=lexer->nextToken();
    if(token.type==TokenType::COLON){
        token=lexer->nextToken();
        if(token.type==TokenType::EOL){
            if(lexer->nextLine()){
                token=lexer->nextToken();
                if(token.type==TokenType::INDENT)
                    blockParser(declmethod);
                else throw SyntacticError(lexeme->modename,token);
            }
            else throw SyntacticError(lexeme->modname,token);
        }
        else throw SyntacticError(lexeme->modname,token);
    }
    else throw SyntacticError(lexeme->modname,token);
}

/****************************************************************/
/***************块处理级别函数***************/

void constructBlockParser(ASTree *node){
}

void blockParser(ASTree *node){
    StackFrame *savedstack=curstack;
    curstackframe=new StackFrame(curstack);
    StmtBlock *block=new StmtBlock();
    token=lexer->nextToken;
    while(token.type!=TokenType::DEDENT){
        statementParser();
        token=lexer->nextToken();
    }
    curstack=savedStack;
}

void statementParser(){
    token=lexer->nextToken();
    switch(token.type){
    case TokenType::IF:
        ifParser();
        break;
    case TokenType::WHILE:
        whileParser();
        break;
    case TokenType::FOR:
        forParser();
        break;
    case TokenType::RETURN:
        returnParser();
        break;
    case TokenType::INPUT:
        inputParser();
        break;
    case TokenType::PRINT:
        printParser();
        break;
    case TokenType::ID:
        statementPParser();
        break;
    default:break;
    }
}

/****************************************************************/
/***************语句处理级别函数***************/


