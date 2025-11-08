#include "ast.h"
#include <iostream>

using namespace std;

// ------------------ Exp ------------------
Exp::~Exp() {}

string Exp::binopToChar(BinaryOp op) {
    switch (op) {
        case PLUS_OP:  return "+";
        case MINUS_OP: return "-";
        case MUL_OP:   return "*";
        case DIV_OP:   return "/";
        case POW_OP:   return "**";
        case LE_OP:   return "<";
        default:       return "?";
    }
}

// ------------------ BinaryExp ------------------
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp o)
    : left(l), right(r), op(o) {}

    
BinaryExp::~BinaryExp() {
    delete left;
    delete right;
}



// ------------------ NumberExp ------------------
NumberExp::NumberExp(int v) : value(v) {}

NumberExp::~NumberExp() {}


// ------------------idExp ------------------
IdExp::IdExp(string v) : value(v) {}

IdExp::~IdExp() {}


Stm::~Stm(){}

PrintStm::~PrintStm(){}

AssignStm::~AssignStm(){}

IfStm::IfStm(Exp* c, Body* t, Body* e): condition(c), then(t), els(e) {}

WhileStm::WhileStm(Exp* c, Body* t): condition(c), b(t) {}


PrintStm::PrintStm(Exp* expresion){
    e=expresion;
}

AssignStm::AssignStm(string variable,Exp* expresion){
    id = variable;
    e = expresion;
}



VarDec::VarDec() {}

VarDec::~VarDec() {}

Body::Body(){
    declarations=list<VarDec*>();
    StmList=list<Stm*>();
}

Body::~Body(){}


