#include "intepreter.h"

Intepreter::Intepreter(const string &path){
    lexer=Lexer(path);
    token=Token();
}

bool Intepreter::isNum(){
    switch(token.type){
    case TokenType::INT:
    case TokenType::FLOAT:
        return true;
    default:
        return false;
    }
}

bool Intepreter::isBoolean(){
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
bool Intepreter::isCompare(){
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
bool Intepreter::isArithmetic(){
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
bool Intepreter::isStatement(){
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
}

ASTNode *program(){
    token=lexer->nextToken();
    if(token.type==TokenType::CLASS){
        token=lexer->nextToken();
        if(token.type==TokenType::ID){
            //new
            token=lex->nextToken();
            initBlock();

        }
    }
}
