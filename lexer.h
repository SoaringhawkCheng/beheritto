#ifndef lexer_h
#define lexer_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctype.h>

#include "token.h"
#include "error.h"
using namespace std;

enum LexState{
    BEGIN,
    IDENTIFIER
}

class Lexer{
public:
    Lexer(const string &path);
    char nextChar();//读取下一个字符
    Token nextToken();//读取下一个单词
private:
    ifstream fin;//读取源文件
    string line;//当前行内容
    int len;//当前行长度
    int row;//当前行
    int col;//当前列
    char ch;//当前字符值
    int state;//当前状态
    //int indentlevel;//当前行缩进量
    vector<int> lineindent;
};

#endif
