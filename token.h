#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <ostream>

using namespace std;

class Token {
public:
    // Tipos de token
    enum Type {
        PLUS,    // +
        MINUS,   // -
        MUL,     // *
        DIV,     // /
        POW,     // **
        LPAREN,  // (
        RPAREN,  // )
        SQRT,    // sqrt
        NUM,     // Número
        ERR,     // Error
        ID,      // ID
        LT,      // <
        LE,      // <=
        GT,      // >
        GE,      // >=
        FUNC,    // func
        RETURN,  // return
        SEMICOL, // ;
        COLON,   // :
        ASSIGN,  // =
        SASSIGN, // :=
        FMT,     // fmt
        DOT,     // .
        PRINTLN, // Println
        IF,      // if
        ELSE,    // else
        VAR,     // var
        COMMA,   // ,
        TRUE,    // true
        FALSE,   // false
        PACKAGE, // package
        IMPORT,  // import
        QUOTE,   // "
        CONST,   // const
        FOR,     // for
        LBRACE,  // {
        RBRACE,  // }
        LBRACK,  // [
        RBRACK,  // ]
        STRUCT,  // struct
        TYPE,    // type
        END,     // Fin de entrada
    };

    // Atributos
    Type type;
    string text;

    // Constructores
    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const string& source, int first, int last);

    // Sobrecarga de operadores de salida
    friend ostream& operator<<(ostream& outs, const Token& tok);
    friend ostream& operator<<(ostream& outs, const Token* tok);
};

#endif // TOKEN_H