#include "parser.h"

Parser::Parser(const string &path){
    lexer=Lexer(path);
    token=Token();
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

ASTNode *createSyntaxTree(){
    ASTNode root;
    token=lexer->nextToken();
    if(token.type==TokenType::CLASS){
        token=lexer->nextToken();
        if(token.type==TokenType::ID){
            //new
            token=lex->nextToken();
            initBlock();

        }
    }
};
