#ifndef intepreter_h
#define intepreter_h

#include "lexer.h"
#include "ast.h"
using namespace std;

class Intepreter{
public:
    Intepreter(const string &path);
    bool isNum();
    bool isBoolean();
    bool isExpr();

    bool isStatement();
private:
    Token token;
    Lexer *lexer;
};

#endif
