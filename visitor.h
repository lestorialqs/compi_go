#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"
#include <list>
#include <vector>
#include <unordered_map>
#include <string>

#include "environment.h"
using namespace std;

class BinaryExp;
class NumberExp;
class Program;
class PrintStm;
class ForWhileStm;
class IfStm;
class AssignStm;
class Body;
class Vardec;
class FcallExp;
class ReturnStm;
class FunDec;
class ForStm;
class ShortAssignStm;
class IncStm;
class DecStm;
class StringExp;
class FieldAccessExp;
class StructDec;
class FieldAssignStm;
class TernaryExp;

class SimpleVarDec;
class ArrayDec;
class AssignArrayStm;
class ArrayAccessExp;
class ArrayLiteralExp;
enum Type;

struct ArrayInfo {
    vector<int> dimensions;  // dimensiones del array
    int totalSize;           // producto total
    int baseOffset;          // offset base en stack
    Type baseType; // <--- AGREGA ESTO: ¿De qué tipo son los elementos?
};
static Environment<int> env;
static Environment<Type> typeEnv;
static Environment<ArrayInfo> arrayEnv;
static Environment<string> structTypeEnv;

struct FieldInfo {
    string name;      // field name
    Type   type;      // basic kind (INT, BOOL, STRING, UNDEFINED)
    int paramOffset;  // param offset (From base address)
};

class Visitor {
public:
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(IdExp* exp) = 0;
    virtual int visit(Program* p) = 0;
    virtual int visit(PrintStm* stm) = 0;
    virtual int visit(ForWhileStm* stm) = 0;
    virtual int visit(IfStm* stm) = 0;
    virtual int visit(AssignStm* stm) = 0;
    virtual int visit(Body* body) = 0;
    virtual int visit(FcallExp* fcall) = 0;
    virtual int visit(ReturnStm* r) = 0;
    virtual int visit(FunDec* fd) = 0;
    virtual int visit(ForStm* fs) = 0;
    virtual int visit(ShortAssignStm* stm) = 0;
    virtual int visit(IncStm* stm) = 0;
    virtual int visit(DecStm* stm) = 0;
    virtual int visit(StringExp* exp) = 0;
    virtual int visit(SimpleVarDec* vd) = 0;
    virtual int visit(ArrayDec* vd) = 0;
    virtual int visit(AssignArrayStm* stm) = 0;
    virtual int visit(ArrayAccessExp* exp) = 0;
    virtual int visit(ArrayLiteralExp* exp) = 0;
    virtual int visit(FieldAccessExp* exp) = 0;
    virtual int visit(StructDec* stm) = 0;
    virtual int visit(FieldAssignStm* stm) = 0;
    virtual int visit(TernaryExp* exp) = 0;
};

class TypeCheckerVisitor : public Visitor {
public:
<<<<<<< HEAD
    unordered_map<string, Type> funcReturnTypes; // <--- AGREGA ESTO
    
    unordered_map<string,int> fun_locales;
=======
    unordered_map<string, int> fun_locales;
>>>>>>> e5f8a274c12e5c339e01465e03c1817982d9d6bc
    unordered_map<string, string> stringIds;
    unordered_map<string, vector<FieldInfo>> structDefs; // Handles struct definitions based on names.
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
    int visit(FcallExp* fcall) override;
    int visit(ReturnStm* r) override;
    int visit(FunDec* fd) override;
    int visit(ForStm* fs) override;
    int visit(ShortAssignStm* stm) override;
    int visit(IncStm* stm) override;
    int visit(DecStm* stm) override;
    int visit(StringExp* exp) override;
    int visit(SimpleVarDec* vd) override;
    int visit(ArrayDec* vd) override;
    int visit(AssignArrayStm* stm) override;
    int visit(ArrayAccessExp* exp) override;
    int visit(ArrayLiteralExp* exp) override;
    int visit(StructDec* stm) override;
    int visit(FieldAccessExp* exp) override;
    int visit(FieldAssignStm* stm) override;
    int visit(TernaryExp* exp) override;
};


class GenCodeVisitor : public Visitor {
private:
    std::ostream& out;
public:
    GenCodeVisitor(std::ostream& out) : out(out) {}
    int generar(Program* program);
    unordered_map<string, int> memoriaGlobal;  // was <string, bool>, it's int now to keep numbers
    unordered_map<string,int> fun_reserva;
    TypeCheckerVisitor typeChecker;
    int offset = -8;
    int labelcont = 0;
    bool entornoFuncion = false;
    string nombreFuncion;

    string currentArray; // nombre del array inicializando
    int currentArrayIndex;
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(IdExp* exp) override;
    int visit(Program* p) override ;
    int visit(PrintStm* stm) override;
    int visit(AssignStm* stm) override;
    int visit(ForWhileStm* stm) override;
    int visit(IfStm* stm) override;
    int visit(Body* body) override;
    int visit(FcallExp* fcall) override;
    int visit(ReturnStm* r) override;
    int visit(FunDec* fd) override;
    int visit(ForStm* fs) override;
    int visit(ShortAssignStm* stm) override;
    int visit(IncStm* stm) override;
    int visit(DecStm* stm) override;
    int visit(StringExp* exp) override;
    int visit(SimpleVarDec* vd) override;
    int visit(ArrayDec* vd) override;
    int visit(AssignArrayStm* stm) override;
    int visit(ArrayAccessExp* exp) override;
    int visit(ArrayLiteralExp* exp) override;
    int visit(StructDec* stm) override;       // (no code)
    int visit(FieldAccessExp* exp) override;  // Field Access handles contact with the variables in a Struct.
    int visit(FieldAssignStm* stm) override;
    int visit(TernaryExp* exp) override;
};

#endif // VISITOR_H