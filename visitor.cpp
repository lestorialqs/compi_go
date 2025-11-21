#include <iostream>
#include "ast.h"
#include "visitor.h"
#include <unordered_map>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////

int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Program::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IdExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int PrintStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int AssignStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IfStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int ForWhileStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Body::accept(Visitor* visitor){
    return visitor->visit(this);
}

int VarDec::accept(Visitor* visitor){
    return visitor->visit(this);
}

int FcallExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int FunDec::accept(Visitor* visitor){
    return visitor->visit(this);
}

int ReturnStm::accept(Visitor* visitor){
    return visitor->visit(this);
}

int ForStm::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int ShortAssignStm::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int IncStm::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int DecStm::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int StringExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

///////////////////////////////////////////////////////////////////////////////////

int GenCodeVisitor::generar(Program* program) {
    env.add_level();
    type.type(program);
    fun_reserva = type.fun_locales;
    stringIds = type.stringIds;
    program->accept(this);
        return 0;
}

int GenCodeVisitor::visit(Program* program) {
    out << ".data\nprint_fmt: .string \"%ld \\n\""<<endl;
    out << "string_fmt: .string \"%s \\n\""<<endl;

    for (auto& [id, str] : stringIds) {
        out << str << ": .string \""<< id << "\"" << endl;
    }

    for (auto dec : program->vdlist){
        dec->accept(this);
    }

    for (auto& [var, _] : memoriaGlobal) {
        out << var << ": .quad 0"<<endl;
    }

    out << ".text\n";
    
    for (auto dec : program->fdlist){
        dec->accept(this);
    }

    out << ".section .note.GNU-stack,\"\",@progbits"<<endl;
        return 0;
}

int GenCodeVisitor::visit(VarDec* stm) {
    for (auto var : stm->vars) {
        if (!entornoFuncion) {
            memoriaGlobal[var] = true;
        } else {
            env.add_var(var, offset);
            offset -= 8;
        }
    }
        return 0;
}

int GenCodeVisitor::visit(NumberExp* exp) {
    out << " movq $" << exp->value << ", %rax"<<endl;
    return 0;
}

int GenCodeVisitor::visit(IdExp* exp) {
    if (memoriaGlobal.count(exp->value))
        out << " movq " << exp->value << "(%rip), %rax"<<endl;
    else
        if (env.check(exp->value)) {
            out << " movq " << env.lookup(exp->value) << "(%rbp), %rax"<<endl;
        }
    return 0;
}

int GenCodeVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    out << " pushq %rax\n";
    exp->right->accept(this);
    out << " movq %rax, %rcx\n popq %rax\n";

    switch (exp->op) {
        case PLUS_OP:  out << " addq %rcx, %rax\n"; break;
        case MINUS_OP: out << " subq %rcx, %rax\n"; break;
        case MUL_OP:   out << " imulq %rcx, %rax\n"; break;
        case LT_OP:
            out << " cmpq %rcx, %rax\n"
              << " movl $0, %eax\n"
              << " setl %al\n"
              << " movzbq %al, %rax\n";
            break;
        case LE_OP:
            out << " cmpq %rcx, %rax\n"
              << " movl $0, %eax\n"
              << " setle %al\n"
              << " movzbq %al, %rax\n";
            break;
        case GT_OP:
            out << " cmpq %rcx, %rax\n"
              << " movl $0, %eax\n"
              << " setg %al\n"
              << " movzbq %al, %rax\n";
            break;
        case GE_OP:
            out << " cmpq %rcx, %rax\n"
              << " movl $0, %eax\n"
              << " setge %al\n"
              << " movzbq %al, %rax\n";
            break;
        case EQ_OP:
            out << " cmpq %rcx, %rax\n"
              << " movl $0, %eax\n"
              << " sete %al\n"
              << " movzbq %al, %rax\n";
            break;
    }
    return 0;
}

