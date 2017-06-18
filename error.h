#ifndef error_h
#define error_h

#include <iostream>
#include <exception>
#include <sstream>
using namespace std;

class CustomException:public exception{
public:
    explicit CustomException() _NOEXCEPT :exception() {}
    virtual ~CustomException() _NOEXCEPT {}
};
class LexicalError:public CustomException{
public:
    explicit LexicalError(const string &lexeme,char ch,int row,int col)
        _NOEXCEPT :CustomException(){
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

#endif
