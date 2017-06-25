#ifndef intepreter_h
#define intepreter_h

#include <vector>
#include <string>
#include <stack>
#include <sstream>

#include "lexer.h"
#include "ast.h"

using namespace std;

class Intepreter{
public:
    Intepreter(const string &filename);
    ~Intepreter();
    void process();//接口函数
private:
    /*流程函数*/
    void lexicalAnaysis();
    void syntacticAnaysis();
    void semanticAnalysis();
    void execution();

    ASTree *tree;
    Lexer *lexer;
    Parser *parser;
    Deriver *deriver;
    Executer *executer;
};

#endif
