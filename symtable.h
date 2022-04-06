#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;

//Modelo de Simbolos
struct Symbol {
  string var;
  string type;
};

//Tabela de simbolos
class SymTable {
  private:
    unordered_map<string, Symbol> table;
    SymTable * prev;

  public:
    SymTable();
    SymTable(SymTable * t);

    bool Insert (string s, Symbol symb);
    Symbol * Find(string s);
};