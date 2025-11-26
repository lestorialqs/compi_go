#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"
#include "parser.h"
#include "ast.h"
#include "visitor.h"

using namespace std;

int main(int argc, const char* argv[]) {
    cerr << "=== INICIANDO ===" << endl;
    
    // Verificar número de argumentos
    if (argc != 2) {
        cout << "Número incorrecto de argumentos.\n";
        cout << "Uso: " << argv[0] << " <archivo_de_entrada>" << endl;
        return 1;
    }

    // Abrir archivo de entrada
    ifstream infile(argv[1]);
    if (!infile.is_open()) {
        cout << "No se pudo abrir el archivo: " << argv[1] << endl;
        return 1;
    }

    // Leer contenido completo del archivo en un string
    string input, line;
    while (getline(infile, line)) {
        input += line + '\n';
    }
    infile.close();

    // Preparar nombres de archivos de salida
    string inputFile(argv[1]);
    size_t dotPos = inputFile.find_last_of('.');
    string baseName = (dotPos == string::npos) ? inputFile : inputFile.substr(0, dotPos);
    
    string tokensFilename = baseName + "_tokens.txt";
    string asmFilename = baseName + ".s";
    string errorFilename = baseName + "_error.txt";

    // ==========================================
    // FASE 1: ESCANEO (TOKENS)
    // ==========================================
    cerr << "=== FASE 1: ESCANEANDO TOKENS ===" << endl;
    Scanner scanner1(input.c_str());
    
    int scanner_result = ejecutar_scanner(&scanner1, argv[1]);
    
    if (scanner_result != 0) {
        cerr << "Error en scanner" << endl;
        cout << "Error léxico. Ver " << tokensFilename << endl;
        return 1;
    }
    
    cerr << "Tokens guardados en: " << tokensFilename << endl;

    // ==========================================
    // FASE 2: PARSING
    // ==========================================
    cerr << "=== FASE 2: PARSEANDO ===" << endl;
    
    Scanner scanner2(input.c_str());
    Parser parser(&scanner2);
    Program* program = nullptr;
    
    try {
        program = parser.parseProgram();
        cerr << "Parser exitoso" << endl;
    } 
    catch (const exception& e) {
        cerr << "Error en parser: " << e.what() << endl;
        
        // Guardar error en archivo
        ofstream errorFile(errorFilename);
        if (errorFile.is_open()) {
            errorFile << "=== ERROR DE PARSEO ===" << endl;
            errorFile << "Archivo: " << argv[1] << endl;
            errorFile << "Error: " << e.what() << endl;
            errorFile << "\nToken actual: ";
            
            Token* currentToken = parser.getCurrentToken();
            if (currentToken) {
                errorFile << "\"" << currentToken->text << "\" (tipo: " << currentToken->type << ")" << endl;
            } else {
                errorFile << "NULL" << endl;
            }
            errorFile.close();
            cout << "Error de parseo guardado en: " << errorFilename << endl;
        }
        
        return 1;
    }
    
    if (!program) {
        cerr << "Error: el parser devolvió nullptr" << endl;
        return 1;
    }

    // ==========================================
    // FASE 3: GENERACIÓN DE CÓDIGO
    // ==========================================
    cerr << "=== FASE 3: GENERANDO CÓDIGO ===" << endl;
    
    ofstream outfile(asmFilename);
    if (!outfile.is_open()) {
        cerr << "Error al crear el archivo de salida: " << asmFilename << endl;
        return 1;
    }

    try {
        GenCodeVisitor codigo(outfile);
        codigo.generar(program);
        outfile.close();
        
        cout << "Código ensamblador generado en: " << asmFilename << endl;
        cerr << "=== PROCESO EXITOSO ===" << endl;
    }
    catch (const exception& e) {
        cerr << "Error en generación de código: " << e.what() << endl;
        outfile.close();
        
        // Guardar error
        ofstream errorFile(errorFilename);
        if (errorFile.is_open()) {
            errorFile << "=== ERROR EN GENERACIÓN DE CÓDIGO ===" << endl;
            errorFile << "Archivo: " << argv[1] << endl;
            errorFile << "Error: " << e.what() << endl;
            errorFile.close();
        }
        
        return 1;
    }
    

    return 0;
}