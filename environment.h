#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

template <typename T>
class Environment {
private:
    vector<unordered_map<string, T>> ribs;

    int search_rib(const string& var) const {
        for (int idx = static_cast<int>(ribs.size()) - 1; idx >= 0; --idx) {
            auto it = ribs[idx].find(var);
            if (it != ribs[idx].end())  // encontrado
                return idx;
        }
        return -1; // no encontrado
    }

public:
    Environment() = default;

    // Limpia completamente el entorno
    void clear() {
        ribs.clear();
    }

    // Agrega un nuevo nivel (scope)
    void add_level() {
        ribs.emplace_back();
    }

    // Agrega una variable con un valor inicial
    void add_var(const string& var, const T& value) {
        if (ribs.empty()) {
            cerr << "[Error] Environment sin niveles: no se pueden agregar variables.\n";
            exit(EXIT_FAILURE);
        }
        ribs.back()[var] = value;
    }

    // Agrega una variable con valor por defecto (solo si T es numérico o tiene constructor por defecto)
    void add_var(const string& var) {
        if (ribs.empty()) {
            cerr << "[Error] Environment sin niveles: no se pueden agregar variables.\n";
            exit(EXIT_FAILURE);
        }
        ribs.back()[var] = T(); // inicializa con valor por defecto
    }

    // Elimina el nivel más interno
    bool remove_level() {
        if (!ribs.empty()) {
            ribs.pop_back();
            return true;
        }
        return false;
    }

    // Actualiza el valor de una variable existente
    bool update(const string& x, const T& v) {
        int idx = search_rib(x);
        if (idx < 0) return false;
        ribs[idx][x] = v;
        return true;
    }

    // Verifica si una variable existe
    bool check(const string& x) const {
        return (search_rib(x) >= 0);
    }

    // Busca y devuelve el valor de una variable
    // Si no existe, devuelve un valor por defecto de T
    T lookup(const string& x) const {
        int idx = search_rib(x);
        if (idx < 0) {
            cerr << "[Advertencia] Variable no encontrada: " << x << endl;
            return T(); // valor por defecto
        }
        return ribs[idx].at(x);
    }

    // Busca y devuelve el valor en una referencia. Devuelve true si existe.
    bool lookup(const string& x, T& v) const {
        int idx = search_rib(x);
        if (idx < 0) return false;
        v = ribs[idx].at(x);
        return true;
    }
};

#endif // ENVIRONMENT_H