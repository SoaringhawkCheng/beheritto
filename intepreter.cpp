#include "intepreter.h"

Intepreter::Intepreter(const string &filename):filename(filename){}

Intepreter::~Intepreter(){}

void Intepreter::process(){
    lexicalAnaysis();
    syntacticAnaysis();
    semanticAnalysis();
    programExecution();
}

void Intepreter::lexicalAnaysis(){
    lexer=new Lexer(filename);
    program=new DeclModule(lexer->modname);
    symboltable.program=program;
}

void Intepreter::syntacticAnaysis(){
    parser=new Parser(lexer);
    parser->process();
}

void Intepreter::semanticAnalysis(){
    deriver=new Deriver();
    deriver->process();
}

void Intepreter::programExecution(){
    executer=new Executer();
    executer->process();
}
