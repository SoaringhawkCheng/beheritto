#ifndef error_h
#define error_h

#include <iostream>
#include <exception>
#include <sstream>
using namespace std;

class BeherritoError:public exception{
public:
    explicit BeherritoError() _NOEXCEPT :exception() {}
    virtual ~BeherritoError() _NOEXCEPT {}
};
class
class LexicalError:public BeherritoError{
public:
    explicit LexicalError(const string &modname,const string &lexeme,char ch,int row,int col)
        _NOEXCEPT :BeherritoError(){
        stringstream scin;
        scin<<"Lexical error: in module "<<modname<<" char ascii: "<<int(ch)<<" after "<<lexeme<<", at row "<<row+1<<",col "<<col+1;
        getline(scin,errmsg);
    }
    const char *what() const _NOEXCEPT{
        return errmsg.c_str();
    }
    ~ LexicalError() _NOEXCEPT {}
private:
    string errmsg;
};

class SyntaxError:public BeherittoError{
public:
    explicit SyntaxError(const string &modname,Token token)
        _NOEXCEPT :BeherritoError(){
        stringstream scin;
        scin<<"Syntax error: "<<"in module "<<modname<<" token: "<<token.lexme
            <<" at row "<<token.row+1<<",col "<<token.col+1;
        getline(scin,errmsg);
    }
    const char *what() const _NOEXCEPT{
        return errmsg.c_str();
    }
    ~ SyntaxError() _NOEXCEPT {}
private:
    string errmsg;
};
class SemanticError:public BeherittoError{
public:
    explicit SemanticError(const string &lexeme,char ch,int row,int col)
        _NOEXCEPT :BeherritoError(){
        stringstream scin;
        scin<<"Semantical error: char ascii: "<<int(ch)<<" after "<<lexeme<<", at row "<<row+1<<",col "<<col+1;
        getline(scin,errmsg);
    }
    const char *what() const _NOEXCEPT{
        return errmsg.c_str();
    }
    ~ SemanticError() _NOEXCEPT {}
private:
    string errmsg;
};

#endif
