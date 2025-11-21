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
    match(Token::PACKAGE);
    match(Token::ID);
    match(Token::SEMICOL);
    match(Token::IMPORT);
    match(Token::QUOTE);
    match(Token::FMT);
    match(Token::QUOTE);
    match(Token::SEMICOL);

    if(check(Token::VAR)) {
        p->vdlist.push_back(parseVarDec());
        while(check(Token::VAR)) {
            p->vdlist.push_back(parseVarDec());
        }
    }

    while(check(Token::FUNC)) {
        p->fdlist.push_back(parseFunDec());
    }
    cout << "Parser exitoso" << endl;
    return p;
}

VarDec* Parser::parseVarDec() {
    VarDec* vd = new VarDec();
    match(Token::VAR);
    match(Token::ID);
    vd->vars.push_back(previous->text);
    while(match(Token::COMMA)) {
        match(Token::ID);
        vd->vars.push_back(previous->text);
    }
    match(Token::ID);
    vd->type = previous->text;
    // match(Token::SEMICOL);  <-- REMOVED SEMICOL FOR TESTING
    return vd;
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
            while(match(Token::COMMA)) {
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
