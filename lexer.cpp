#include "lexer.h"

Lexer::Lexer(const string &path){
    fin.open(path.c_str());
    ch=EOL;
    len=0;
    row=-1;
    col=0;
    state=0;
    int indentlevel=0;
}

char Lexer::nextChar(){
    if(ch==EOL){
        while(col==len){
            if(getline(fin,line)){
                len=line.size();
                row=row+1;
                col=0;
                if(col==len) return EOL;
            }
            else return EOF;
        }
        return line[col++];
    }
    else if(col==len)
        return EOL;
    else
        return line[col++];
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

        case 0:{
            if(ch==' '){//是占位符
                state=0;
                ch=nextChar();
            }
            else if(ch=='_'||isalpha(ch)){//是标识符
                state=1;
                lexeme.append(ch);
                ch=nextChar();
            }
            else if(isdigit(ch)){//是数值
                state=2;
                lexeme.append(ch);
                ch=nextChar();
            }
            else if(ch=='\"'){//是字符串
                state=5;
                ch=nextChar();
            }
            else if(ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='%'){
                state=-1;
                lexeme.append(ch);
                ch=nextChar();
            }
            else if(ch=='='){
                state=8;
                lexeme.append(ch);
                ch=nextChar();
            }
            else if(ch=='>'){
                state=9;
                lexeme.append(ch);
                ch=nextChar();
            }
            else if(ch=='<'){
                state=10;
                lexeme.append(ch);
                ch=nextChar();
            }
            else if(ch=='!'){
                state=11;
                lexeme.append(ch);
                ch=nextChar();
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
            else if(ch==EOL){
                state=12;
                ch=nextChar();
            }
        }
        break;

        case 1:{//接受态：标志符
            if(isalnum(ch)||ch=='_'){
                state=1;
                lexeme.append(ch);
                ch=nextChar();
            }
            else if(TokenMap.find(lexeme)!=TokenMap.end())
                state=-1;
            else{//是关键字
                state=0;
                return Token(laxeme,TokenType::VAR,row,col);
            }
            break;
        }

        case 2:{//接受态：整数
            if(isdigit(ch)){//是数值
                state=2;
                lexeme.append(ch);
                ch=nextChar();
            }
            else if(ch=='.'){//有小数点
                state=3;
                lexeme.append(ch);
                ch=nextChar();
            }
            else{//整数
                state=0;
                return Token(lexeme,TokenType::INT,row,col);
            }
        }
        break;

        case 3:{
            if(isdigit(ch)){//是小数
                state=4;
                lexeme.append(ch);
                ch=nextChar();
            }
            else{//小数点后没有数字，错误
                throw LexicalError(lexeme,ch,row,col);
            }
        }
        break;

        case 4:{//接受态：小数
            if(isdigit(ch)){//小数的位数不止一位
                state=4;
                lexeme.append(ch);
                ch=nextChar();
            }
            else{
                state=0;
                return Token(lexeme,TokenType::FLOAT,row,col);
            }
        }
        break;

        case 5:{
            if(ch=='\"'){//是空字符串
                state=0;
                ch=nextChar();
                return Token("",TokenType::STRING,row,col);
            }
            else if(ch==EOL||ch==EOF){//不完整字符串
                throw LexicalError(lexeme,ch,row,col);
            }
            else{
                state=6;
                lexeme.append(ch);
                ch=nextChar();
            }
        }
        break;

        case 6:{
            if(ch=='\"'){//完整字符串
                state=7;
                ch=nextChar();
            }
            else if(ch==EOL||ch==EOF){//字符串残缺
                throw LexicalError(lexeme,ch,row,col);
            }
            else{//接着读入字符串字符
                state=6;
                lexeme.append(ch);
                ch=nextChar();
            }
        }
        break;

        case 7:{//接受态：字符串
            state=0;
            return Token(lexeme,TokenType::STRING,row,col);
        }
        break;

        case 8:{//接受态：=
            if(ch=='='){
                state=-1;
                lexeme.append(ch);
                ch=nextChar();
            }
            else state=-1;
        }
        break;

        case 9:{//接受态：>
            if(ch=='>'||ch=='='){
                state=-1;
                lexeme.append(ch);
                ch=nextChar();
            }
            else state=-1;
        }
        break;

        case 10:{//接受态：<
            if(ch=='<'||ch=='='){
                state=-1;
                lexeme.append(ch);
                ch=nextChar();
            }
            else state=-1;
        }
        break;

        case 11:{
            if(ch=='='){
                state=-1;
                lexeme.append(ch);
                ch=nextChar();
            }
            else
                throw LexicalError(lexeme,ch,row,col);
        }
        break;

        case 12:{//接受态：缩进
            if(ch==' '){
                lexeme.append(ch);
                while(ch==' '){
                    lexeme.append(ch);
                    ch=nextChar();
                }
                state=0;
                if(lexeme.length()%4){
                    lineindent.push_back(lexeme.length()/4);
                    return Token(lexeme,TokenType::INDENT,row,col);
                }
                else
                    throw LexicalError("EOL",ch,row,1);
            }
            else if(ch==EOL){
                state=12;
                lineindent.push_back(0);
                return Token("",TokenType::INDENT,row,-1);
            }
            else{
                state=0;
                lineindent.push_back(0);
                return Token("",TokenType::INDENT,row,-1);
            }
        }
        break;
    }
}