int GenCodeVisitor::visit(AssignStm* stm) {
    stm->e->accept(this);
    if (memoriaGlobal.count(stm->id))
        out << " movq %rax, " << stm->id << "(%rip)"<<endl;
    else
        if (env.check(stm->id)) {
            out << " movq %rax, " << env.lookup(stm->id) << "(%rbp)"<<endl;
        }
    return 0;
}

int GenCodeVisitor::visit(PrintStm* stm) {
    stm->e->accept(this);
    out <<
        " movq %rax, %rsi\n"
        " leaq print_fmt(%rip), %rdi\n"
        // "leaq string_fmt(%rip), %rdi\n"    TODO: ADD TYPES TO DETERMINE WHICH PRINT FORMAT TO USE
        " movl $0, %eax\n"
        " call printf@PLT\n";
    return 0;
}

int GenCodeVisitor::visit(Body* b) {
    env.add_level();
    for (auto dec : b->declarations){
        dec->accept(this);
    }
    for (auto s : b->StmList){
        s->accept(this);
    }
    env.remove_level();
    return 0;
}

int GenCodeVisitor::visit(IfStm* stm) {
    int label = labelcont++;
    stm->condition->accept(this);
    out << " cmpq $0, %rax"<<endl;
    out << " je else_" << label << endl;
    stm->then->accept(this);
    out << " jmp endif_" << label << endl;
    out << " else_" << label << ":"<< endl;
    if (stm->els) stm->els->accept(this);
    out << "endif_" << label << ":"<< endl;
    return 0;
}

int GenCodeVisitor::visit(ForWhileStm* stm) {
    int label = labelcont++;
    out << "while_" << label << ":"<<endl;
    stm->condition->accept(this);
    out << " cmpq $0, %rax" << endl;
    out << " je endwhile_" << label << endl;
    stm->b->accept(this);
    out << " jmp while_" << label << endl;
    out << "endwhile_" << label << ":"<< endl;
    return 0;
}


int GenCodeVisitor::visit(ReturnStm* stm) {
    stm->e->accept(this);
    out << " jmp .end_"<<nombreFuncion << endl;
    return 0;
}

int GenCodeVisitor::visit(FunDec* f) {
    entornoFuncion = true;
    env.clear();
    env.add_level();
    offset = -8;
    nombreFuncion = f->nombre;
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    out << ".globl " << f->nombre << endl;
    out << f->nombre <<  ":" << endl;
    out << " pushq %rbp" << endl;
    out << " movq %rsp, %rbp" << endl;
    out << " subq $" << fun_reserva[f->nombre]*8 << ", %rsp" << endl;
    int size = f->Pnombres.size();
    for (int i = 0; i < size; i++) {
        env.add_var(f->Pnombres[i], offset);
        out << " movq " << argRegs[i] << "," << offset << "(%rbp)" << endl;
        offset -= 8;
    }
    for (auto i: f->cuerpo->declarations){
        i->accept(this);
    }
    for (auto i: f->cuerpo->StmList){
        i->accept(this);
    }
    out << ".end_"<< f->nombre << ":"<< endl;
    out << "leave" << endl;
    out << "ret" << endl;
    entornoFuncion = false;
    return 0;
}

int GenCodeVisitor::visit(FcallExp* exp) {
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int size = exp->argumentos.size();
    for (int i = 0; i < size; i++) {
        exp->argumentos[i]->accept(this);
        out << " mov %rax, " << argRegs[i] <<endl;
    }
    out << "call " << exp->nombre << endl;
    return 0;
}

int GenCodeVisitor::visit(ForStm *fs) {
    int label = labelcont++;
    env.add_var(fs->var, offset);
    offset -= 8;
    fs->initial->accept(this);
    out << "for_" << label << ":"<<endl;
    fs->condition->accept(this);
    out << " cmpq $0, %rax" << endl;
    out << " je endfor_" << label << endl;
    fs->b->accept(this);
    fs->adder->accept(this);
    out << " jmp for_" << label << endl;
    out << "endfor_" << label << ":"<< endl;
    return 0;
}

