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
    //bool isKeyword();
    bool isVar();
    ASTNode * program();
private:
    Token token;
    Lexer *lexer;
    vector<int> lineindent;
    vector<DeclClass *> declclasslist;
    vector<DeclFunction &> declfunctionlist;
    StackFrame *curstackframe;
    Statement *curstatement;

    ASTNode *createSyntaxTree();
    ASTNode *defineClass();
    ASTNode *defineFunction();
    ASTNode *defineGlobalVar();
};

#endif
