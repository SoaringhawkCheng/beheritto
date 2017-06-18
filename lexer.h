#ifndef lexer_h
#define lexer_h

#include <iostrem>
#include <fstream>
#include <string>
#include <vector>
#include <ctype.h>
using namespace std;

enum LexState{
    BEGIN,
    IDENTIFIER
}

class Lexer{
public:
    Lexer(const string &path);
    void errorHandler(char ch,int row,int col);
    char nextChar();
    Token nextToken();
    bool is
private:
    ifstream sourcefile;
    vector<string> code;
    int state;
    int row;
    int column;
};

#endif
