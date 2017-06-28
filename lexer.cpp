#include "lexer.h"

Lexer::Lexer(const string &filename){
    fin.open(filename.c_str());
    modname=filename;
    auto pos=modname.find(".");
    modname.erase(pos);
    if(!fin) throw LoadingError(modname);
    ch=EOL;
    len=0;
    row=0;
    col=0;
    state=0;
    indentlist.push(0);
}
bool Lexer::nextLine(){
    while(getline(fin,line)){
        col=col+1;
        if(!line.empty()){
            len=line.length();
            row=0;
            state=0;
            return true;
        }
    }
    return false;
}

char Lexer::nextChar(){
    if(row=len) return EOL;
    else return line[row++];
}

Token Lexer::nextToken(){
    string lexeme="";
    while(true){
        switch(state){
        //公用接受态：
        // + - * ／ %
        // == >= <= != >> <<
        // : , (  ) [  ]
        case -1:{
                state=0;
                return Token(lexeme,TokenMap[lexeme],row,col);
            }
            break;

        case 0:{//行初始态
                while(ch==' '){
                    lexeme.append(ch);
                    ch=nextChar();
                }
                if(lexeme.size()%4)
                    throw LexicalError(modname,"EOL",ch,row,col);
                if(lexeme.size()!=indentlist.back())
                    state=1;
                else
                    state=2;
            }
            break;

        case 1:{//当前相对于上一行的缩进情况
                if(lexeme.size()>indentlist.back()){
                    if(lexeme.size()==indentlist.back()+4){
                        indentlist.push(lexeme.size());
                        return Token("",TokenType::INDENT,row,col);
                    }
                    throw
                        LexicalError(modname,lexeme,ch,row,col);
                }
                else{
                    indentlist.pop();
                    if(lexeme.size()!=indentlist.back())
                        state=1;
                    else
                        state=2;
                    return Token("",TokenType::DEDENT,row,col);
                }
            }
            break;

        case 2:{//新标记态
                if(ch==' '){//是占位符
                    state=2;
                    ch=nextChar();
                }
                else if(isalpha(ch)){//是标识符
                    state=3;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else if(isdigit(ch)){//是数值
                    state=5;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else if(ch=='\"'){//是字符串
                    state=8;
                    ch=nextChar();
                }
                else if(ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='%'){
                    state=-1;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else if(ch=='='){
                    state=11;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else if(ch=='>'){
                    state=12;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else if(ch=='<'){
                    state=13;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else if(ch=='!'){
                    state=14;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else if(ch==EOL){
                    state=0;
                    ch=nextChar();
                    return Token(lexeme,Token::EOL,row,col);
                }
                else if(ch==':'||ch==','||ch=='('
                    ||ch==')'||ch=='['||ch==']'){
                    state=-1;
                    lexeme.append(ch);
                    ch=nextChar();
                    }
                else if(ch=='.'){
                    state=-1;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else
                    throw LexicalError(modname,lexeme,ch,row,col);
            }
            break;

        case 3:{//接受态：标志符
                if(isalnum(ch)){
                    state=3;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else if(ch=='.'){
                    state=4;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else if(TokenMap.find(lexeme)!=TokenMap.end())
                    state=-1;
                else{//是关键字
                    state=2;
                    return Token(laxeme,TokenType::ID,row,col);
                }
            }
            break;

        case 4:{//成员运算符
                if(isalpha(ch)){
                    state=3;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else
                    throw LexicalError(modname,lexeme,ch,row,col);
            }
            break;
        case 5:{//接受态：整数
                if(isdigit(ch)){//是数值
                    state=5;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else if(ch=='.'){//有小数点
                    state=6;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else{//整数
                    state=2;
                    return Token(lexeme,TokenType::INT,row,col);
                }
            }
            break;

        case 6:{
                if(isdigit(ch)){//是小数
                    state=7;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else//小数点后没有数字，错误
                    throw LexicalError(modename,lexeme,ch,row,col);
            }
            break;

        case 7:{//接受态：小数
                if(isdigit(ch)){//小数的位数不止一位
                    state=7;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else{
                    state=2;
                    return Token(lexeme,TokenType::FLOAT,row,col);
                }
            }
            break;

        case 8:{
                if(ch=='\"'){//是空字符串
                    state=2;
                    ch=nextChar();
                    return Token("",TokenType::STRING,row,col);
                }
                else if(ch==EOL)//不完整字符串
                    throw LexicalError(modname,lexeme,ch,row,col);
                else{
                    state=9;
                    lexeme.append(ch);
                    ch=nextChar();
                }
            }
            break;

        case :9{
                if(ch=='\"'){//完整字符串
                    state=10;
                    ch=nextChar();
                }
                else if(ch==EOL)//字符串残缺
                    throw LexicalError(modename,lexeme,ch,row,col);
                else{//接着读入字符串字符
                    state=9;
                    lexeme.append(ch);
                    ch=nextChar();
                }
            }
            break;

        case 10:{//接受态：字符串
                state=2;
                return Token(lexeme,TokenType::STRING,row,col);
            }
            break;

        case 11:{//接受态：=
                if(ch=='='){
                    state=-1;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else state=-1;
            }
            break;

        case 12:{//接受态：>
                if(ch=='>'||ch=='='){
                    state=-1;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else state=-1;
            }
            break;

        case 13:{//接受态：<
                if(ch=='<'||ch=='='){
                    state=-1;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else state=-1;
            }
            break;

        case 14:{
                if(ch=='='){
                    state=-1;
                    lexeme.append(ch);
                    ch=nextChar();
                }
                else
                    throw LexicalError(modname,lexeme,ch,row,col);
            }
            break;

        default:break;
    }
}

