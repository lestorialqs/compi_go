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

int WhileStm::accept(Visitor* visitor) {
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


// ! = = = New additions = = = !

int StructStm::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int FieldAccessExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

// ! = = = New additions = = = !

// ! = = = New additions = = = !

int StructStm::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int FieldAccessExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

// ! = = = New additions = = = !

// ! = = = New additions = = = !

int StructStm::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int FieldAccessExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

// ! = = = New additions = = = !

///////////////////////////////////////////////////////////////////////////////////

int GenCodeVisitor::generar(Program* program) {
    program->accept(this);
        return 0;
}

int GenCodeVisitor::visit(Program* program) {
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
out << ".data\nprint_fmt: .string \"%ld \\n\""<<endl;
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes

    // Further logic in the case of the existence of struct words for further memory reservation
    for (auto& [var, words] : memoriaGlobal) {
        if (words <= 1) {
            out << var << ": .quad 0" << endl;
        } else {
            // Reserve words * 8 bytes for the struct instance
            out << var << ": .zero " << (words * 8) << endl;
        }
    }


    for (auto& [id, str] : typeChecker.stringIds) {
        out << str << ": .string \""<< id << "\"" << endl;
    }
>>>>>>> Stashed changes

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

// Changes to handle variable and id types in case of id summons from a struct statement.
int GenCodeVisitor::visit(VarDec* stm) {
    int wordsPerVar = 1;

    auto it = typeChecker.structDefs.find(stm->type);
    if (it != typeChecker.structDefs.end()) {
        wordsPerVar = static_cast<int>(it->second.size());
    }

    for (auto var : stm->vars) {
        if (!entornoFuncion) {
            // Global: remember how many 8-byte slots we need
            memoriaGlobal[var] = wordsPerVar;
        } else {
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
            memoria[var] = offset;
            offset -= 8;
=======
            // Local: base offset points to first field
            env.add_var(var, offset);
            offset -= 8 * wordsPerVar;
>>>>>>> Stashed changes
=======
            // Local: base offset points to first field
            env.add_var(var, offset);
            offset -= 8 * wordsPerVar;
>>>>>>> Stashed changes
=======
            // Local: base offset points to first field
            env.add_var(var, offset);
            offset -= 8 * wordsPerVar;
>>>>>>> Stashed changes
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
        out << " movq " << memoria[exp->value] << "(%rbp), %rax"<<endl;
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
        case LE_OP:
            out << " cmpq %rcx, %rax\n"
                      << " movl $0, %eax\n"
                      << " setle %al\n"
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
        out << " movq %rax, " << memoria[stm->id] << "(%rbp)"<<endl;
            return 0;
}

int GenCodeVisitor::visit(PrintStm* stm) {
    stm->e->accept(this);
    out <<
        " movq %rax, %rsi\n"
        " leaq print_fmt(%rip), %rdi\n"
        " movl $0, %eax\n"
        " call printf@PLT\n";
            return 0;
}



int GenCodeVisitor::visit(Body* b) {
    for (auto dec : b->declarations){
        dec->accept(this);
    }
    for (auto s : b->StmList){
        s->accept(this);
    }
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

int GenCodeVisitor::visit(WhileStm* stm) {
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
    memoria.clear();
    offset = -8;
    nombreFuncion = f->nombre;
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    out << ".globl " << f->nombre << endl;
    out << f->nombre <<  ":" << endl;
    out << " pushq %rbp" << endl;
    out << " movq %rsp, %rbp" << endl;
    int size = f->Pnombres.size();
    for (int i = 0; i < size; i++) {
        memoria[f->Pnombres[i]]=offset;
        out << " movq " << argRegs[i] << "," << offset << "(%rbp)" << endl;
        offset -= 8;
    }
    for (auto i: f->cuerpo->declarations){
        i->accept(this);
    }
    int reserva = -offset - 8;
    out << " subq $" << reserva << ", %rsp" << endl;
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
<<<<<<< Updated upstream
=======

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
    if (typeChecker.stringIds.count(exp->value)) {
        out << " leaq " << typeChecker.stringIds[exp->value] << "(%rip), %rax" << endl;
    }
    return 0;
}

int GenCodeVisitor::visit(StructStm *stm) {
    return 0;
}

int GenCodeVisitor::visit(FieldAccessExp* exp) {
    IdExp* id = dynamic_cast<IdExp*>(exp->base);
    if (!id) {
        // Not supporting nested struct fields as base yet
        out << " movq $0, %rax" << endl;
        return 0;
    }

    string varName = id->value;
    int idx = exp->fieldIndex;
    if (idx < 0) idx = 0; // fallback

    if (memoriaGlobal.count(varName)) {
        // Global struct: var + idx*8(%rip)
        int bytes = idx * 8;
        if (bytes == 0)
            out << " movq " << varName << "(%rip), %rax" << endl;
        else
            out << " movq " << varName << "+" << bytes << "(%rip), %rax" << endl;
    } else if (env.check(varName)) {
        // Local struct: base offset minus 8*idx
        int baseOffset = env.lookup(varName);
        int fieldOffset = baseOffset - idx * 8;
        out << " movq " << fieldOffset << "(%rbp), %rax" << endl;
    } else {
        out << " movq $0, %rax" << endl;
    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////////////

int TypeCheckerVisitor::type(Program *program) {
    // First collect struct definitions
    for (auto s : program->sdlist) {
        s->accept(this);
    }

    // Then type-check functions (bodies use those struct types)
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
    typeEnv.add_level();
    typeNameEnv.add_level();  // NEW, this adds a level to the Name Enviorment shelf

    for (auto i : body->declarations) {
        i->accept(this);
    }

    for (auto i : body->StmList) {
        i->accept(this);
    }

    typeNameEnv.remove_level(); // NEW, remove level upon full-visit.
    typeEnv.remove_level();
    return 0;
}


// Massive change of logic: Now the VarDec visitor handles summonings of a member of a struct class instance.
int TypeCheckerVisitor::visit(VarDec *vd) {
    Type type = UNDEFINED;

    if (vd->type == "int")      type = INT;
    else if (vd->type == "bool")   type = BOOL;
    else if (vd->type == "string") type = STRING;
    // else: could be a struct type name

    int wordsPerVar = 1;

    // If this is a struct type, its size is number of fields
    auto it = structDefs.find(vd->type);
    if (it != structDefs.end()) {
        wordsPerVar = static_cast<int>(it->second.size());
    }

    for (auto &v : vd->vars) {
        typeEnv.add_var(v, type);       // basic kind (INT/BOOL/STRING/UNDEFINED)
        typeNameEnv.add_var(v, vd->type); // full type name, e.g. "student_data"
        locales += wordsPerVar;
    }

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
    exp->left->accept(this);
    exp->right->accept(this);

    Type leftType = exp->left->type;
    Type rightType = exp->right->type;

    switch (exp->op) {
        case PLUS_OP:
        case MINUS_OP:
        case MUL_OP:
        case DIV_OP:
            exp->type = (leftType == INT && rightType == INT) ? INT : UNDEFINED;
        break;

        case LT_OP:
        case LE_OP:
        case GT_OP:
        case GE_OP:
        case EQ_OP:
            exp->type = BOOL;
        break;

        default:
            exp->type = UNDEFINED;
    }
    return 0;
}

int TypeCheckerVisitor::visit(NumberExp *exp) {
    exp->type = INT;
    return 0;
}

int TypeCheckerVisitor::visit(IdExp *exp) {
    Type type;
    if (typeEnv.lookup(exp->value, type))
        exp->type = type;
    else
        exp->type = UNDEFINED;

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
    exp->type = STRING;

    if (!stringIds.count(exp->value))
        stringIds[exp->value] = ".L.str_" + to_string(stringCont++);
    return 0;
}

// New additions

int TypeCheckerVisitor::visit(StructStm* stm) {
    vector<FieldInfo> fields;

    for (size_t i = 0; i < stm->fieldNames.size(); ++i) {
        FieldInfo info;
        info.name     = stm->fieldNames[i];
        info.typeName = stm->fieldTypes[i];

        if (info.typeName == "int")      info.type = INT;
        else if (info.typeName == "bool")   info.type = BOOL;
        else if (info.typeName == "string") info.type = STRING;
        else                               info.type = UNDEFINED; // could be nested struct later, though for simplicity's sake, this should be avoided at all costs.

        fields.push_back(info);
        // NOTE: stm->fieldInits[i] is parsed but not used yet (no default init code yet)
    }

    structDefs[stm->name] = fields;
    return 0;
}

// Visitor accesses the field expressions for structs and uses dynamic casts to provide the types.
int TypeCheckerVisitor::visit(FieldAccessExp* exp) {
    // Type-check the base expression
    exp->base->accept(this);

    // We only support base as a simple variable id (for now)
    IdExp* id = dynamic_cast<IdExp*>(exp->base);
    if (!id) {
        exp->type = UNDEFINED;
        return 0;
    }

    string varName = id->value;
    string typeName;

    // Get declared type name of the variable, e.g. "student_data"
    if (!typeNameEnv.lookup(varName, typeName)) {
        exp->type = UNDEFINED;
        return 0;
    }

    auto it = structDefs.find(typeName);
    if (it == structDefs.end()) {
        exp->type = UNDEFINED;
        return 0;
    }

    const auto& fields = it->second;
    for (size_t i = 0; i < fields.size(); ++i) {
        if (fields[i].name == exp->field) {
            exp->fieldIndex = static_cast<int>(i);
            exp->type = fields[i].type; // INT / BOOL / STRING
            return 0;
        }
    }

    exp->type = UNDEFINED; // unknown field
    return 0;
}

<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
