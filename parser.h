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
    Parser(Lexer *lexer,DeclModule *program);
    ~Parser();
    void process();
private:
    void programParser();
    DeclModule *importParser();
    
    void fromParser();
    DeclModule *moduleParser(const string &modname);
    DeclClass *modClassParser(const string &modname,const string &classname);
    DeclMethod *modMethodParser(const string &modname,const string &methodname);

    //DeclFunction *functionParser();
    DeclClass *classParser();
    DeclEntry *entryParser();
    
    DeclField *fieldParser();
    
    void constructorParser(DeclClass *declclass);
    void constructorBlockParser(DeclClass *declclass);
    
    DeclMethod *methodParser();
    StmtBlock *blockParser();
    
    vector<string> paraListParser();
    
    Statement *statementParser();
    StmtAssign *assignParser();
    StmtIf *ifParser();
    StmtElif *elifParser();
    StmtWhile *whileParser();
    StmtFor *forParser();
    StmtRange *rangeParser();
    StmtPrint *printParser();
    StmtReturn *returnParser();
    Statement *statementPParser();
    

    Expr *assignPParser();
    Expr *exprParser();
    Expr *logicOrParser();
    Expr *logicAndParser();
    Expr *relationParser();
    Expr *summaryParser();
    Expr *productParser();
    Expr *bitwiseParser();
    Expr *termParser();
    Expr *exprPParser();
    Expr *constantParser();

    stack<Lexer *> lexerlist;
    Lexer *lexer;
    Token token;
    DeclModule *program;

};

#endif
