#ifndef token_h
#define token_h

#include <unordered_map>
using namespace std;

#define EOL -2//行尾符号

enum TokenType{

    //算数运算符
    ADD,SUB,MUL,DIV,MOD,

    //赋值预算符
    SLEFT,SRIGHT,NEGATION,ASSIGN,

    //逻辑运算法
    GT,LT,GE,LE,EQ,DE,AND,OR,NOT,

    //标点符号
    LPAR,RPAR,LBRACK,RBRACK,LBRACE,RBRACE,
    COLON,COMMA,INDENT,

    //保留关键词
    CLASS,DEF,INIT,MAIN,IF,ELIF,ELSE,WHILE,
    FOR,IN,RANGE,RETURN,BREAK,TRUE,FALSE,
    INT2STR,STR2INT,INPUT,PRINT,END,

    //左值右值
    ID,INT,FLOAT,STRING//LIT_CADENAAAA
};


unordered_map<string,TokenType> TokenMap;
TokenMap["+"] = TokenType::ADD;//1
TokenMap["-"] = TokenType::SUB;//2
TokenMap["*"] = TokenType::MUL;//3
TokenMap["/"] = TokenType::DIV;//4
TokenMap["%"] = TokenType::MOD;//5

TokenMap["<<"] = TokenType::SLEFT;//6
TokenMap[">>"] = TokenType::SRIGHT;//7
TokenMap["~"] = TokenType::NEGATION;//8
TokenMap["="] = TokenType::ASSIGM#M#N;//9

TokenMap[">"] = TokenType::GT;//10
TokenMap["<"] = TokenType::LT;//11
TokenMap[">="] = TokenType::GE;//12
TokenMap["<="] = TokenType::LE;//13
TokenMap["=="] = TokenType::EQ;//14
TokenMap["!="] = TokenType::DE;//15
TokenMap["and"] = TokenType::AND;//16
TokenMap["or"] = TokenType::OR;//17
TokenMap["not"] = TokenType::NOT;//18

TokenMap[":"] = TokenType::COLON;//19
TokenMap[","] = TokenType::COMMA;//20
TokenMap["("] = TokenType::LPAR;//21
TokenMap[")"] = TokenType::RPAR;//22
TokenMap["["] = TokenType::LBRACK;//23
TokenMap["]"] = TokenType::RBRACK;//24
TokenMap["{"] = TokenType::LBRACE;//25
TokenMap["}"] = TokenType::RBRACE;//26

TokenMap["class"] = TokenType::CLASS;
TokenMap["def"] = TokenType::DEF;
TokenMap["__init__"] = TokenType::INIT;
TokenMap["main"] = TokenType::MAIN;
TokenMap["if"] = TokenType::IF;
TokenMap["elif"] = TokenType::ELIF;
TokenMap["else"] = TokenType::ELSE;
TokenMap["while"] = TokenType::WHILE;
TokenMap["for"] = TokenType::FOR;
TokenMap["in"] = TokenType::IN;
TokenMap["range"] = TokenType::RANGE;
TokenMap["return"] = TokenType::RETURN;
TokenMap["break"] = TokenType::BREAK;
TokenMap["true"] = TokenType::TRUE;
TokenMap["false"] = TokenType::FALSE;
TokenMap["int2str"] = TokenType::INT2STR;
TokenMap["str2int"] = TokenType::STR2INT;
TokenMap["input"] = TokenType::INPUT;
TokenMap["print"] = TokenType::PRINT;
TokenMap["end"] = TokenType::END;

class Token{
public:
    Token(){}
    Token(const string &name,TokenType type,int row,int col):
        str(str),type(type),row(row),col(col) {}
    //TokenType getType();
    bool isNum;
    bool isBoolean;
    bool isKeyword;
    bool isArithmetic;
    bool isCompare;
    bool
    string str;
    TokenType type;
    int row;
    int col;
}

#endif