int GenCodeVisitor::visit(IncStm *stm) {
    if (env.check(stm->id)) {
        out << " movq " << env.lookup(stm->id) << "(%rbp), %rax"<<endl;
        out << " addq $1, %rax" << endl;
        out << " movq %rax, " << env.lookup(stm->id) << "(%rbp)"<<endl;
    }
    return 0;
}

int GenCodeVisitor::visit(DecStm *stm) {
    if (env.check(stm->id)) {
        out << " movq " << env.lookup(stm->id) << "(%rbp), %rax"<<endl;
        out << " subq $1, %rax" << endl;
        out << " movq %rax, " << env.lookup(stm->id) << "(%rbp)"<<endl;
    }
    return 0;
}

int GenCodeVisitor::visit(ShortAssignStm *stm) {
    stm->e->accept(this);
    if (memoriaGlobal.count(stm->id))
        out << " movq %rax, " << stm->id << "(%rip)"<<endl;
    else
        if (env.check(stm->id)) {
            out << " movq %rax, " << env.lookup(stm->id) << "(%rbp)"<<endl;
        }
    return 0;
}

int GenCodeVisitor::visit(StringExp *exp) {
    if (stringIds.count(exp->value)) {
        out << " leaq " << stringIds[exp->value] << "(%rip), %rax" << endl;
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////

int TypeCheckerVisitor::type(Program *program) {
    for(auto i: program->fdlist) {
        i->accept(this);
    }

    return 0;
}

int TypeCheckerVisitor::visit(FunDec *fd) {
    int parametros = fd->Pnombres.size();
    locales = 0;
    fd->cuerpo->accept(this);
    fun_locales[fd->nombre] = parametros + locales;
    return 0;
}

int TypeCheckerVisitor::visit(Body *body) {
    for (auto i:body->declarations) {
        i->accept(this);
    }

    for(auto i:body->StmList) {
        i->accept(this);
    }
    return 0;
}

int TypeCheckerVisitor::visit(VarDec *vd) {
    locales += vd->vars.size();
    return 0;
}

int TypeCheckerVisitor::visit(IfStm *stm) {
    int a = locales;
    stm-> then -> accept(this);
    int b = locales;
    stm-> els  -> accept(this);
    int c = locales;
    locales = a + max(b-a,c-b);
    return 0;
}

int TypeCheckerVisitor::visit(ForStm *fs) {
    fs->b->accept(this);
    locales++;
    return 0;
}

int TypeCheckerVisitor::visit(ForWhileStm *stm) {
    return 0;
}

int TypeCheckerVisitor::visit(BinaryExp *exp) {
    return 0;
}

int TypeCheckerVisitor::visit(NumberExp *exp) {
    return 0;
}

int TypeCheckerVisitor::visit(IdExp *exp) {
    return 0;
}

int TypeCheckerVisitor::visit(Program *p) {
    return 0;
}

int TypeCheckerVisitor::visit(PrintStm *stm) {
    stm->e->accept(this);
    return 0;
}

int TypeCheckerVisitor::visit(AssignStm *stm) {
    stm->e->accept(this);
    return 0;
}

int TypeCheckerVisitor::visit(FcallExp *fcall) {
    return 0;
}

int TypeCheckerVisitor::visit(ReturnStm *r) {
    r->e->accept(this);
    return 0;
}

int TypeCheckerVisitor::visit(DecStm *stm) {
    return 0;
}

int TypeCheckerVisitor::visit(IncStm *stm) {
    return 0;
}

int TypeCheckerVisitor::visit(ShortAssignStm *stm) {
    return 0;
}

int TypeCheckerVisitor::visit(StringExp *exp) {
    string id;
    if (stringIds.count(exp->value)) {
        id = stringIds[exp->value];
    } else {
        id = ".L.str_" + to_string(stringCont++);
        stringIds[exp->value] = id;
    }
    return 0;
}

