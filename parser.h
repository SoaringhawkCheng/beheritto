#ifndef parser_h
#define parser_h

class Parser{
public:
    Parser(Lexer *,ASTree *tree);
    ~Parser();
    process();
private:
    void rootParser();
    void importParser();
    void fromParser();
    void moduleParser();
    void modClassParser();
    void modMethodParser();
    void classParser();
    void methodParser();
    void entryParser();

    vector<Lexer *> lexerlist;
    Lexer lexer;
};
#endif
