#ifndef parser_h
#define parser_h

#include <stack>
#include <stdlib.h>
#include <iostream>

#include "lexer.h"
#include "astree.h"

using namespace std;

class Parser{
public:
    Parser(Lexer *lexer,ASTree *program);
    ~Parser();
    process();
private:
    void programParser();
    DeclModule *importParser(DeclProgram *program);
    
    void fromParser(DeclProgram *program);
    DeclModule *moduleParser(const string &modname);
    DeclClass *modClassParser(const string &modname,const string &classname);
    
    DeclMethod *modMethodParser(const string &modname,const string &methodname);
    DeclClass *classParser();
    
    DeclConstructor *constructorParser();
    DeclMethod *methodParser();
    
    StmtBlock *constructorBlockParser();
    StmtBlock *blockParser();
    Statement *statementParser();
    
    Stmtif ifParser();
    StmtElif
    
    void methodParser();
    void entryParser();


    stack<Lexer *> lexerlist;
    Lexer *lexer;
    Token token;
    TreeNode *program;

};

#endif
