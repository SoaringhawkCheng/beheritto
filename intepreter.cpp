#include "intepreter.h"

Intepreter::Intepreter(const string &path){
    lexer=Lexer(path);
    token=Token();
}
