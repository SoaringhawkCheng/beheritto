#ifndef path_h
#define path_h

#include <iostream>
#include <vector>
using namespace std;

vector <string> PATH;

void addpath(const string &path){
    PATH.push_back(path);
}
#endif
