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
    Program* p = new Program();
    if(check(Token::VAR)) {
        p->vdlist.push_back(parseVarDec());
        while(match(Token::SEMICOL)) {
            if(check(Token::VAR)) {
                p->vdlist.push_back(parseVarDec());
            }
        }
    }
    if(check(Token::FUN)) {
        p->fdlist.push_back(parseFunDec());
        while(check(Token::FUN)){
                p->fdlist.push_back(parseFunDec());
            }
        }
    cout << "Parser exitoso" << endl;
    return p;
}

VarDec* Parser::parseVarDec(){
    VarDec* vd = new VarDec();
    match(Token::VAR);
    match(Token::ID);
    vd->type = previous->text;
    match(Token::ID);
    vd->vars.push_back(previous->text);
    while(match(Token::COMA)) {
        match(Token::ID);
        vd->vars.push_back(previous->text);
    }
    return vd;
}

FunDec *Parser::parseFunDec() {
    FunDec* fd = new FunDec();
    match(Token::FUN);
    match(Token::ID);
    fd->tipo = previous->text;
    match(Token::ID);
    fd->nombre = previous->text;
    match(Token::LPAREN);
    if(check(Token::ID)) {
        while(match(Token::ID)) {
            fd->Ptipos.push_back(previous->text);
            match(Token::ID);
            fd->Pnombres.push_back(previous->text);
            match(Token::COMA);
        }
    }
    match(Token::RPAREN);
    fd->cuerpo = parseBody();
    match(Token::ENDFUN);
    return fd;
}



Body* Parser::parseBody(){
    Body* b = new Body();
    if(check(Token::VAR)) {
        b->declarations.push_back(parseVarDec());
        while(match(Token::SEMICOL)) {
            if(check(Token::VAR)) {
                b->declarations.push_back(parseVarDec());
            }
        }
    }
    b->StmList.push_back(parseStm());
    while(match(Token::SEMICOL)) {
        b->StmList.push_back(parseStm());
    }
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
        match(Token::ASSIGN);
        e = parseCE();
        return new AssignStm(variable,e);
    }
    else if(match(Token::PRINT)){
        match(Token::LPAREN);
        e = parseCE();
        match(Token::RPAREN);
        return new PrintStm(e);
    }
    else if(match(Token::RETURN)) {
        ReturnStm* r  = new ReturnStm();
        match(Token::LPAREN);
        r->e = parseCE();
        match(Token::RPAREN);
        return r;
    }
else if (match(Token::IF)) {
        e = parseCE();
        if (!match(Token::THEN)) {
            cout << "Error: se esperaba 'then' después de la expresión." << endl;
            exit(1);
        }
        tb = parseBody();
        if (match(Token::ELSE)) {
            fb = parseBody();
        }
        if (!match(Token::ENDIF)) {
            cout << "Error: se esperaba 'endif' al final de la declaración de if." << endl;
            exit(1);
        }
        a = new IfStm(e, tb, fb);
    }
    else if (match(Token::WHILE)) {
        e = parseCE();
        if (!match(Token::DO)) {
            cout << "Error: se esperaba 'do' después de la expresión." << endl;
            exit(1);
        }
        tb = parseBody();
        if (!match(Token::ENDWHILE)) {
            cout << "Error: se esperaba 'endwhile' al final de la declaración." << endl;
            exit(1);
        }
        a = new WhileStm(e, tb);
    }
    else{
        throw runtime_error("Error sintáctico");
    }
    return a;
}

Exp* Parser::parseCE() {
    Exp* l = parseBE();
    if (match(Token::LE)) {
        BinaryOp op = LE_OP;
        Exp* r = parseBE();
        l = new BinaryExp(l, r, op);
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
    else if (match(Token::LPAREN))
    {
        e = parseCE();
        match(Token::RPAREN);
        return e;
    }
    else if (match(Token::ID)) {
        nom = previous->text;
        if(check(Token::LPAREN)) {
            match(Token::LPAREN);
            FcallExp* fcall = new FcallExp();
            fcall->nombre = nom;
            fcall->argumentos.push_back(parseCE());
            while(match(Token::COMA)) {
                fcall->argumentos.push_back(parseCE());
            }
            match(Token::RPAREN);
            return fcall;
        }
        else {
            return new IdExp(nom);
            }
    }
    else {
        throw runtime_error("Error sintáctico");
    }
}
