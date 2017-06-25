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
    void lexicalAnaysis();
    void syntacticAnaysis();
    void semanticAnalysis();
    void programExecution();

    string filename;
    ASTree *program;
    Lexer *lexer;
    Parser *parser;
    Deriver *deriver;
    Executer *executer;
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
