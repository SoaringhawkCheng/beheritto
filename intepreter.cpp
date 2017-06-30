#include "intepreter.h"

Intepreter::Intepreter(const string &filename):
    filename(filename){
}

Intepreter::~Intepreter(){

}
void Intepreter::process(){
    lexicalAnaysis();
    syntacticAnaysis();
    semanticAnalysis();
    programExecution();
}

void Intepreter::lexicalAnaysis(){
    lexer=new Lexer(filename);
    program=new DeclModule(lexer->modname);
}

void Intepreter::syntaxAnaysis(){
    parser=new parser(lexer,program);
    parser->process();
}

void Intepreter::semanticAnalysis(){
    deriver=new Deriver(program);
    deriver->process();
}

void Intepreter::programExecution(){
    executer=new Executer(program);
    executer->process();
}
