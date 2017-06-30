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
    IMPORT,FROM,AS,CLASS,DEF,INIT,SELF,
    IF,ELIF,ELSE,WHILE,FOR,IN,RANGE,
    RETURN,BREAK,TRUE,FALSE,INPUT,PRINT,

    //左值右值
    ID,INT,FLOAT,STRING,
};

unordered_map<string,TokenType> TokenMap;

class Token{
public:
    Token(){}
    Token(const string &lexeme,TokenType type,int row,int col):
        lexeme(lexeme),type(type),row(row),col(col) {}
    //TokenType getType();
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
        case TokenType::INT:
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

bool Token::isNumeric(){
    switch(type){
        case TokenType::INT:
        case TokenType::FLOAT:
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
        case TokenType::INT:
        case TokenType::FLOAT:
        case TokenType::STRING:
        case TokenType::TRUE:
        case TokenType::FALSE:
            return true;
        default:
            return false;
    }
}

#endif
