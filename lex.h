#include <iostream>
#include <vector>
#include "fa.h"
using namespace std;

class Lexer{
    public:
        Lexer(const string &path);
        NFA connectNFA(const NFA &lnfa,const NFA &rnfa);
        NFA closure(NFA nfa);
        NFA branch(vector<NFA> nfalist);
        NFA addNewNFA(vector<NFA> nfalist);
        DFA convertNFA2DFA(NFA nfa);
        DFA OptimizeDFA(DFA dfa);
};
