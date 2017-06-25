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
}
bool Parser::isStatement(){
    switch(token.type){
        case TokenType::CLASS:
        case TokenType::DEF:
        case TokenType::IF:
        case TokenType::ELIF:
        case TokenType::ELSE:
        case TokenType::WHILE:
        case TokenType::FOR:
        case TokenType::IN:
        case TokenType::RETURN:
        case TokenType::BREAK:
        case TokenType::CONTINUE:
        case TokenType::INPUT:
        case TokenType::PRINT:
        case TokenType::READ:
        case TokenType::WRITE:
            return true;
        default:
            return false;
    }
}*/

/****************************************************************/
/****************程序处理的函数****************/

void rootParser(){
    program=new DeclProgram();
    while(lexer->nextLine()){
        token=lexer->nextToken();
        switch(token.type){
        case TokenType::FROM:
            fromParser(program);
            break;
        case TokenType::IMPORT:
            importParser(program);
            break;
        case TokenType::CLASS:
            classParser(program);
            break;
        case TokenType::DEF:
            methodParser(program);
            break;
        case TokenType::IF:
            entryParser(program);
            break;
        default:
            throw SyntaxError(lexer->modname,token);
        }
    }
}

void importParser(Decl *decl){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        string modname=token.lexeme;
        DeclModule *declmodule=(moduleParser(modname));
        decl->modulelist.push_back()
    }
    else
        throw SyntaxError(lexer->modname,token);
    token=lexer->nextToken();
    if(token.type!=TokenType::EOL)
        throw SyntaxError(lexer->modname,token);
}

void fromParser(Declaration * decl){
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
                            throw SyntaxError(lexer->modname,token;)
                    }
                    if(token.type!=TokenType::EOL)
                        throw SyntaxError(lexer->modname,token);
                    declclass->classname=classname;
                    decl->classlist.push_back(declclass);
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
                            throw SyntaxError(lexer->modname,token;)
                    }
                    if(token.type!=TokenType::EOL)
                        throw SyntaxError(lexer->modname,token);
                    declmethod->methodname=methodname;
                    decl->methodlist.push_back(declmethod);
                    return;
                }
                throw LoadingError(modname);
            }
            else
                throw SyntaxError(lexer->modname,token);
        }
        else
            throw SyntaxError(lexer->modname,token);
    }
    else
        throw SyntaxError(lexer->modname,token);
}


/****************************************************************/
/***************模块处理函数***************/

DeclModule *moduleParser(const string &modname){
    DeclModule *declmodule=new DeclModule(modname);
    lexerlist.push_back(modname);
    lexer=new Lexer(modname+".be");
    while(lexer->nextLine()){
        token=lexer->nextToken();
        switch(token.type){
        case TokenType::FROM:
            fromParser(declmodule);
        case TokenType::IMPORT:
            declmodule->modulelist.push_back(moduleParser(declmodule));
        case TokenType::CLASS:
            declmodule->classlist.push_back(classParser());
        case TokenType::DEF:
            declmodule->methodlist.push_back(methodParser());
        default:break;
        }
    }
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
                DeclClass *declmethod=mothodParser();
                lexer=lexerlist.top();
                lexerlist.pop();
                return declmethod;
            }
        }
    }
    return NULL;
}
/****************************************************************/
/*处理类声明的函数*/

DeclClass *classParser(Declaration *decl){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        string classname=token.lexeme;
        DeclClass *declclass=new DeclClass(classname);
        token=lexer->nextToken();
        if(token.type==TokenType::COlON){
            token=lexer->nextToken();
            if(token.type==TokenType::EOL){

            }
            else
                throw SyntaxError()
        }
    }
}

/****************************************************************/
/*函数级别的函数*/

void mainParser(){
    if(line==" __name__=="\"main\":"){
        if(lexer->nextLine()){
            DeclMain *declmain=dynamic_cast<DeclMain *>(mainParser())
        }
    }
}

ASTNode *declClass(){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        //curstackframe=new StackFrame(NULL);
        DeclClass *declclass=new DeclClass(token.lexeme);
        token=lexer->nextToken();
        if(token.type==TokenType::COLON){
            token=lexer->nextToken();
            initBlock();
            if(token.type==TokenType::DEF){

            }
        }
    }
    return declclass;
}

ASTNode *declmethod(){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        DeclMethod *declmethod=new DeclMethod(token.lexeme);
        token=lexer->nextToken();
        if(token.type==TokenType:LPAR){

        }
    }
    return declmethod;
}

void initBlock(){
    token=lexer->nextToken();
    if(token.type==TokenType::INDENT){
        int indent=token.lexeme/4;
        lineindent.push_back(indent);
        if()
    }
    else
}
