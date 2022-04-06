#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "parser.h"
#include "error.h"
using namespace std;
//Arquivo de entrada
ifstream fin;
//Cabeçalhos dos métodos
void helpText();
void errorText();
//Programa pode receber nomes de arquivos
int main(int argc, char **argv) {
  if (argc == 2) {
    //Variável auxiliar para o arquivo
    string aux {argv[1]}; 
    //Verificar se está digitando algum -- helpe
    if (argv[1][0] == '-') {
      if(!aux.compare("--help")) {
        helpText();
        return 0;
      } else {
        errorText();
        return 0;
      }
    }
    //Vetor de caracteres com o que está escrito nos arquivos
    vector<char> charAux(aux.begin(), aux.end());
    //Variavel para verificar se está terminando em ".math"
    string auxiliar;
    //contar para que a variavel auxiliar possa ter exatamente ".math"
    //(cinco caracteres) no final
    int cont = 0; 
    //Verificar se termina com .math
    bool math = false; 
    //Esse for irá começar a ler do final do nome do arquivo 
    //E irá ler até cont ficar contando como cinco caracteres
    for (int i = aux.length() - 1; i >= 0 && cont != 5; i--) {
        auxiliar = charAux[i] + auxiliar;
        cont++;
    }
    //Ao terminar o for, se auxilair for igual a ".math"
    //Então será permitida a leitura do arquivo
    if (!auxiliar.compare(".math")) {
      math = true;
    }
    //Leitura do arquivo
    if (math) {
      fin.open(argv[1]);
      if (!fin.is_open()){
        cout << "Falha na abertura do arquivo \'" << argv[1] << "\'.\n";
        cout << "Verifique se o nome está correto, ou se é protegido para leitura,\n";
        exit(EXIT_FAILURE);
      }
      //Inicio da leitura do arquivo
      Parser tradutor;
      try {
        tradutor.Start();
      } catch (SyntaxError err){
        err.What();
      }
      cout << endl;
      fin.close();
      } else { 
      //Caso não termine em "./math"
      cout << "\n Falha na abertura do arquivo \'" << argv[1] << "\'.\n";
      cout << "Verifique se o arquivo termina com \".math\".\n";
      exit(EXIT_FAILURE);
    }
    return 0;
  }
}
//Ao digitar --help
void helpText() {
    cout << "\n Escreva a linha de comando da seguinte forma para que funcione: \n";
    cout << "\n ./trabalho <caminho do arquivo> \n";
    cout << "\n Observação: \n";
    cout << "\n Garanta que seu arquivo está terminando com a estenção \".math\".\n";
}
//Ao digitar um comando errado
void errorText() {
    cout << "\n Comando não encontrado para ./trabalho \n";
    cout << "\n Use --help para saber como usar. \n";
}