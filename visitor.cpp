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

// arrays 
int SimpleVarDec::accept(Visitor* v) {
    return v->visit(this);
}
int ArrayDec::accept(Visitor* visit) {
    return visit->visit(this);
}
int AssignArrayStm::accept(Visitor* visit) {
    return visit->visit(this);
}
int ArrayAccessExp::accept(Visitor* visit) {
     return visit->visit(this);
}
int ArrayLiteralExp::accept(Visitor* visit) {
    return visit->visit(this);
}

int FieldAccessExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int StructDec::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int FieldAssignStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int TernaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

///////////////////////////////////////////////////////////////////////////////////

int GenCodeVisitor::generar(Program* program) {
    env.add_level();
    arrayEnv.add_level();
    structTypeEnv.add_level();
    typeChecker.type(program);
    optimizer.optimizar(program);
    fun_reserva = typeChecker.fun_locales;
    program->accept(this);
        return 0;
}

int GenCodeVisitor::visit(Program* program) {
    out << ".data\nprint_fmt: .string \"%ld \\n\""<<endl;
    out << "string_fmt: .string \"%s \\n\""<<endl;

    for (auto& [id, str] : typeChecker.stringIds) {
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
    // WARNING: Asume que el tipo se mantiene
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
    if (stm->e->type == STRING) {
        out <<
            " movq %rax, %rsi\n"
            " leaq string_fmt(%rip), %rdi\n"
            " movl $0, %eax\n"
            " call printf@PLT\n";
    } else {
        out <<
            " movq %rax, %rsi\n"
            " leaq print_fmt(%rip), %rdi\n"
            " movl $0, %eax\n"
            " call printf@PLT\n";
    }
    return 0;
}

int GenCodeVisitor::visit(Body* b) {
    env.add_level();
    arrayEnv.add_level();
    structTypeEnv.add_level();
    for (auto dec : b->declarations){
        dec->accept(this);
    }
    for (auto s : b->StmList){
        s->accept(this);
    }
    arrayEnv.remove_level(); 
    env.remove_level();
    structTypeEnv.remove_level();
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
    arrayEnv.clear();


    env.add_level();
    arrayEnv.add_level();
    
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
    if (typeChecker.stringIds.count(exp->value)) {
        out << " leaq " << typeChecker.stringIds[exp->value] << "(%rip), %rax" << endl;
    }
    return 0;
}

int GenCodeVisitor::visit(SimpleVarDec *vd) {
    int wordsPerVar = 1;

    auto it = typeChecker.structDefs.find(vd->type);
    if (it != typeChecker.structDefs.end()) {
        wordsPerVar = static_cast<int>(it->second.size());
    }

    for (auto var : vd->vars) {
        if (!entornoFuncion) {
            // Global: remember how many 8-byte slots we need
            memoriaGlobal[var] = wordsPerVar;
        } else {
            // Local: base offset points to first field
            env.add_var(var, offset);
            if (typeChecker.structDefs.count(vd->type)) {
                structTypeEnv.add_var(var, vd->type);
            }
            offset -= 8 * wordsPerVar;
        }
    }
    return 0;
}

int GenCodeVisitor::visit(ArrayDec *ad)
{
    // Evaluar cada dimensión (debe ser NumberExp)
    vector<int> dimensions;
    int totalSize = 1;
    TypeCheckerVisitor evaluator;

    for (Exp* dimExp : ad->dimensiones) {
        int val = dimExp->accept(&evaluator);
        
        if (val <= 0) {
            cerr << "Error: Dimensión inválida (debe ser > 0)" << endl;
            return 0;
        }

        dimensions.push_back(val);
        totalSize *= val;
    }

    Type baseType = UNDEFINED;
    if (ad->type == "int") baseType = INT;
    else if (ad->type == "bool") baseType = BOOL;
    else if (ad->type == "string") baseType = STRING;

    if (!entornoFuncion) {
        memoriaGlobal[ad->id] = true;
    } else {
        env.add_var(ad->id, offset);
        
        ArrayInfo info = {dimensions, totalSize, offset,baseType};
        arrayEnv.add_var(ad->id, info);
        offset -= totalSize * 8;
    }

    if (ad->initializer != nullptr) {
        currentArray = ad->id;
        currentArrayIndex = 0;
        ad->initializer->accept(this);
    }
    
    return 0;
}


int GenCodeVisitor::visit(ArrayLiteralExp* exp) {
    for (Exp* e : exp->elements) {
        e->accept(this);
        if (e->type != ARRAY) {
            if (e->type == STRING) {
                out << " movq %rax, " << (env.lookup(currentArray) - currentArrayIndex * 8) << "(%rbp)  # String ptr [" << currentArrayIndex << "]" << endl;
            }
            else {
                out << " movq %rax, " << (env.lookup(currentArray) - currentArrayIndex * 8) << "(%rbp)  # Valor [" << currentArrayIndex << "]" << endl;
            }
            currentArrayIndex++;
        }
    }
    return 0;
}

int GenCodeVisitor::visit(AssignArrayStm *stm)
{
    if (!env.check(stm->id)) {
        cerr << "Error: Array " << stm->id << " no definido" << endl;
        return 0;
    }

    int baseOffset = env.lookup(stm->id);
    
    ArrayInfo info;
    if (!arrayEnv.lookup(stm->id, info)) {
        cerr << "Error: No hay info de dimensiones para " << stm->id << endl;
        return 0;
    }

    vector<int> dims = info.dimensions;

    out << " # Calculando dirección para asignación en " << stm->id << endl;

    stm->indices[0]->accept(this);

    for (size_t i = 1; i < stm->indices.size(); i++) {
        out << " pushq %rax" << endl;
        stm->indices[i]->accept(this);
        out << " movq %rax, %rcx" << endl;
        out << " popq %rax" << endl;
        out << " imulq $" << dims[i] << ", %rax" << endl;
        out << " addq %rcx, %rax" << endl;
    }

    out << " imulq $-8, %rax" << endl;        // * -8 (stack negativo)
    out << " addq $" << baseOffset << ", %rax" << endl; // + base
    out << " pushq %rax  # Guardamos la dirección de destino" << endl;
    stm->value->accept(this);
    out << " popq %rcx   # Recuperamos la dirección de destino" << endl;
    out << " movq %rax, (%rbp, %rcx) # Asignación final" << endl;

    return 0;
}

int GenCodeVisitor::visit(ArrayAccessExp *exp)
{
    if (!env.check(exp->id)) {
        cerr << "Error: Array " << exp->id << " no definido" << endl;
        return 0;
    }

    int baseOffset = env.lookup(exp->id);
    
    ArrayInfo info;
    if (!arrayEnv.lookup(exp->id, info)) {
        cerr << "Error: No hay info de dimensiones para " << exp->id << endl;
        return 0;
    }

    vector<int> dims = info.dimensions;

    out << " # Acceso unificado a " << exp->id << endl;
    exp->indices[0]->accept(this);

    for (size_t i = 1; i < exp->indices.size(); i++) {
        out << " pushq %rax" << endl;
        exp->indices[i]->accept(this);
        out << " movq %rax, %rcx" << endl;
        out << " popq %rax" << endl;
        out << " imulq $" << dims[i] << ", %rax" << endl;
        out << " addq %rcx, %rax" << endl;
    }
    out << " imulq $-8, %rax" << endl;
    out << " addq $" << baseOffset << ", %rax" << endl;
    out << " movq (%rbp,%rax), %rax" << endl;
    return 0;
}

int GenCodeVisitor::visit(FieldAccessExp* exp) {
    string varName = exp->base;

    string structType;
    if (structTypeEnv.lookup(varName, structType)) {
        int idx = 0;
        for (const FieldInfo& param : typeChecker.structDefs[structType]) {
            if (exp->field == param.name) {
                idx = param.paramOffset;
            }
        }

        if (memoriaGlobal.count(varName)) {
            // Global struct: var + idx*8(%rip)
            if (idx == 0)
                out << " movq " << varName << "(%rip), %rax" << endl;
            else
                out << " movq " << varName << "+" << idx << "(%rip), %rax" << endl;
        } else if (env.check(varName)) {
            int baseOffset = env.lookup(varName);
            int fieldOffset = baseOffset - idx;
            out << " movq " << fieldOffset << "(%rbp), %rax" << endl;
        } else {
            out << " movq $0, %rax" << endl;
        }
    }

    return 0;
}

int GenCodeVisitor::visit(StructDec* stm) {
    return 0;
}

int GenCodeVisitor::visit(FieldAssignStm *stm) {
    stm->e->accept(this);

    string varName = stm->base;
    string structType;

    if (structTypeEnv.lookup(varName, structType)) {
        int idx = 0;
        for (const FieldInfo& param : typeChecker.structDefs[structType]) {
            if (stm->param == param.name) {
                idx = param.paramOffset;
            }
        }

        if (memoriaGlobal.count(varName)) {
            // Global struct: var + idx*8(%rip)
            if (idx == 0)
                out << " movq %rax, " << varName << "(%rip)" << endl;
            else
                out << " movq %rax, " << varName << "+" << idx << "(%rip)" << endl;
        } else if (env.check(varName)) {
            int baseOffset = env.lookup(varName);
            int fieldOffset = baseOffset - idx;
            out << " movq %rax, " << fieldOffset << "(%rbp)" << endl;
        }
    }

    return 0;
}

int GenCodeVisitor::visit(TernaryExp *exp) {
    int label = labelcont++;
    exp->condition->accept(this);
    out << " cmpq $0, %rax"<<endl;
    out << " je else_" << label << endl;
    exp->trueExp->accept(this);
    out << " jmp endt_" << label << endl;
    out << " else_" << label << ":"<< endl;
    exp->falseExp->accept(this);
    out << "endt_" << label << ":"<< endl;
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////

int TypeCheckerVisitor::type(Program *program) {
    for (auto s : program->sdlist) {
        s->accept(this);
    }

    for(auto i: program->fdlist) {
        i->accept(this);
    }

    return 0;
}


int TypeCheckerVisitor::visit(FunDec *fd) {
    int parametros = fd->Pnombres.size();
    cout << "[TC DEBUG] FunDec: " << fd->nombre << " parametros=" << parametros << endl;
    locales = 0;
    
    cout << "[TC DEBUG] Procesando cuerpo de " << fd->nombre << endl;
    fd->cuerpo->accept(this);
    
    cout << "[TC DEBUG] Después de Body: locales=" << locales << endl;
    fun_locales[fd->nombre] = parametros + locales;
    cout << "[TC DEBUG] fun_locales[" << fd->nombre << "] = " << fun_locales[fd->nombre] << endl;
    return 0;
}
int TypeCheckerVisitor::visit(Body *body) {
    cout << "[TC DEBUG] Body - inicio, locales=" << locales << endl;
    cout << "[TC DEBUG] Num declaraciones: " << body->declarations.size() << endl;
    cout << "[TC DEBUG] Num statements: " << body->StmList.size() << endl;
    
    typeEnv.add_level();
    structTypeEnv.add_level();

    for (auto i : body->declarations) {
        cout << "[TC DEBUG] Procesando declaración..." << endl;
        i->accept(this);
        cout << "[TC DEBUG] Después de declaración, locales=" << locales << endl;
    }

    for (auto i : body->StmList) {
        i->accept(this);
    }

    structTypeEnv.remove_level();
    typeEnv.remove_level();
    cout << "[TC DEBUG] Body - fin, locales=" << locales << endl;
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
    return exp->value;
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

int TypeCheckerVisitor::visit(SimpleVarDec* vd) {
    Type type = UNDEFINED;

    if (vd->type == "int") type = INT;
    else if (vd->type == "bool") type = BOOL;
    else if (vd->type == "string") type = STRING;
    // else: could be a struct type name

    cout << "[TC DEBUG] SimpleVarDec: tipo=" << vd->type << ", vars=" << vd->vars.size() << endl;

    int wordsPerVar = 1;

    // If this is a struct type, its size is number of fields
    auto it = structDefs.find(vd->type);
    if (it != structDefs.end()) {
        wordsPerVar = static_cast<int>(it->second.size());
    }

    for (auto &v : vd->vars) {
        cout << "[TC DEBUG]   - var: " << v << endl;
        typeEnv.add_var(v, type);       // basic kind (INT/BOOL/STRING/UNDEFINED)
        if (structDefs.count(vd->type)) {
            structTypeEnv.add_var(v, vd->type);
        }
        locales += wordsPerVar;
    }

    cout << "[TC DEBUG] SimpleVarDec: locales incrementado a " << locales << endl;
    return 0;
}

int TypeCheckerVisitor::visit(ArrayDec* ad) {
    // 1. Determinar tipo base
    Type baseType = UNDEFINED;
    if (ad->type == "int") baseType = INT;
    else if (ad->type == "bool") baseType = BOOL;
    else if (ad->type == "string") baseType = STRING;

    cout << "[TC DEBUG] ArrayDec: id=" << ad->id <<", tipo=" << ad->type
         << ", dimensiones=" << ad->dimensiones.size() << endl;
    vector<int> currentDims;
    
    int totalSize = 1;
    for (Exp* dimExp : ad->dimensiones) {
        int number = dimExp->accept(this);
        cout << "[TC DEBUG]   - dimensión: " << number << endl;
        totalSize *= number;
        currentDims.push_back(number);
    }
    
    cout << "[TC DEBUG] ArrayDec: totalSize=" << totalSize << endl;
    locales += totalSize;
    cout << "[TC DEBUG] ArrayDec: locales incrementado a " << locales << endl;
    ArrayInfo info = {currentDims, totalSize, 0, baseType};
    arrayEnv.add_var(ad->id, info);

    if (ad->initializer != nullptr) {
        ad->initializer->accept(this);
    }
    
    return 0;
}

int TypeCheckerVisitor::visit(AssignArrayStm *stm)
{
    return 0;
}

int TypeCheckerVisitor::visit(ArrayAccessExp *exp) {
    for(auto idx : exp->indices) {
        idx->accept(this);
    }

    ArrayInfo info;
    if (arrayEnv.lookup(exp->id, info)) {
        exp->type = info.baseType;
    } else {
        exp->type = UNDEFINED;
    }
    return 0;
}

int TypeCheckerVisitor::visit(ArrayLiteralExp *exp) {
    exp->type = ARRAY;
    for (Exp* e : exp->elements) {
        e->accept(this);  // esto rellena e->type
    }
}

int TypeCheckerVisitor::visit(StructDec* stm) {
    vector<FieldInfo> fields;

    for (int i = 0; i < stm->fieldNames.size(); ++i) {
        FieldInfo info;
        info.name = stm->fieldNames[i];
        info.paramOffset = i * 8;
        string typeString = stm->fieldTypes[i];

        if (typeString == "int") info.type = INT;
        else if (typeString == "bool") info.type = BOOL;
        else if (typeString == "string") info.type = STRING;
        else info.type = UNDEFINED;
        fields.emplace_back(info);
    }

    structDefs[stm->name] = fields;
    return 0;
}

int TypeCheckerVisitor::visit(FieldAccessExp* exp) {
    // WARNING: Asume que exp->base (id) si es de tipo struct

    string varName = exp->base;
    string structType;

    if (structTypeEnv.lookup(varName, structType)) {
        for (const FieldInfo& param : structDefs[structType]) {
            if (exp->field == param.name) {
                exp->type = param.type;
            }
        }
    } else {
        exp->type = UNDEFINED;
    }

    return 0;
}

int TypeCheckerVisitor::visit(FieldAssignStm *stm) {
    stm->e->accept(this);
    return 0;
}

int TypeCheckerVisitor::visit(TernaryExp *exp) {
    exp->trueExp->accept(this);
    exp->falseExp->accept(this);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////

Exp *CodeOptimizerVisitor::constFold(Exp *e) {
    BinaryExp* bin = dynamic_cast<BinaryExp*>(e);
    if (!bin) return e;

    bin->left = constFold(bin->left);
    bin->right = constFold(bin->right);

    NumberExp* leftRes = dynamic_cast<NumberExp*>(bin->left);
    NumberExp* rightRes = dynamic_cast<NumberExp*>(bin->right);
    if (!leftRes || !rightRes) return e;

    int lv = leftRes->value;
    int rv = rightRes->value;
    int res;

    switch (bin->op) {
        case PLUS_OP:  res = lv + rv; break;
        case MINUS_OP: res = lv - rv; break;
        case MUL_OP:   res = lv * rv; break;
        case LT_OP:    res = lv < rv; break;
        case LE_OP:    res = lv <= rv; break;
        case GT_OP:    res = lv > rv; break;
        case GE_OP:    res = lv >= rv; break;
        case EQ_OP:    res = (lv == rv); break;
        default:
            return e;
    }

    NumberExp* newNode = new NumberExp(res);
    newNode->type = bin->type;
    return newNode;
}

int CodeOptimizerVisitor::optimizar(Program *program) {
    for (auto fd : program->fdlist) {
        fd->accept(this);
    }

    for (auto vd : program->vdlist) {
        vd->accept(this);
    }
    return 0;
}

int CodeOptimizerVisitor::visit(BinaryExp *exp) {
    return 0;
}

int CodeOptimizerVisitor::visit(NumberExp *exp) {
    return 0;
}

int CodeOptimizerVisitor::visit(IdExp *exp) {
    return 0;
}

int CodeOptimizerVisitor::visit(Program *p) {
    return 0;
}

int CodeOptimizerVisitor::visit(PrintStm *stm) {
    stm->e = constFold(stm->e);
    return 0;
}

int CodeOptimizerVisitor::visit(AssignStm *stm) {
    stm->e = constFold(stm->e);
    return 0;
}

int CodeOptimizerVisitor::visit(ForWhileStm *stm) {
    return 0;
}

int CodeOptimizerVisitor::visit(IfStm *stm) {
    stm->condition = constFold(stm->condition);
    stm->then->accept(this);
    if (stm->els) stm->els->accept(this);
    return 0;
}

int CodeOptimizerVisitor::visit(Body *body) {
    for (auto dec : body->declarations) {
        dec->accept(this);
    }

    for (auto stm : body->StmList) {
        stm->accept(this);
    }
    return 0;
}

int CodeOptimizerVisitor::visit(FcallExp *fcall) {
    for (auto arg : fcall->argumentos) {
        arg->accept(this);
    }
    return 0;
}

int CodeOptimizerVisitor::visit(ReturnStm *r) {
    r->e = constFold(r->e);
    return 0;
}

int CodeOptimizerVisitor::visit(FunDec *fd) {
    if (fd->cuerpo) {
        fd->cuerpo->accept(this);
    }
    return 0;
}

int CodeOptimizerVisitor::visit(ForStm *fs) {
    fs->initial->accept(this);
    fs->condition = constFold(fs->condition);
    fs->adder->accept(this);
    fs->b->accept(this);
    return 0;
}

int CodeOptimizerVisitor::visit(ShortAssignStm *stm) {
    return 0;
}

int CodeOptimizerVisitor::visit(IncStm *stm) {
    return 0;
}

int CodeOptimizerVisitor::visit(DecStm *stm) {
    return 0;
}

int CodeOptimizerVisitor::visit(StringExp *exp) {
    return 0;
}

int CodeOptimizerVisitor::visit(SimpleVarDec *vd) {
    return 0;
}

int CodeOptimizerVisitor::visit(ArrayDec *vd) {
    return 0;
}

int CodeOptimizerVisitor::visit(AssignArrayStm *stm) {
    return 0;
}

int CodeOptimizerVisitor::visit(ArrayAccessExp *exp) {
    return 0;
}

int CodeOptimizerVisitor::visit(ArrayLiteralExp *exp) {
    return 0;
}

int CodeOptimizerVisitor::visit(StructDec *stm) {
    return 0;
}

int CodeOptimizerVisitor::visit(FieldAccessExp *exp) {
    return 0;
}

int CodeOptimizerVisitor::visit(FieldAssignStm *stm) {
    return 0;
}

int CodeOptimizerVisitor::visit(TernaryExp *exp) {
    return 0;
}