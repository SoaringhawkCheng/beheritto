
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>

#include "error.h"
#include "lexer.h"
#include "parser.h"
#include "astree.h"
#include "intepreter.h"

using namespace std;

unordered_map<string,TokenType> TokenMap;
RuntimeStack runtimestack;
SymbolTable symboltable;
int state=-1;
DeclModule *program=NULL;
DeclModule *curmodule=NULL;
DeclClass *curclass=NULL;
DeclMethod *curmethod=NULL;
string curmodname="";
int curline=-1;

int main(int argc, const char * argv[]){
    string pathname="/Users/soaringhawk/Documents/code/c++/beheritto/testcase/unittest";
    for(int i=0;i<1;i++){
        string filename;
        stringstream ss;
        ss<<pathname<<i<<".be";
        ss>>filename;
        Intepreter intepreter(filename);
        intepreter.process();
    }
    return 0;
}
