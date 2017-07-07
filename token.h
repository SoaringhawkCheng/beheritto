#ifndef token_h
#define token_h

#include <unordered_map>
#include <string>
using namespace std;

#define EOL -2//行末符号

enum TokenType{

    //算数运算符
    ADD,SUB,MUL,DIV,MOD,

    //赋值预算符
    SLEFT,SRIGHT,NEGATION,ASSIGN,

    //逻辑运算法
    GT,LT,GE,LE,EQ,DE,AND,OR,NOT,

    //标点符号
    LPAR,RPAR,LBRACK,RBRACK,LBRACE,RBRACE,
    COLON,COMMA,INDENT,DEDENT,

    //保留关键词
    IMPORT,CLASS,DEF,DONE,INIT,
    IF,ELIF,ELSE,WHILE,FOR,IN,RANGE,
    RETURN,BREAK,TRUE,FALSE,INPUT,PRINT,

    //左值右值
    ID,INTEGER,FLOAT,STRING
};

class Token{
public:
    Token(){}
    Token(const string &lexeme,int type,int row,int col):
        lexeme(lexeme),type(type),row(row),col(col) {}
    //TokenType getObjType();
    bool isExpr();
    bool isCompare();
    bool isNumeric();
    bool isBoolean();
    bool isConstant();
    string lexeme;
    int type;
    int row;
    int col;
};

#endif
