#ifndef parser_h
#define parser_h

#include <stack>
#include <stdlib.h>

#include "lexer.h"
#include "astree.h"

using namespace std;

class Parser{
public:
    Parser(Lexer *lexer,ASTree *program);
    ~Parser();
    process();
private:
    bool isStatement();

    void programParser();
    void importParser();
    void fromParser();
    void moduleParser();
    void modClassParser();
    void modMethodParser();
    void classParser();
    void methodParser();
    void entryParser();


    stack<Lexer *> lexerlist;
    Lexer *lexer;
    Token token;
    ASTree *astree;

};

#endif
