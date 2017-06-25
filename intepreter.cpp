#include "Intepreter.h"

Intepreter::Intepreter(const string &filename){

}

void Intepreter::process(){
    lexicalAnaysis();
    syntacticAnaysis();
    semanticAnalysis();
    programExecution();
}

void Intepreter::lexicalAnaysis(){
    lexer();
}

void Intepreter::syntaxAnaysis(){
    parser(lexer);
    parser.process();
}

void Intepreter::semanticAnalysis(){

}
