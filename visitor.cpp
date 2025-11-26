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

///////////////////////////////////////////////////////////////////////////////////

int GenCodeVisitor::generar(Program* program) {
    env.add_level();
    arrayEnv.add_level();

    typeChecker.type(program);
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


    for (auto dec : b->declarations){
        dec->accept(this);
    }
    for (auto s : b->StmList){
        s->accept(this);
    }
    arrayEnv.remove_level(); 
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

int GenCodeVisitor::visit(SimpleVarDec *vd)
{
    for (auto &var : vd->vars)
    {
        if (!entornoFuncion) {
            // variables globales
            memoriaGlobal[var] = true;
        } else {
            // variables locales
            env.add_var(var, offset);
            offset -= 8;   // cada variable ocupa 8 bytes
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
// 2. POLIMORFISMO PURO (El truco maestro):
        // En lugar de castear "dimExp" a "NumberExp", le decimos:
        // "Oye nodo, deja que 'evaluator' te visite".
        // Como dimExp es en realidad un NumberExp, el C++ llamará automáticamente
        // a evaluator.visit(NumberExp*), el cual retorna el valor entero.
        
        int val = dimExp->accept(&evaluator);
        
        // Validación básica
        if (val <= 0) {
            // Si por error metieron un StringExp, el TypeChecker probablemente
            // retornó 0 u otro valor. Aquí lo atrapamos.
            cerr << "Error: Dimensión inválida (debe ser > 0)" << endl;
            return 0;
        }

        dimensions.push_back(val);
        totalSize *= val;
    }
        // ✅ NUEVO: Determinar el tipo base del array
    Type baseType = UNDEFINED;
    if (ad->type == "int") baseType = INT;
    else if (ad->type == "bool") baseType = BOOL;
    else if (ad->type == "string") baseType = STRING;

    if (!entornoFuncion) {
        // Arrays globales (por ahora sin implementar completamente)
        memoriaGlobal[ad->id] = true;
        // Podrías también guardar en arrayEnv aquí si lo necesitas
    } else {
        // Arrays locales
        env.add_var(ad->id, offset);
        
        // Guardar info de dimensiones en el Environment
        ArrayInfo info = {dimensions, totalSize, offset,baseType};
        arrayEnv.add_var(ad->id, info);  // <-- Usar arrayEnv en lugar de map

        // Reservar espacio para cada elemento
        offset -= totalSize * 8;
    }

    // SI HAY inicializador, procesarlo
    if (ad->initializer != nullptr) {
        currentArray = ad->id;
        currentArrayIndex = 0;
        ad->initializer->accept(this);
    }
    
    return 0;
}


int GenCodeVisitor::visit(ArrayLiteralExp* exp) {
    // Recorremos los elementos genéricos (Exp*)
    for (Exp* e : exp->elements) {
        
        // PASO 1: Polimorfismo puro.
        // - Si 'e' es un sub-array: Se ejecutará visit(ArrayLiteralExp) recursivamente.
        // - Si 'e' es un número: Se ejecutará visit(NumberExp) y pondrá el valor en %rax.
        e->accept(this);

        // PASO 2: Decidir si guardamos en memoria.
        // Aquí usamos tu enum 'type' (INT, BOOL, ARRAY...) que definiste en el AST.
        // No necesitamos castear el objeto, solo saber qué es.
        
        if (e->type != ARRAY) {
            // Es un valor escalar (hoja del árbol).
            // El visit(NumberExp) ya puso el valor en %rax.
            // Ahora nosotros lo guardamos en la posición lineal que toca.
            // ✅ NUEVO: Verificar si es string
            if (e->type == STRING) {
                // Para strings, %rax ya tiene la DIRECCIÓN del string (leaq)
                // La guardamos directamente como puntero
                out << " movq %rax, " 
                    << (env.lookup(currentArray) - currentArrayIndex * 8) 
                    << "(%rbp)  # String ptr [" << currentArrayIndex << "]" << endl;
            }
            else {
                // Para int/bool, %rax tiene el VALOR
                out << " movq %rax, " 
                    << (env.lookup(currentArray) - currentArrayIndex * 8) 
                    << "(%rbp)  # Valor [" << currentArrayIndex << "]" << endl;
            }
            
            // Incrementamos el índice lineal global
            currentArrayIndex++;
        }
        
        // NOTA: Si e->type == ARRAY, NO incrementamos currentArrayIndex aquí,
        // porque la llamada recursiva (e->accept) ya incrementó el índice 
        // por cada numerito que encontró dentro de ese sub-array.
    }
    return 0;
}

int GenCodeVisitor::visit(AssignArrayStm *stm)
{
    // 1. Verificaciones (Igual que en Access)
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

    // ---------------------------------------------------------
    // PASO 1: Calcular la dirección de memoria (Igual que Access)
    // ---------------------------------------------------------
    
    out << " # Calculando dirección para asignación en " << stm->id << endl;

    // Primer índice
    stm->indices[0]->accept(this);

    // Bucle para dimensiones extra (si las hay)
    for (size_t i = 1; i < stm->indices.size(); i++) {
        out << " pushq %rax" << endl;
        stm->indices[i]->accept(this);
        out << " movq %rax, %rcx" << endl;
        out << " popq %rax" << endl;
        out << " imulq $" << dims[i] << ", %rax" << endl;
        out << " addq %rcx, %rax" << endl;
    }

    // Calcular offset final en bytes
    out << " imulq $-8, %rax" << endl;        // * -8 (stack negativo)
    out << " addq $" << baseOffset << ", %rax" << endl; // + base

    // HASTA AQUÍ ES IGUAL QUE ACCESS. 
    // Ahora %rax tiene la dirección de memoria (el offset respecto a %rbp).

    // ---------------------------------------------------------
    // PASO 2: Guardar la dirección y calcular el valor
    // ---------------------------------------------------------

    // IMPORTANTE: Guardamos la dirección calculada en la pila porque 
    // stm->value->accept(this) va a sobreescribir %rax con el resultado de la expresión.
    out << " pushq %rax  # Guardamos la dirección de destino" << endl;

    // Evaluamos la expresión de la derecha (el valor a asignar)
    stm->value->accept(this);
    // Ahora %rax tiene el VALOR (ej: 5, o resultado de una suma)

    // ---------------------------------------------------------
    // PASO 3: Realizar la asignación
    // ---------------------------------------------------------

    // Recuperamos la dirección guardada en un registro temporal (%rcx)
    out << " popq %rcx   # Recuperamos la dirección de destino" << endl;

    // Movemos el valor (%rax) a la dirección de memoria (%rcx + %rbp)
    out << " movq %rax, (%rbp, %rcx) # Asignación final" << endl;

    return 0;
}

int GenCodeVisitor::visit(ArrayAccessExp *exp)
{
    // 1. Verificaciones y búsqueda de información (Igual que antes)
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

    // ---------------------------------------------------------
    // PASO 1: Calcular el primer índice (siempre existe)
    // ---------------------------------------------------------
    exp->indices[0]->accept(this);
    // En este punto, %rax tiene el valor del primer índice (i0)

    // ---------------------------------------------------------
    // PASO 2: Bucle para dimensiones adicionales (Horner's Method)
    // ---------------------------------------------------------
    // Si es 1D (size=1), este bucle NO se ejecuta (i=1 < 1 es falso).
    // Si es 2D o más, entra y acumula: acc = (acc * dim_size) + index
    
    for (size_t i = 1; i < exp->indices.size(); i++) {
        // Guardamos el acumulado actual en la pila para evaluar el siguiente índice
        out << " pushq %rax" << endl;

        // Evaluamos el siguiente índice (ej: j)
        exp->indices[i]->accept(this);

        // El resultado del nuevo índice está en %rax, lo movemos a %rcx temporalmente
        out << " movq %rax, %rcx" << endl;

        // Recuperamos el acumulado anterior
        out << " popq %rax" << endl;

        // Aplicamos la fórmula: (Acumulado * TamañoDimensión) + NuevoÍndice
        out << " imulq $" << dims[i] << ", %rax" << endl;
        out << " addq %rcx, %rax" << endl;
    }

    // ---------------------------------------------------------
    // PASO 3: Cálculo final de memoria (Igual para todos)
    // ---------------------------------------------------------
    
    // Multiplicar por -8 (Bytes por elemento + dirección negativa de stack)
    out << " imulq $-8, %rax" << endl;
    
    // Sumar la base (que ya es un número negativo, ej: -8)
    out << " addq $" << baseOffset << ", %rax" << endl;
    
    // Cargar el valor final
    out << " movq (%rbp,%rax), %rax" << endl;

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

    for (auto i : body->declarations) {
        cout << "[TC DEBUG] Procesando declaración..." << endl;
        i->accept(this);
        cout << "[TC DEBUG] Después de declaración, locales=" << locales << endl;
    }

    for(auto i : body->StmList) {
        i->accept(this);
    }

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

    cout << "[TC DEBUG] SimpleVarDec: tipo=" << vd->type << ", vars=" << vd->vars.size() << endl;
    
    for (auto &v : vd->vars) {
        cout << "[TC DEBUG]   - var: " << v << endl;
        typeEnv.add_var(v, type);
    }

    locales += vd->vars.size();
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
    // 4. GUARDAR EN EL ENTORNO (¡Faltaba esto!)
    // Esto es vital para que luego sepamos que arr[0] es un STRING
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
    // 1. Visitar índices
    for(auto idx : exp->indices) {
        idx->accept(this);
    }

    // 2. RECUPERAR EL TIPO (¡Faltaba esto!)
    ArrayInfo info;
    if (arrayEnv.lookup(exp->id, info)) {
        // Le decimos al nodo: "Tú eres del tipo que diga tu array"
        exp->type = info.baseType; 
    } else {
        exp->type = UNDEFINED;
    }
    return 0;
}

int TypeCheckerVisitor::visit(ArrayLiteralExp *exp) 
{
    exp->type = ARRAY;
    for (Exp* e : exp->elements) {
        e->accept(this);  // esto rellena e->type
    }
    return 0;
}
