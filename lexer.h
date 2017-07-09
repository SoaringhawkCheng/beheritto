#ifndef lexer_h
#define lexer_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <ctype.h>

#include "token.h"
#include "error.h"
using namespace std;

class Lexer{
public:
    Lexer(const string &filename);
//    void initTokenMap();
    bool nextLine();//读取下一个非空行
    Token nextToken();//读取下一个单词
    string pathname;//当前路径名
    string modname;//当前模块名
    string line;//当前行内容
    string printline;//打印行
private:
    char nextChar();//读取下一个字符
    ifstream fin;//读取源文件
    int indent;//当前行缩进量
    stack<int> indentlist;//每行缩进情况
    int len;//当前行长度
    int row;//当前行
    int col;//当前列
    char ch;//当前字符值
    int state;//当前状态
    //int indentlevel;//当前行缩进量
};

#endif
