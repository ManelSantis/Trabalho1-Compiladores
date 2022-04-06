#include "symtable.h"

//Construtor para a primeira tabela
SymTable::SymTable() : prev(nullptr) {}

//Construtor para novas tabelas
SymTable::SymTable(SymTable * t) : prev(t) {}

//Insere um simbolo na tabela
bool SymTable::Insert(string s, Symbol symb){
  const auto& [pos, success] = table.insert({s, symb});
  return success;
}

//Buca um simbolo na tabela atual
//Se não encontrado, busca nas tabelas dos escopos envolventes
Symbol * SymTable::Find(string s){
  for (SymTable * st = this; st != nullptr; st =  st->prev) {
    auto found = st->table.find(s);
    if (found != st->table.end()){
      return &found->second;
    }
  }
  return nullptr;
}