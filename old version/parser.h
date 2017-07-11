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
    Parser(Lexer *lexer);
    ~Parser();
    void process();
private:
    void programParser();
    DeclModule *importParser();
    
    void fromParser();
    DeclModule *moduleParser(const string &path,const string &modname);
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
    ExprRange *rangeParser();
    StmtBreak *breakParser();
    StmtContinue *continueParser();
    StmtPrint *printParser();
    StmtPrintLn *printlnParser();
    StmtReturn *returnParser();
    Statement *statementPParser();
    

    Expression *assignPParser();
    Expression *exprParser();
    Expression *arglistParser();
    Expression *logicOrParser();
    Expression *logicAndParser();
    Expression *relationParser();
    Expression *summaryParser();
    Expression *productParser();
    Expression *bitwiseParser();
    Expression *oppositeParser();
    Expression *termParser();
    Expression *exprPParser();
    Expression *constantParser();

    stack<Lexer *> lexerlist;
    Lexer *lexer;
    Token token;
};

#endif
