#include "intepreter.h"

extern void initTokenMap();

Intepreter::Intepreter(const string &filename):filename(filename){}

Intepreter::~Intepreter(){}

void Intepreter::process(){
    cout<<"**************Program**************"<<endl;
    cout<<endl;
    lexicalAnaysis();
    syntaxAnaysis();
    cout<<endl;
    cout<<"***************Result**************"<<endl;
    cout<<endl;
    programExecution();
}

void Intepreter::lexicalAnaysis(){
    initTokenMap();
    lexer=new Lexer(filename);
    program=new DeclModule(lexer->modname);
    state=StateType::STATEANALYZE;
}

void Intepreter::syntaxAnaysis(){
    parser=new Parser(lexer);
    parser->process();
    state=StateType::STATEINTERM;
}

void Intepreter::programExecution(){
    executer=new Executer();
    executer->process();
}
