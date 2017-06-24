#include "parser.h"

Parser::Parser(const string &filename){
    lexer=new Lexer(path);
    token=Token();
    modulename=filename;
    modulename.erase(module.rfind("."));
};

bool Parser::isNum(){
    switch(token.type){
    case TokenType::INT:
    case TokenType::FLOAT:
        return true;
    default:
        return false;
    }
};

bool Parser::isBoolean(){
    switch(token.type){
    case TokenType::TRUE:
    case TokenType::FALSE:
        return true;
    default:
        return false;
    }
};

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
};
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
};
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
};
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
};

ASTNode programParser(){
    ASTNode *declprogram=new DeclProgram();
    token=lexer->nextToken();
    while(token.type!=TokenType::EOF){
        switch(token.type){
        case TokenType::FROM:fromParser();break;
        case TokenType::ID:importParser();break;
        case TokenType::CLASS:classParser();break;
        case TokenType::DEF:functionParser();break;
        default:
            throw SyntaxError(lexer->modname,token);
        }

    }
}

void fromParser(){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        string modname=token.lexeme;
        token==lexer->nextToken();
        if(token.type==TokenType::IMPORT){
            token==lexer->nextToken();
            if(token.type==TokenType::ID){
                string classname=token.lexeme;
                //lexerlist.push(lexer);
                //lexer(modulename+".be");
                DeclClass *declclass
                    =dynamic_cast<DeclClass *>(modclassParser(classname));
                classname=modname+classname;
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

void importParser(){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        string modname=token.lexeme;
        DeclModule *declmodule=dynamic_cast<DeclModule *>(moduleParser(modname));
        declmodlist.push_back(declmodule);
    }
    else
        throw SyntaxError(lexer->modname,token);
    token=lexer->nextToken();
    if(token.type!=TokenType::EOL)
        throw SyntaxError(lexer->modname,token);
}

ASTNode *modclassparser

ASTNode Parser(){
    ASTNode *declprogram=new DeclProgram();
    token=lexer->nextToken();
    switch(token.type){
    case TokenType::CLASS:
        DeclClass *declclass=dynamic_cast<DeclClass *>(classParser());
        declprogram->declclasslist.push_back(declclass);
        break;
    case TokenType::DEF:
        Declfunction *declfunction=dynamic_cast<Declfunction *>(functionParser());
        declprogram->declfunclist.push_back(declfunction);
        break;
    default:
        throw SyntaxError(token.lexeme);
    return declprogram;
};

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

ASTNode *declFunction(){
    token=lexer->nextToken();
    if(token.type==TokenType::ID){
        DeclMethod *declfunction=new DeclFunction(token.lexeme);
        token=lexer->nextToken();
        if(token.type==TokenType:LPAR){

        }
    }
    return declfunction;
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
