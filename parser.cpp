#include<iostream>
#include "token.h"
#include "scanner.h"
#include "ast.h"
#include "parser.h"

using namespace std;

// =============================
// Métodos de la clase Parser
// =============================

Parser::Parser(Scanner* sc) : scanner(sc) {
    previous = nullptr;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        throw runtime_error("Error léxico");
    }
}

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;

        if (check(Token::ERR)) {
            throw runtime_error("Error lexico");
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

// =============================
// Reglas gramaticales
// =============================

Program* Parser::parseProgram() {
    cout<<"iniciando parseo"<<endl;
    Program* p = new Program();
    match(Token::PACKAGE);
    match(Token::ID);
    match(Token::SEMICOL);
    match(Token::IMPORT);
    match(Token::QUOTE);
    match(Token::FMT);
    match(Token::QUOTE);
    match(Token::SEMICOL);

    if(check(Token::TYPE)) {
        p->sdlist.push_back(parseStructDec());
        while(check(Token::TYPE)) {
            p->sdlist.push_back(parseStructDec());
        }
    }

    if(check(Token::VAR)) {
        p->vdlist.push_back(parseVarDec());
        while(check(Token::VAR)) {
            p->vdlist.push_back(parseVarDec());
        }
    }

    while (check(Token::FUNC)) {
        p->fdlist.push_back(parseFunDec());
    }
    cout << "Parser exitoso" << endl;
    return p;
}

StructDec* Parser::parseStructDec() {
    StructDec* st = new StructDec();

    match(Token::TYPE);
    match(Token::ID);
    st->name = previous->text;
    match(Token::STRUCT);

    match(Token::LBRACE);

    // Parse fields: Type ID [= expr]? ;
    match(Token::ID);
    string fieldName = previous->text;
    match(Token::ID);
    string typeName = previous->text;
    st->fieldTypes.push_back(typeName);
    st->fieldNames.push_back(fieldName);

    while(match(Token::SEMICOL)) {
        match(Token::ID);
        fieldName = previous->text;
        match(Token::ID);
        typeName = previous->text;
        st->fieldTypes.push_back(typeName);
        st->fieldNames.push_back(fieldName);
    }

    match(Token::RBRACE);
    return st;
}

VarDec* Parser::parseVarDec() {
    match(Token::VAR);

    // Primer identificador obligatorio
    match(Token::ID);
    string id = previous->text;

    // -------------------------------------------------------
    //         ¿VIENEN DIMENSIONES?  →  ES ARRAY
    // -------------------------------------------------------
    vector<Exp*> dims;

    while (match(Token::LBRACK)) {
        Exp* dim = parseCE();  // CExp
        dims.push_back(dim);
        match(Token::RBRACK);
    }

    // Si dims NO está vacío → ArrayDec
    if (!dims.empty()) {
        // Ahora debe venir el tipo
        match(Token::ID);
        string type = previous->text;

        // --------------------------------------
        // ¿Hay inicializador? (= ArrayLiteral)
        // --------------------------------------
        ArrayLiteralExp* init = nullptr;

        if (match(Token::ASSIGN)) {
            init = parseArrayLiteral();  // <-- función nueva
        }

        match(Token::SEMICOL);

        // Construimos el nodo
        ArrayDec* arr = new ArrayDec();
        arr->id = id;
        arr->type = type;
        arr->dimensiones = dims;
        arr->initializer = init;
                cout << "[PARSER] Creado ArrayDec: id=" << id 
             << ", dims=" << dims.size() 
             << ", tipo=" << type << endl;

        return arr;
    }

    // -------------------------------------------------------
    //     CASO CONTRARIO ES SimpleVarDec (NO ARRAY)
    // -------------------------------------------------------
    SimpleVarDec* vd = new SimpleVarDec();
    vd->vars.push_back(id);

    while (match(Token::COMMA)) {
        match(Token::ID);
        vd->vars.push_back(previous->text);
    }

    match(Token::ID);
    vd->type = previous->text;

    match(Token::SEMICOL);

    cout << "[PARSER] Creado SimpleVarDec: vars=" << vd->vars.size() 
         << ", tipo=" << vd->type << endl;
    return vd;
}


// Función auxiliar para parsear solo los elementos {...}
ArrayLiteralExp* Parser::parseArrayElements() {
    match(Token::LBRACE);  // {
    
    ArrayLiteralExp* literal = new ArrayLiteralExp();
    
    // Caso vacío: {}
    if (check(Token::RBRACE)) {
        match(Token::RBRACE);
        return literal;
    }

    // Parsear elementos
    do {
        if (check(Token::LBRACE)) {
            // Sub-array recursivo
            literal->elements.push_back(parseArrayElements());
        }
        else {
            // Elemento simple
            literal->elements.push_back(parseCE());
        }
    }
    while (match(Token::COMMA));

    match(Token::RBRACE);  // }
    return literal;
}
// Función principal para el literal completo
ArrayLiteralExp* Parser::parseArrayLiteral() {
    // 1. Parsear dimensiones: [3][3]
    vector<Exp*> dims;
    while (match(Token::LBRACK)) {
        Exp* dim = parseCE();
        dims.push_back(dim);
        match(Token::RBRACK);
    }

    // 2. Parsear tipo base
    match(Token::ID);
    string type = previous->text;

    // 3. Parsear elementos
    ArrayLiteralExp* literal = parseArrayElements();
    
    // 4. Asignar dims y type
    literal->dims = dims;
    literal->type = type;

    return literal;
}

FunDec *Parser::parseFunDec() {
    FunDec* fd = new FunDec();
    match(Token::FUNC);
    match(Token::ID);
    fd->nombre = previous->text;
    match(Token::LPAREN);
    if(check(Token::ID)) {
        match(Token::ID);
        fd->Pnombres.push_back(previous->text);
        match(Token::ID);
        fd->Ptipos.push_back(previous->text);

        while(match(Token::COMMA)) {
            match(Token::ID);
            fd->Pnombres.push_back(previous->text);
            match(Token::ID);
            fd->Ptipos.push_back(previous->text);
        }
    }
    match(Token::RPAREN);
    if (match(Token::ID)) {
        fd->tipo = previous->text;
    }
    match(Token::LBRACE);
    fd->cuerpo = parseBody();
    match(Token::RBRACE);
    return fd;
}

Body* Parser::parseBody(){
    Body* b = new Body();
    
    cout << "[PARSER] parseBody - inicio" << endl;

    while(check(Token::VAR)) {
        cout << "[PARSER] Parseando VAR..." << endl;
        b->declarations.push_back(parseVarDec());
    }

    cout << "[PARSER] Total declaraciones parseadas: " << b->declarations.size() << endl;

    if (!check(Token::RBRACE)) {
        b->StmList.push_back(parseStm());
        
        while(match(Token::SEMICOL)) {
            if (!check(Token::RBRACE)) {
                b->StmList.push_back(parseStm());
            }
        }
    }
    
    cout << "[PARSER] parseBody - fin, statements=" << b->StmList.size() << endl;
    return b;
}

Stm* Parser::parseStm() {
    Stm* a;
    Exp* e;
    string variable;
    Body* tb = nullptr;
    Body* fb = nullptr;
    if(match(Token::ID)){
        variable = previous->text;

        // ---  acceso array --- //
        if (check(Token::LBRACK)) {
            vector<Exp*> idx = parseArrayIndices();

            if (match(Token::ASSIGN)) {
                Exp* val = parseCE();
                // construir nodo manualmente
                AssignArrayStm* stm = new AssignArrayStm();
                stm->id = variable;
                stm->indices = idx;
                stm->value = val;
                return stm;  // <-- FALTABA ESTO
            }

        }


        if (match(Token::ASSIGN)) {
            e = parseCE();
            return new AssignStm(variable,e);
        } else if (match(Token::SASSIGN)) {
            e = parseCE();
            return new ShortAssignStm(variable,e);
        } else if (match(Token::INC)) {
            return new IncStm(variable);
        } else if (match(Token::DEC)) {
            return new DecStm(variable);
        } else if (match(Token::DOT)) {
            match(Token::ID);
            string param = previous->text;
            match(Token::ASSIGN);
            e = parseCE();
            return new FieldAssignStm(variable, param, e);
        }
    }
    else if (match(Token::FMT)) {
        match(Token::DOT);
        match(Token::PRINTLN);
        match(Token::LPAREN);
        e = parseCE();
        match(Token::RPAREN);
        return new PrintStm(e);
    }
    else if (match(Token::RETURN)) {
        ReturnStm* r  = new ReturnStm();
        r->e = parseCE();
        return r;
    }
    else if (match(Token::IF)) {
        e = parseCE();
        match(Token::LBRACE);
        tb = parseBody();
        match(Token::RBRACE);
        if (match(Token::ELSE)) {
            match(Token::LBRACE);
            fb = parseBody();
            match(Token::RBRACE);
        }
        a = new IfStm(e, tb, fb);
    }
    // for i := 0; i <= 2; i++
    else if (match(Token::FOR)) {
        ForStm* st = new ForStm();
        st->var = current->text;
        st->initial = parseStm();
        match(Token::SEMICOL);
        st->condition = parseCE();
        match(Token::SEMICOL);
        st->adder = parseStm();
        match(Token::LBRACE);
        st->b = parseBody();
        match(Token::RBRACE);
        return st;
    }
    else{
        throw runtime_error("Error sintáctico");
    }
    return a;
}
vector<Exp*> Parser::parseArrayIndices() {
    vector<Exp*> idx;
    do {
        match(Token::LBRACK);
        idx.push_back(parseCE());   // parsea expresión del índice
        match(Token::RBRACK);
    } while (check(Token::LBRACK));
    return idx;
}
Exp* Parser::parseCE() {
    Exp* l = parseBE();

    if (match(Token::LT)) {
        Exp* r = parseBE();
        return new BinaryExp(l, r, LT_OP);
    }
    if (match(Token::LE)) {
        Exp* r = parseBE();
        return new BinaryExp(l, r, LE_OP);
    }
    if (match(Token::GT)) {
        Exp* r = parseBE();
        return new BinaryExp(l, r, GT_OP);
    }
    if (match(Token::GE)) {
        Exp* r = parseBE();
        return new BinaryExp(l, r, GE_OP);
    }
    if (match(Token::EQ)) {
        Exp* r = parseBE();
        return new BinaryExp(l, r, EQ_OP);
    }
    return l;
}

Exp* Parser::parseBE() {
    Exp* l = parseE();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else{
            op = MINUS_OP;
        }
        Exp* r = parseE();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

Exp* Parser::parseE() {
    Exp* l = parseT();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL_OP;
        }
        else{
            op = DIV_OP;
        }
        Exp* r = parseT();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

Exp* Parser::parseT() {
    Exp* l = parseF();
    if (match(Token::POW)) {
        BinaryOp op = POW_OP;
        Exp* r = parseF();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

Exp* Parser::parseF() {
    Exp* e;
    string nom;
    if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    }
    else if (match(Token::TRUE)) {
        return new NumberExp(1);
    }
    else if (match(Token::FALSE)) {
        return new NumberExp(0);
    }
    else if (match(Token::QUOTE)) {
        string str = "";
        match(Token::ID);
        str.append(previous->text);
        while (match(Token::ID)) {
            str.append(" ");
            str.append(previous->text);
        }
        e = new StringExp(str);
        match(Token::QUOTE);
        return e;
    }
    else if (match(Token::LPAREN)) {
        e = parseCE();
        match(Token::RPAREN);

        if (match(Token::QUESTION)) {
            TernaryExp* tep = new TernaryExp();
            tep->condition = e;
            tep->trueExp = parseCE();
            match(Token::COLON);
            tep->falseExp = parseCE();
            return tep;
        }
        return e;
    }
    else if (match(Token::ID)) {
        nom = previous->text;

        if(check(Token::LPAREN)) {
            match(Token::LPAREN);
            FcallExp* fcall = new FcallExp();
            fcall->nombre = nom;


            fcall->argumentos.push_back(parseCE());
            while (match(Token::COMMA)) {
                fcall->argumentos.push_back(parseCE());
            }
            match(Token::RPAREN);
            return fcall;
        }
        // --- acceso a arreglo: id[exp][exp]... --- //
        if (check(Token::LBRACK)) {
            vector<Exp*> idx = parseArrayIndices();

            ArrayAccessExp* arr = new ArrayAccessExp();
            arr->id = nom;
            arr->indices = idx;
            return arr;
        }
        else {
            return new IdExp(nom);
        } else {
            IdExp* base = new IdExp(nom);

            if (match(Token::DOT)) {
                match(Token::ID);
                string fieldName = previous->text;
                return new FieldAccessExp(nom, fieldName);
            }

            return base;
        }
    }
    
    else {
        throw runtime_error("Error sintáctico");
    }
}