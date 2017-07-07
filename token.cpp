#include "token.h"

extern unordered_map<string, TokenType>TokenMap;

void initTokenMap(){
    TokenMap["+"]=TokenType::ADD;//1
    TokenMap["-"]=TokenType::SUB;//2
    TokenMap["*"]=TokenType::MUL;//3
    TokenMap["/"]=TokenType::DIV;//4
    TokenMap["%"]=TokenType::MOD;//5

    TokenMap["<<"]=TokenType::SLEFT;//6
    TokenMap[">>"]=TokenType::SRIGHT;//7
    TokenMap["~"]=TokenType::NEGATION;//8
    TokenMap["="]=TokenType::ASSIGN;//9

    TokenMap[">"]=TokenType::GT;//10
    TokenMap["<"]=TokenType::LT;//11
    TokenMap[">="]=TokenType::GE;//12
    TokenMap["<="]=TokenType::LE;//13
    TokenMap["=="]=TokenType::EQ;//14
    TokenMap["!="]=TokenType::DE;//15
    TokenMap["and"]=TokenType::AND;//16
    TokenMap["or"]=TokenType::OR;//17
    TokenMap["not"]=TokenType::NOT;//18

    TokenMap[":"]=TokenType::COLON;//19
    TokenMap[","]=TokenType::COMMA;//20
    TokenMap["("]=TokenType::LPAR;//21
    TokenMap[")"]=TokenType::RPAR;//22
    TokenMap["["]=TokenType::LBRACK;//23
    TokenMap["]"]=TokenType::RBRACK;//24
    TokenMap["{"]=TokenType::LBRACE;//25
    TokenMap["}"]=TokenType::RBRACE;//26

    TokenMap["import"]=TokenType::IMPORT;
    TokenMap["class"]=TokenType::CLASS;
    TokenMap["def"]=TokenType::DEF;
    TokenMap["done"]=TokenType::DONE;
    TokenMap["__init__"]=TokenType::INIT;
    TokenMap["if"]=TokenType::IF;
    TokenMap["elif"]=TokenType::ELIF;
    TokenMap["else"]=TokenType::ELSE;
    TokenMap["while"]=TokenType::WHILE;
    TokenMap["for"]=TokenType::FOR;
    TokenMap["in"]=TokenType::IN;
    TokenMap["range"]=TokenType::RANGE;
    TokenMap["return"]=TokenType::RETURN;
    TokenMap["break"]=TokenType::BREAK;
    TokenMap["true"]=TokenType::TRUE;
    TokenMap["false"]=TokenType::FALSE;
    TokenMap["input"]=TokenType::INPUT;
    TokenMap["print"]=TokenType::PRINT;
}

bool Token::isExpr(){
    switch(type){
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
        case TokenType::INTEGER:
        case TokenType::FLOAT:
        case TokenType::STRING:
        case TokenType::TRUE:
        case TokenType::FALSE:
            return true;
        default:
            return false;
    }
}

bool Token::isCompare(){
    switch(type){
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

bool Token::isBoolean(){
    switch(type){
        case TokenType::TRUE:
        case TokenType::FALSE:
            return true;
        default:
            return false;
    }
}

bool Token::isConstant(){
    switch(type){
        case TokenType::INTEGER:
        case TokenType::FLOAT:
        case TokenType::STRING:
        case TokenType::TRUE:
        case TokenType::FALSE:
            return true;
        default:
            return false;
    }
}
