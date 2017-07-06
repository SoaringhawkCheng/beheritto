#include "intepreter.h"

Intepreter::Intepreter(const string &filename):filename(filename){}

Intepreter::~Intepreter(){}

void Intepreter::process(){
    lexicalAnaysis();
    syntaxAnaysis();
    programExecution();
}

void Intepreter::lexicalAnaysis(){
    lexer=new Lexer(filename);
    program=new DeclModule(lexer->modname);
}

void Intepreter::syntaxAnaysis(){
    parser=new Parser(lexer);
    parser->process();
}

void Intepreter::programExecution(){
    executer=new Executer();
    executer->process();
}
