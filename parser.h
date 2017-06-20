#ifndef parser_h
#define parser_h

#include "lexer.h"
#include "ast.h"
using namespace std;

class Parser{
public:
    Parser(const string &path);
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
    StackFrame *curstackframe;
    Method
    Statement *curstatement;
};

#endif
