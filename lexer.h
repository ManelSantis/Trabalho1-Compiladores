#include <unordered_map>
#include <string>
#include <sstream>
using std::stringstream;
using std::unordered_map;
using std::string;

//Cada token possui uma tag
enum Tag {TYPE = 256, NUM, ID};

struct Token {
  int tag;
  Token() : tag(0){}
  Token(int t) : tag(t) {}
  virtual string toString() { stringstream ss; ss << char(tag); return ss.str(); }
	virtual string typeString() { stringstream ss; ss << char(tag); return ss.str(); }
};

struct Num : public Token {
  int valueInt;
	float valueFloat;
	string type;
	Num(): Token(Tag::NUM), valueInt(0) {}
	Num(int v, string t) : Token(Tag::NUM), valueInt(v), type(t) {}
	Num(float v, string t) : Token(Tag::NUM), valueFloat(v), type(t) {}	
	virtual string toString() { 
		stringstream ss; 
			if (type == "int"){
				ss << valueInt; 
			} else {
				ss << valueFloat; 
			}
		return ss.str(); 
		}
	virtual string typeString() { return type; }
};

struct Id : public Token {
  string name;
  Id() : Token(Tag::ID) {}
  Id(int t, string s) : Token(t), name(s) {}
  virtual string toString() { return name; }
};

//Analisador Léxico
class Lexer {
  private:
    //Tipos de Token-
    struct {
      Token t;
      Num n;
      Id i;
    } token;
    //Variáveis auxiliares
    int line = 1;
    char peek;
    unordered_map<string, Id> id_table;

  public:
    Lexer();
    int Lineno();
    Token * Scan();
    void Comentarios();
};
