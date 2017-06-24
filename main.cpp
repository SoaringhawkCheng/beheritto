#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fstream>

#include "path.h"
#include "error.h"
#include "lexer.h"

using namespace std;

void errorHandler(string s);
void sigHandler(int sig);
void launchInterpreter(string path);

int main(int argc, const char * argv[]){
    cout<<"ベヘリット Beheritto Version 1.0.0"<<endl;
    if(signal(SIGINT,sigHandler)==SIG_ERR){
        cout<<"Interpreter error, crashed!"<<endl;
        exit(-1);
    }
    cout<<"Press Ctrl+C to exit"<<endl;
    while(1){
        string path;
        cout<<"Beheritto > "<<flush;
        getline(cin,path);

        //if(line=="Q") break;
        //string command;
        //if(command[0]!='.'&&command[0]!='/'&&command[0]!='~')
        //    command="./"+command;
        //launchCompiler(path);
        //command="cat "+line;
        //system(command.c_str());
        try{
            errorHandler("haha!");
        }
        catch(CustomException &err){
            cout<<err.what()<<endl;
        }
        sleep(20);
    }
    return 0;
}

void errorHandler(string s){
    throw LexicalError(s.c_str(),1,1);
}

void sigHandler(int sig){
    cout<<endl<<"God bless u, martyr of dream!"<<endl;
    exit(-1);
}

void launchInterpreter(string path){
    ifstream sourcefile(path.c_str());
    if(!sourcefile){
        cout<<"No such file!"<<endl;
        return;
    }
    string line;
    while(getline(sourcefile,line))
        cout<<line;
}
