#ifndef intepreter_h
#define intepreter_h

#include "lexer.h"
#include "ast.h"
using namespace std;

class Intepreter{
public:
    Intepreter(const string &path);
    /*获取Token类型*/
    bool isNum();
    bool isBoolean();
    bool isExpression();
    bool isCompare();
    bool isArithmetic();
    bool isStatement();
    ASTNode * program();

private:
    Token token;
    Lexer *lexer;
};

#endif
