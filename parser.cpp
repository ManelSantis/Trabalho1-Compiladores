#include "parser.h"
#include "error.h"
#include <iostream>
#include <sstream>
#include <cctype>
using std::cin;
using std::cout;
using std::endl;
using std::stringstream;

void Parser::Program() {
  //program -> match block
  if (lookahead->toString().compare("math")) {
    throw SyntaxError (scanner.Lineno(), "\'math\' esperado");
  } else {
    Match(Tag::TYPE);
    Block();
  }
}

void Parser::Block() {
  //block -> { decls stmts }
  if (!Match('{')) {
    throw SyntaxError (scanner.Lineno(), "\'{\' esperado");
  }
  //Criar nova tabela para o bloco
  SymTable * saved = symtable;
  symtable = new SymTable(symtable);
  //-------------------------------
  Decls();
  Stmts();
  //lookahead = scanner.Scan();
  //cout << lookahead->tag << endl;
  if (!Match('}')){
    throw SyntaxError (scanner.Lineno(), "\'}\' esperado");
  }
  delete symtable;
  symtable = saved;
}

void Parser::Decls() {
  //decls -> decls decl | e
  //decl -> type id;
    while (lookahead->tag == Tag::TYPE) {
      
      string type {lookahead->toString()};
      Match(Tag::TYPE);

      string name {lookahead->toString()};
      Match(Tag::ID);

      Symbol s { name, type };

      //Insere variavel na tabela de simbolos
      if(!symtable->Insert(name, s)){
        //Inserção falha se o simbolo já existir na tabela
        stringstream ss;
        ss << "variavel \"" << name << "\" já definida";
        throw SyntaxError(scanner.Lineno(), ss.str());
      }
      //Tenta achar o final como um ;
      if (!Match(';')) {
        stringstream ss;
        ss << "encontrado \'" << lookahead->toString() << "\' no lugar de ';'";
        throw SyntaxError { scanner.Lineno(), "esperando ;" };
      }
    }
}

void Parser::Stmts() {
  //stmts -> stmts stmt
  while (true) {
    switch (lookahead->tag) {
      //stmt -> block
      case '{':
        Block();
        break;
      //stmt -> expr
      case Tag::ID:
      case Tag::NUM:
      case '(':
        Expr();
        if (!Match(';')) {
        stringstream ss;
        ss << "encontrado \'" << lookahead->toString() << "\' no lugar de ';'";
        throw SyntaxError { scanner.Lineno(), "esperando ;" };
       }
       cout << endl;
       break;
      //stmts -> e
      default: return;
    }
  }
}

void Parser::Expr() {
      //expr -> term exprs
      Term();
      Exprs();
}

void Parser::Exprs() { 
  //exprs -> + term exprs
  //       | - term exprs
  //       | e
  while(true) {
    if (lookahead-> tag == '+') {
        Match('+');
        Term();
        cout << '+';
    } else if (lookahead-> tag == '-') {
        Match('-');
        Term();
        cout << '-';
    } else { return; }
  }
}

void Parser::Term() {
  //term -> factor terms 
  Factor();
  Terms();
}

void Parser::Terms() {
  //terms -> * factor terms
  //       | / factor terms
  //       | e
   while (true) {
    if (lookahead-> tag == '*') {
        Match('*');
        Factor();
        cout << '*';
    } else if (lookahead-> tag == '/') {
        Match('/');
        Factor();
        cout << '/';
    } else { return; }
  }
}

void Parser::Factor() {
  //factor -> (expr)
  if (lookahead->tag == '(') {
    Match('(');
    Expr();
    if (!Match(')')){
      throw SyntaxError{ scanner.Lineno(), "\')\' esperado"};
    }
  } else if (lookahead->tag == Tag::NUM){ //factor -> num
      cout << '(' << lookahead->toString() << ')';
      Match(Tag::NUM);
  } else if (lookahead->tag == Tag::ID) { //factor -> id
      Symbol * s = symtable->Find(lookahead->toString());
        if (!s) {
            stringstream ss;
            ss << "variável \"" << lookahead->toString() << "\" não declarada";
            throw SyntaxError{ scanner.Lineno(), ss.str() };
        }
      cout << '(' << s->var << ":" << s->type << ')';
      Match(Tag::ID);
  } else { 
      //erro de sintaxe
      stringstream ss;
      ss << "símbolo \'" << lookahead->toString() << "\' inválido";  
      throw SyntaxError{ scanner.Lineno(), ss.str() };
  }
}

bool Parser::Match(int tag) {
  //Verificar se o lookahead atual da match com a tag recebida
  if (tag == lookahead->tag) {
    //Se der match passa para o proximo lookahead e retorna true
    lookahead = scanner.Scan();
    return true;
  }
  return false;
}

Parser::Parser() {
  //Construtor Parser
  lookahead = scanner.Scan();
  symtable = nullptr;
}

void Parser::Start() {
  //Começo da leitura
  Program();
}