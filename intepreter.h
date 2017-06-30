#ifndef intepreter_h
#define intepreter_h

#include <vector>
#include <string>
#include <stack>
#include <sstream>

#include "lexer.h"
#include "astree.h"
#include "parser.h"

using namespace std;

class Deriver;
class Executer;

class Intepreter{
public:
    Intepreter(const string &filename);
    ~Intepreter();
    void process();//接口函数
private:
    /*流程函数*/
    void lexicalAnaysis();//词法分析
    void syntacticAnaysis();//语法分析
    void semanticAnalysis();//语义分析
    void programExecution();//解释执行

    string filename;//文件名
    DeclModule *program;//程序名
    Lexer *lexer;//词法分析器
    Parser *parser;//语法分析器
    Deriver *deriver;//语义分析器
    Executer *executer;//解释执行器
};

class Deriver{
public:
    Deriver(DeclModule *program):program(program){}
    void process(){
        program->analyzeSemantic();
    }
private:
    DeclModule *program;
};

class Executer{
public:
    Executer(DeclModule *program):program(program){}
    void process(){
        program->intepret();
    }
private:
    DeclModule *program;
};

#endif
