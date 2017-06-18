#include "lexer.h"
#include "token.h"
#include "error.hpp"
using namespace std;

Lexer::Lexer(const string &path){
    sourcefile.open(path.c_str());
    string line;
    while(getline(sourcefile,line)){
        line=line+'\n';
        code.push_back(line);
    }
    state=0;
    row=0;
    col=0;
}

void Lexer::errorHandler(char ch,int row,int col){
    stringstream errmsg;
    errmsg<<"Lexical error: character "<<ch<<" invalid (row "\
        <<row+1<<", col "<<col+1<<")"<<endl;
    throw exception(errmsg.str().c_str());
}

char Lexer::nextChar(){
    if(col>=code[row].length()) ++row,col=0;
    if(row>=code.size()) return EOF;
    return code[row][col];
}

Token Lexer::nextToken(){
    string lexeme;
    while(true){
        char ch=nextChar();
        switch(state){
        case 0:{
            if(ch==' '||ch=='\n'){
                state=0;
                ch=nextChar();
            }
            else if(ch=='_'||isalpha(ch)){
                state=1;
                lexeme.append(ch);
                ch=nextChar();
            }
            else if(isdigit(ch)){
                state=2;
                lexeme.append(ch);
                ch=nextChar();
            }
        }
        break;

        case 1:{
            if(isalnum(ch)||ch=='_'){
                state=1;
                lexeme.append(ch);
                ch=nextChar();
            }
            else if(TokenMap.find(lexeme)!=TokenMap.end()){
                state=0;
                return Token(lexeme,TokenMap[lexeme],row,col);
            }
            else{
                state=0;
                return Token(laxeme,TokenType::ID,row,col);
            }
        }
        break;

        case 2:{
            if(isdigit(ch)){
                state=2;
                lexeme.append(ch);
                ch=nextChar();
            }
            else if(ch=='.'){
                state=3;
                lexeme.append(ch);
                ch=nextChar();
            }
            else{//?这边不考虑小数吗
                state=0;
                return Token(lexeme,TokenType::INT,row,col);
            }
        }
        break;

        case 3:{
            if(isdigit(ch)){
                state=4;
                lexeme.append(ch);
                ch=nextChar();
            }
            else{
            }
        }
        break;
    }
}
