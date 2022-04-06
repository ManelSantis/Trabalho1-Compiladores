#include "lexer.h"
#include <fstream>
#include <iostream>
#include <sstream>
using std::stringstream;
extern std::ifstream fin;

//COnstrutor Lexer
Lexer::Lexer() {
  //Palavras reservadas
  id_table["math"] = Id{Tag::TYPE, "math"};
  id_table["int"] = Id{Tag::TYPE, "int"};
  id_table["char"] = Id{Tag::TYPE, "char"};
  id_table["bool"] = Id{Tag::TYPE, "bool"};
  id_table["float"] = Id{Tag::TYPE, "float"};
  //Iniciando a leitura do arquivo
  peek = fin.get();
}

//Retornar número da linha atual
int Lexer::Lineno() {
  return line;
}

void Lexer::Comentarios() {
    //Primeiro verifica se está começando com /
  if (peek == '/') {
    peek = fin.get();
    //Agora o peek pega o proximo caracter do arquivo
    //Se ele for outra barra então
      if (peek == '/') {
        //Irá continuar a ler até encotnrar um \n que indica o final do comentário
        //utilizando a finalComent para saber se chegou ou não ao final do comentario
        bool finalComent = false;
        while (!finalComent){
          if (peek == '\n') {
            line += 1;
            finalComent = true;
          }
          peek = fin.get();
        }

        while (isspace(peek)){
          peek = fin.get();
        }
        //verifica se está lendo algum espaço ao final do comentario
      } else if (peek == '*') { 
        //se for um * então irá ler como se estivesse lendo o arquivo inteiro
        //até chegar ao final do comentario com outro */ que será indicado pela variavel finalComnent
        bool finalComent = false;
          while (!finalComent){
            peek = fin.get();
            //Se no meio do comentario tiver uma quebra de linha é para continuar a ler
            while (isspace(peek)){
              if (peek == '\n') {
                line += 1;
              }
              peek = fin.get();
            }

            if (peek == '*'){
              peek = fin.get();
                //pode ocorrer de ainda dentro do comentario existir * e / que n sejam do final então continua a ler
                if(peek == '/') {
                  peek = fin.get();
                  finalComent = true;
                    while (isspace(peek)){
                        if (peek == '\n') {
                          line += 1;
                        }
                      peek = fin.get();
                    }
              }
            }
          }
      } else {
        fin.unget();
        fin.unget();
        peek = fin.get();
      }
  }
  //recursividade para verificar se é um comentario seguido de outro comentario
  if (peek == '/') {
    Comentarios();
  }
}


//Envia os tokens para o analisador sintático
Token * Lexer::Scan(){
  //Ignorar espaços em branco, tabulações e novas linhas
  while (isspace(peek)) {
    //Se achar uma nova linha, acrescenta a quantidade
    if (peek == '\n'){
      line += 1;
    }
    peek = fin.get();
  }

  Comentarios();

  //Retorna números
  if (isdigit(peek)) {
    int v = 0;
    float complet = 0.0;
    
    do {
      //Converter o caractere para o digito númerico
      int n = peek - '0';
      v = 10 * v + n;
      peek = fin.get();
    } while (isdigit(peek));

    //verificar se o numero é inteiro ou float
    if (peek == '.') {
      peek = fin.get();
      int contUni = 0;
      do {
      //Converter o caractere para o digito númerico
      contUni++;
      int n = peek - '0';
      complet = (10.0 * complet + n);
      peek = fin.get();
      } while (isdigit(peek));

      for (int i = 1; i <= contUni; i++){
        complet /= 10;
      }
      complet +=v;
    }

    //Retona o token feito
    if (complet == 0.0) {
      token.n = Num{v, "int"};    
    } else {
      token.n = Num{complet, "float"};    
    }
    
    return &token.n;
  }
  //Retorna palavras-chaves e identificadores
  if (isalpha(peek)) {
    stringstream ss;
    do {
      ss << peek;
      peek = fin.get();
    } while (isalpha(peek));

    string s = ss.str();
    auto pos = id_table.find(s);

    //Se a lexana já estiver na tabela
    if (pos != id_table.end()) {
      //Retorna o token relacionado a ela
      token.i = pos->second;
      return &token.i;
    }
  
    Id new_id {Tag::ID, s};
    id_table[s] = new_id;

    //Retorna o Token encontrado
    token.i = new_id;
    return &token.i;
  }
  //Operadores e caracteres não alphanuméricos isolados

  Token op {peek};
	peek = ' ';
  
	//Retorna o token
	token.t = op;
	return &token.t;

}