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
class LexicalError:public BeherritoError{
public:
    explicit LexicalError(const string &lexeme,char ch,int row,int col)
        _NOEXCEPT :BeherritoError(){
        stringstream scin;
        scin<<"Lexical error: char ascii: "<<int(ch)<<" after "<<lexeme<<", at row "<<row+1<<",col "<<col+1;
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
    explicit SyntaxError(const string &lexeme,char ch,int row,int col)
        _NOEXCEPT :BeherritoError(){
        stringstream scin;
        scin<<"Analytical error: char ascii: "<<int(ch)<<" after "<<lexeme<<", at row "<<row+1<<",col "<<col+1;
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
        scin<<"Analytical error: char ascii: "<<int(ch)<<" after "<<lexeme<<", at row "<<row+1<<",col "<<col+1;
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
