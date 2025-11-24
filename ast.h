#ifndef AST_H
#define AST_H

#include <string>
#include <list>
#include <ostream>
#include <vector>
using namespace std;

class Visitor;
class VarDec;
class StructStm; // Forward Declaration.
class FieldAccessExp; // Forward Declaration.


// Operadores binarios soportados
enum BinaryOp { 
    PLUS_OP,
    MINUS_OP,
    MUL_OP,
    DIV_OP,
    POW_OP,
    LT_OP,
    LE_OP,
    GT_OP,
    GE_OP,
    EQ_OP
};

enum Type {
    UNDEFINED,
    INT,
    BOOL,
    STRING,
};

// Clase abstracta Exp
class Exp {
public:
    virtual int  accept(Visitor* visitor) = 0;
    Type type = UNDEFINED;
    virtual ~Exp() = 0;  // Destructor puro → clase abstracta
    static string binopToChar(BinaryOp op);  // Conversión operador → string
};

// Expresión binaria
class BinaryExp : public Exp {
public:
    Exp* left;
    Exp* right;
    BinaryOp op;
    int accept(Visitor* visitor);
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    ~BinaryExp();
};

// Expresión numérica
class NumberExp : public Exp {
public:
    int value;
    int accept(Visitor* visitor);
    NumberExp(int v);
    ~NumberExp();
};

// Expresión numérica
class IdExp : public Exp {
public:
    string value;
    int accept(Visitor* visitor);
    IdExp(string v);
    ~IdExp();
};

class Stm {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;
};

class VarDec {
public:
    string type;
    list<string> vars;
    VarDec();
    int accept(Visitor* visitor);
    ~VarDec();
};

class Body {
public:
    list<Stm*> StmList;
    list<VarDec*> declarations;
    int accept(Visitor* visitor);
    Body();
    ~Body();
};

class IfStm: public Stm {
public:
    Exp* condition;
    Body* then;
    Body* els;
    IfStm(Exp* condition, Body* then, Body* els);
    int accept(Visitor* visitor);
    ~IfStm(){};
};

class ForWhileStm: public Stm {
public:
    Exp* condition;
    Body* b;
    ForWhileStm(){};
    int accept(Visitor* visitor);
    ~ForWhileStm(){};
};

class AssignStm: public Stm {
public:
    string id;
    Exp* e;
    AssignStm(string, Exp*);
    ~AssignStm();
    int accept(Visitor* visitor);
};

class ShortAssignStm: public Stm {
public:
    string id;
    Exp* e;
    ShortAssignStm(string, Exp*);
    ~ShortAssignStm();
    int accept(Visitor* visitor);
};

class IncStm: public Stm {
public:
    string id;
    IncStm(string);
    ~IncStm() {};
    int accept(Visitor* visitor);
};

class DecStm: public Stm {
public:
    string id;
    DecStm(string);
    ~DecStm() {};
    int accept(Visitor* visitor);
};

class PrintStm: public Stm {
public:
    Exp* e;
    PrintStm(Exp*);
    ~PrintStm();
    int accept(Visitor* visitor);
};

class ReturnStm: public Stm {
public:
    Exp* e;
    ReturnStm(){};
    ~ReturnStm(){};
    int accept(Visitor* visitor);
};

class FcallExp: public Exp {
public:
    string nombre;
    vector<Exp*> argumentos;
    int accept(Visitor* visitor);
    FcallExp(){};
    ~FcallExp(){};
};

class StringExp : public Exp {
public:
    string value;
    int accept(Visitor* visitor);
    StringExp(string);
    ~StringExp() {};
};

class FunDec {
public:
    string nombre;
    string tipo;
    Body* cuerpo;
    vector<string> Ptipos;
    vector<string> Pnombres;
    int accept(Visitor* visitor);
    FunDec(){};
    ~FunDec(){};
};

class ForStm: public Stm {
public:
    string var;
    Stm* initial;
    Exp* condition;
    Stm* adder;
    Body* b;
    ForStm(){};
    ~ForStm(){};
    int accept(Visitor* visitor);
};
// ! = = = New addition = = = !

class FieldAccessExp : public Exp {
public:
    Exp* base;         // usually an IdExp (e.g., "s")
    string field;      // e.g., "age"
    int fieldIndex = -1; // filled by the type checker

    FieldAccessExp(Exp* base, const string& field);
    ~FieldAccessExp();
    int accept(Visitor* visitor);
};

class StructStm : public Stm {
public:
    string name;                  // struct name, e.g. "student_data"
    vector<string> fieldTypes;    // "int", "string", etc.
    vector<string> fieldNames;    // "age", "name", etc.
    vector<Exp*>   fieldInits;    // optional default values (may be nullptr)

    StructStm() {}
    ~StructStm() {}
    int accept(Visitor* visitor);
};

// ! = = = End of New Addition = = = !

class Program {
public:
    list<VarDec*>   vdlist;
    list<FunDec*>   fdlist;
    list<StructStm*> sdlist;   // List that contains all struct declarations.

    Program() {}
    ~Program() {}
    int accept(Visitor* visitor);
};


#endif // AST_H
