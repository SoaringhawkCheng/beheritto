//#include <iostream>
//#include <fstream>
//#include <string>
//#include <unordered_map>
//#include <stdlib.h>
//#include <unistd.h>
//#include <signal.h>
//#include <limits.h>
//
//#include "error.h"
//#include "lexer.h"
//#include "parser.h"
//#include "astree.h"
//#include "intepreter.h"
//
//using namespace std;
//
//unordered_map<string,TokenType> TokenMap;
//RuntimeStack runtimestack;
//SymbolTable symboltable;
//DeclModule *program=NULL;
//DeclModule *curmodule=NULL;
//DeclClass *curclass=NULL;
//DeclMethod *curmethod=NULL;
//StmtLoop *curloop=NULL;
//string curmodname="";
//int curline=-1;
//
//void errorHandler(string s);
//void sigHandler(int sig);
//void launchIntepreter(const string &filename);
//
//int main(int argc, const char * argv[]){
//    cout<<"ベヘリット Beheritto Version 1.0.0"<<endl;
//    if(signal(SIGINT,sigHandler)==SIG_ERR){
//        cout<<"Interpreter error, crashed!"<<endl;
//        exit(-1);
//    }
//    cout<<"Press Ctrl+C to exit"<<endl;
//    while(1){
//        string filename;
//        cout<<"Beheritto > "<<flush;
//        getline(cin,filename);
//        try{
//            launchIntepreter(filename);
//        }
//        catch(BeherittoError &err){
//            cout<<err.what()<<endl;
//        }
//        //sleep(20);
//    }
//    return 0;
//}
//
////void errorHandler(string s){
////    throw LexicalError(s.c_str(),1,1);
////}
//
//void sigHandler(int sig){
//    cout<<endl<<"God bless u, martyr of dream!"<<endl;
//    exit(-1);
//}
//
//void launchIntepreter(const string &filename){
////    /Users/soaringhawk/Documents/code/c++/beheritto/test/main.be
//    ifstream fin;
//    fin.open(filename);
//    if(!fin){
//        fin.clear();
//        cout<<"No such file!"<<endl;
//        return;
//    }
//    fin.close();
//    Intepreter intepreter(filename);
//    intepreter.process();
//}
