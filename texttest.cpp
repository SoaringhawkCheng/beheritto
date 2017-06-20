#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main(int argc,const char *argv[]){
    ifstream fin("test.py");
    vector<string> result;
    string line;
    while(getline(fin,line))
        result.push_back(line);
    for(auto line:result){
        for(auto ch:line)
            cout<<int(ch)<<" ";
        cout<<endl;
    }
    cout<<"\\ t="<<int('\t')<<endl;
    return 0;
}
