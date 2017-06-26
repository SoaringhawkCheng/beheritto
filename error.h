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
class LoadingError:public BeherittoError{
public:
    explicit LoadingError(const string &modename) _NOEXCEPT :BeherittoError(){
        stringstream scin;
        scin<<"Loading error: module "<<modname<<" cannot be loaded"<<endl;
        getline(scin,errmsg);
    }
    const char *what() const _NOEXCEPT{
        return errmsg.c_str();
    }
    ~LoadingError() _NOEXCEPT {}
private:
    string errmsg;
}
class LexicalError:public BeherittoError{
public:
    explicit LexicalError(const string &modname,const string &lexeme,char ch,int row,int col)
        _NOEXCEPT :BeherittoError(){
        stringstream scin;
        scin<<"Lexical error: in module "<<modname<<" char ascii: "<<int(ch)<<" after "<<lexeme<<", at row "<<row<<", col "<<col;
        getline(scin,errmsg);
    }
    const char *what() const _NOEXCEPT{
        return errmsg.c_str();
    }
    ~LexicalError() _NOEXCEPT {}
private:
    string errmsg;
};

class SyntacticError:public BeherittoError{
public:
    explicit SyntacticError(const string &modname,Token token)
        _NOEXCEPT :BeherittoError(){
        stringstream scin;
        scin<<"Syntactic error: "<<"in module "<<modname<<" token: "<<token.lexme
            <<" at row "<<token.row<<", col "<<token.col;
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
        _NOEXCEPT :BeherittoError(){
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
