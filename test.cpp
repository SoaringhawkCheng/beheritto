#include <iostream>
#include <unordered_map>
using namespace std;

unordered_map<string,int> mmap;
mmap.insert(pair<string,int>("+",1));
int main(){
    string str=string("aa");
    //unordered_map<string,string> mmap;
    //mmap["+"]=1;
    //cout<<mmap["+"]<<endl;
    //cout<<str<<endl;
    return 0;
}
