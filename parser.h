#ifndef parser_h
#define parser_h

#include <stack>
#include "lexer.h"
#include "astree.h"

using namespace std;

class Parser{
public:
    Parser(Lexer *lexer,ASTree *program);
    ~Parser();
    process();
private:
    void rootParser();
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
    ASTree *program;
};

#endif
