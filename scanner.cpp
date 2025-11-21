#include <iostream>
#include <cstring>
#include <fstream>
#include "token.h"
#include "scanner.h"

using namespace std;

// -----------------------------
// Constructor
// -----------------------------
Scanner::Scanner(const char* s): input(s), first(0), current(0) {}

// -----------------------------
// Función auxiliar
// -----------------------------

bool is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

// -----------------------------
// nextToken: obtiene el siguiente token
// -----------------------------


Token* Scanner::nextToken() {
    Token* token;

    // Saltar espacios en blanco
    while (current < input.length() && is_white_space(input[current])) 
        current++;

    // Fin de la entrada
    if (current >= input.length()) 
        return new Token(Token::END);

    char c = input[current];

    first = current;

    // Números
    if (isdigit(c)) {
        current++;
        while (current < input.length() && isdigit(input[current]))
            current++;
        token = new Token(Token::NUM, input, first, current - first);
    }
    // ID
    else if (isalpha(c)) {
        current++;
        while (current < input.length() && isalnum(input[current]))
            current++;
        string lexema = input.substr(first, current - first);
        if (lexema=="sqrt") return new Token(Token::SQRT, input, first, current - first);
        else if (lexema=="func") return new Token(Token::FUNC, input, first, current - first);
        else if (lexema=="return") return new Token(Token::RETURN, input, first, current - first);
        else if (lexema=="fmt") return new Token(Token::FMT, input, first, current - first);
        else if (lexema=="Println") return new Token(Token::PRINTLN, input, first, current - first);
        else if (lexema=="if") return new Token(Token::IF, input, first, current - first);
        else if (lexema=="else") return new Token(Token::ELSE, input, first, current - first);
        else if (lexema=="var") return new Token(Token::VAR, input, first, current - first);
        else if (lexema=="true") return new Token(Token::TRUE, input, first, current - first);
        else if (lexema=="false") return new Token(Token::FALSE, input, first, current - first);
        else if (lexema=="package") return new Token(Token::PACKAGE, input, first, current - first);
        else if (lexema=="import") return new Token(Token::IMPORT, input, first, current - first);
        else if (lexema=="const") return new Token(Token::CONST, input, first, current - first);
        else if (lexema=="for") return new Token(Token::FOR, input, first, current - first);
        else if (lexema=="struct") return new Token(Token::STRUCT, input, first, current - first);
        else if (lexema=="type") return new Token(Token::TYPE, input, first, current - first);
        else return new Token(Token::ID, input, first, current - first);
    }
    // Operadores
    else if (strchr("+/-*();=<>:{}[].,\"", c)) {
        switch (c) {
            case '+':
                if (input[current+1]=='+') {
                    current++;
                    token = new Token(Token::INC, input, first, current + 1 - first);
                } else {
                    token = new Token(Token::PLUS, c);
                }
            break;
            case '-':
                if (input[current+1]=='-') {
                    current++;
                    token = new Token(Token::DEC, input, first, current + 1 - first);
                } else {
                    token = new Token(Token::MINUS, c);
                }
            break;
            case '*':
                if (input[current+1]=='*') {
                    current++;
                    token = new Token(Token::POW, input, first, current + 1 - first);
                } else {
                    token = new Token(Token::MUL, c);
                }
            break;
            case '/': token = new Token(Token::DIV,   c); break;
            case '(': token = new Token(Token::LPAREN,c); break;
            case ')': token = new Token(Token::RPAREN,c); break;
            case '<':
                if (input[current+1]=='=') {
                    current++;
                    token = new Token(Token::LE, input, first, current + 1 - first);
                } else {
                    token = new Token(Token::LT, c);
                }
            break;
            case '>':
                if (input[current+1]=='=') {
                    current++;
                    token = new Token(Token::GE, input, first, current + 1 - first);
                } else {
                    token = new Token(Token::GT, c);
                }
            break;
            case ';': token = new Token(Token::SEMICOL,c); break;
            case ':':
                if (input[current+1]=='=') {
                    current++;
                    token = new Token(Token::SASSIGN, input, first, current + 1 - first);
                } else {
                    token = new Token(Token::COLON, c);
                }
            break;
            case '=':
                if (input[current+1]=='=') {
                    current++;
                    token = new Token(Token::EQ, input, first, current + 1 - first);
                } else {
                    token = new Token(Token::ASSIGN, c);
                }
            break;
            case '.': token = new Token(Token::DOT,c); break;
            case ',': token = new Token(Token::COMMA,c); break;
            case '"': token = new Token(Token::QUOTE,c); break;
            case '{': token = new Token(Token::LBRACE,c); break;
            case '}': token = new Token(Token::RBRACE,c); break;
            case '[': token = new Token(Token::LBRACK,c); break;
            case ']': token = new Token(Token::RBRACK,c); break;
        }
        current++;
    }

    // Carácter inválido
    else {
        token = new Token(Token::ERR, c);
        current++;
    }

    return token;
}




// -----------------------------
// Destructor
// -----------------------------
Scanner::~Scanner() { }

// -----------------------------
// Función de prueba
// -----------------------------

int ejecutar_scanner(Scanner* scanner, const string& InputFile) {
    Token* tok;

    // Crear nombre para archivo de salida
    string OutputFileName = InputFile;
    size_t pos = OutputFileName.find_last_of(".");
    if (pos != string::npos) {
        OutputFileName = OutputFileName.substr(0, pos);
    }
    OutputFileName += "_tokens.txt";

    ofstream outFile(OutputFileName);
    if (!outFile.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << OutputFileName << endl;
        return 0;
    }

    outFile << "Scanner\n" << endl;

    while (true) {
        tok = scanner->nextToken();

        if (tok->type == Token::END) {
            outFile << *tok << endl;
            delete tok;
            outFile << "\nScanner exitoso" << endl << endl;
            outFile.close();
            return 0;
        }

        if (tok->type == Token::ERR) {
            outFile << *tok << endl;
            delete tok;
            outFile << "Caracter invalido" << endl << endl;
            outFile << "Scanner no exitoso" << endl << endl;
            outFile.close();
            return 0;
        }

        outFile << *tok << endl;
        delete tok;
    }
}
