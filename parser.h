#include "lexer.h"
#include "symtable.h"

class Parser {
  private:
    Lexer scanner;
    SymTable * symtable;
    Token * lookahead;

    void Program();
    void Block();
    void Decls();
    void Stmts();
    void Expr();
    void Exprs();
    void Term();
    void Terms();
    void Factor();
    bool Match(int tag);

  public:
    Parser();
    void Start();
};