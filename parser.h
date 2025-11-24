#ifndef PARSER_H       
#define PARSER_H

#include "scanner.h"    // Incluye la definición del escáner (provee tokens al parser)
#include "ast.h"        // Incluye las definiciones para construir el Árbol de Sintaxis Abstracta (AST)

class Parser {
private:
    Scanner* scanner;       // Puntero al escáner, de donde se leen los tokens
    Token *current, *previous; // Punteros al token actual y al anterior
    bool match(Token::Type ttype);   // Verifica si el token actual coincide con un tipo esperado y avanza si es así
    bool check(Token::Type ttype);   // Comprueba si el token actual es de cierto tipo, sin avanzar
    bool advance();                  // Avanza al siguiente token
    bool isAtEnd();                  // Comprueba si ya se llegó al final de la entrada
public:
    Parser(Scanner* scanner);       
    Program* parseProgram();
    FunDec* parseFunDec();
    Body* parseBody();
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
    VarDec* parseVarDec();
=======
    StructStm* parseStructStm(); // Adding a new rule sounds like an awful idea...
>>>>>>> Stashed changes
=======
    StructStm* parseStructStm(); // Adding a new rule sounds like an awful idea...
>>>>>>> Stashed changes
=======
    StructStm* parseStructStm(); // Adding a new rule sounds like an awful idea...
>>>>>>> Stashed changes
=======
    StructStm* parseStructStm(); // Adding a new rule sounds like an awful idea...
>>>>>>> Stashed changes
    Stm* parseStm();
    Exp* parseCE();
    Exp* parseBE();
    Exp* parseE();
    Exp* parseT();
    Exp* parseF();
};

#endif // PARSER_H      