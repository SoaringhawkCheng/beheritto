#ifndef token_h
#define token_h

#include <unordered_map>
using namespace std;

#define EOL -2;

enum TokenType{
    ID,

    //算数运算符
    ADD,SUB,MUL,DIV,MOD,

    //赋值预算符
    SLEFT,SRIGHT,NEGATION,ASSIGN,

    //逻辑运算法
    GT,LT,GE,LE,EQ,DE,AND,OR,NOT,

    //占位符
    COLON,COMMA,LPAR,RPAR,LBRACK,RBRACK,LBRACE,RBRACE,INDENT,DETENT,

    //保留关键词
    CLASS,DEF,IF,ELIF,ELSE,WHILE,FOR,
    IN,RETURN,BREAK,PRINT,READ,WRITE,
    TRUE,FALSE,

    //字面值
    INT,FLOAT,STRING//LIT_CADENA
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
TokenMap["="] = TokenType::ASSIGN;//9

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
TokenMap["if"] = TokenType::IF;
TokenMap["elif"] = TokenType::ELIF;
TokenMap["else"] = TokenType::ELSE;
TokenMap["while"] = TokenType::WHILE;
TokenMap["for"] = TokenType::FOR;
TokenMap["in"] = TokenType::IN;
TokenMap["return"] = TokenType::RETURN;
TokenMap["break"] = TokenType::BREAK;
TokenMap["print"] = TokenType::PRINT;
TokenMap["read"] = TokenType::READ;
TokenMap["write"] = TokenType::WRITE;
TokenMap["true"] = TokenType::TRUE;
TokenMap["false"] = TokenType::FALSE;

class Token{
public:
    Token(){}
    Token(string w,TokenType t,int r,int c):
        word(w),type(t),row(r),col(c) {}
    TokenType getType();
    string word;
    TokenType type;
    int row;
    int col;
}

#endif
