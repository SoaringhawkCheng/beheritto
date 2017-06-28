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
    ASTree *program;//程序名
    Lexer *lexer;//词法分析器
    Parser *parser;//语法分析器
    Deriver *deriver;//语义分析器
    Executer *executer;//解释执行期
};

class Deriver{
public:
    Deriver(ASTree *program):program(program){}
    void process(){
        program->derive();
    }
private:
    ASTree *program;
;}

class Executer{
public:
    Executer(ASTree *program):program(program){}
    void process(){
        program->execute();
    }
private:
    ASTree *program;
;}

#endif
