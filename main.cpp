#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fstream>

using namespace std;

void sigHandler(int sig);
void launchCompiler(string path);

int main(int argc, const char * argv[]){
    cout<<"ベヘリット Beheritto Version 1.0.0"<<endl;
    if(signal(SIGINT,sigHandler)==SIG_ERR){
        cout<<"Compiler error, crashed!"<<endl;
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
        launchCompiler(path);
        //command="cat "+line;
        //system(command.c_str());
    }
    return 0;
}

void sigHandler(int sig){
    cout<<endl<<"God bless u, martyr of dream!"<<endl;
    exit(-1);
}

void launchCompiler(string path){
    ifstream sourcefile(path.c_str());
    if(!sourcefile){
        cout<<"No such file!"<<endl;
        return;
    }
    string line;
    while(getline(sourcefile,line))
        cout<<line;
}
