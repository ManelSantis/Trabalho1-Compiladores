#include "error.h"
#include <iostream>
using std::cout;
using std::endl;

//Construtor da nova mensagem de error
SyntaxError::SyntaxError(int line, string msg) : lineno(line), desc(msg) {}

//Exibir a mensagem de erro
void SyntaxError::What() {
  cout << "\n Erro (linha " << lineno << "): " << desc << endl;  
}