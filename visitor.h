#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

struct FieldInfo {
    string name;      // field name
    string typeName;  // "int", "bool", "string", or struct name
    Type   type;      // basic kind (INT, BOOL, STRING, UNDEFINED)
};


class BinaryExp;
class NumberExp;
class Program;
class PrintStm;
class WhileStm;
class IfStm;
class AssignStm;
class Body;
class Vardec;
class FcallExp;
class ReturnStm;
class FunDec;
<<<<<<< Updated upstream

=======
class ForStm;
class ShortAssignStm;
class IncStm;
class DecStm;
class StringExp;
class StructStm;
class FieldAccessExp;

static Environment<int> env;
static Environment<Type> typeEnv;
static Environment<string> typeNameEnv;   // Use this to track declared type names of variables
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes

class Visitor {
public:
    // 
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(IdExp* exp) = 0;
    virtual int visit(Program* p) = 0;
    virtual int visit(PrintStm* stm) = 0;
    virtual int visit(WhileStm* stm) = 0;
    virtual int visit(IfStm* stm) = 0;
    virtual int visit(AssignStm* stm) = 0;
    virtual int visit(Body* body) = 0;
    virtual int visit(VarDec* vd) = 0;
    virtual int visit(FcallExp* fcall) = 0;
    virtual int visit(ReturnStm* r) = 0;
    virtual int visit(FunDec* fd) = 0;
<<<<<<< Updated upstream
};

=======
    virtual int visit(ForStm* fs) = 0;
    virtual int visit(ShortAssignStm* stm) = 0;
    virtual int visit(IncStm* stm) = 0;
    virtual int visit(DecStm* stm) = 0;
    virtual int visit(StringExp* exp) = 0;
    virtual int visit(FieldAccessExp* exp); // Bruh what is THIS.
    virtual int visit(StructStm* stm) = 0; // ...Is a struct a statement or an expression.
};

class TypeCheckerVisitor : public Visitor {
public:
    unordered_map<string,int> fun_locales;
    unordered_map<string, string> stringIds;
    unordered_map<string, vector<FieldInfo>> structDefs; // NEW

    int stringCont = 0;
    int locales;

    int type(Program* program);
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(IdExp* exp) override;
    int visit(Program* p) override ;
    int visit(PrintStm* stm) override;
    int visit(AssignStm* stm) override;
    int visit(ForWhileStm* stm) override;
    int visit(IfStm* stm) override;
    int visit(Body* body) override;
    int visit(VarDec* vd) override;
    int visit(FcallExp* fcall) override;
    int visit(ReturnStm* r) override;
    int visit(FunDec* fd) override;
    int visit(ForStm* fs) override;
    int visit(ShortAssignStm* stm) override;
    int visit(IncStm* stm) override;
    int visit(DecStm* stm) override;
    int visit(StringExp* exp) override;

    int visit(StructStm* stm) override; // I have no damned idea I just are hoping for the best.
    int visit(FieldAccessExp *exp) override; // This is to have access.

};
>>>>>>> Stashed changes

class GenCodeVisitor : public Visitor {
private:
    std::ostream& out;
public:
    GenCodeVisitor(std::ostream& out) : out(out) {}
    int generar(Program* program);
    unordered_map<string, int> memoria;
    unordered_map<string, bool> memoriaGlobal;
<<<<<<< Updated upstream
=======
    unordered_map<string,int> fun_reserva;
    TypeCheckerVisitor typeChecker;

<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
    int offset = -8;
    int labelcont = 0;

    bool entornoFuncion = false;
    string nombreFuncion;
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(IdExp* exp) override;
    int visit(Program* p) override ;
    int visit(PrintStm* stm) override;
    int visit(AssignStm* stm) override;
    int visit(WhileStm* stm) override;
    int visit(IfStm* stm) override;
    int visit(Body* body) override;
    int visit(VarDec* vd) override;
    int visit(FcallExp* fcall) override;
    int visit(ReturnStm* r) override;
    int visit(FunDec* fd) override;
<<<<<<< Updated upstream
=======
    int visit(ForStm* fs) override;
    int visit(ShortAssignStm* stm) override;
    int visit(IncStm* stm) override;
    int visit(DecStm* stm) override;
    int visit(StringExp* exp) override;

    int visit(StructStm* stm) override; // I am simply a believer.
    int visit(FieldAccessExp* exp) override; // This is for field access.
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
};

#endif // VISITOR_H