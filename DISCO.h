#pragma once
#include <string>
#include <filesystem>
#include <vector>
using std::vector;

bool existeCarpetaDisco();
int contarDigitos(int n);

class Disco {
public:
    int platos;
    int pistas;
    int sectores;
    int capSector;
    int sectoresPorBloque;
    int espacioTotal;
    int espaciocupado;

    Disco();
    void pedirDatos();
    void crearCarpeta(const char* ruta);
    void crearDisco();
    void recuperarDatosDisco();
    void borrarDisco();
    void mostrarArbol(const char* path, int nivel = 0);
    void mostrarInfo();
    void escribirSector(vector<char>&, int*);
    vector<char> leerSector(int*);
};