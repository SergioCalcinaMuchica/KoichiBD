#include <iostream>
#include "DISCO.h"
#include "MicroControlador.h"
#include "BufferManager.h"
using namespace std;
Disco disco;
string extraerBitmap(const vector<char>& datos) {
    int indice = 0;
    // Saltar la primera línea (cabecera)
    while (indice < datos.size() && datos[indice] != '\n') indice++;
    indice++;
    // Saltar la segunda línea (cabecera)
    while (indice < datos.size() && datos[indice] != '\n') indice++;
    indice++;
    // Extraer el bitmap hasta el siguiente salto de línea
    string bitmap;
    while (indice < datos.size() && datos[indice] != '\n') {
        bitmap += datos[indice];
        indice++;
    }
    return bitmap;
}

void actualizarBitmapEnDatos(vector<char>& datos, const string& nuevoBitmap) {
    int indice = 0;
    // Saltar la primera línea (cabecera)
    while (indice < datos.size() && datos[indice] != '\n') indice++;
    indice++;
    // Saltar la segunda línea (cabecera)
    while (indice < datos.size() && datos[indice] != '\n') indice++;
    indice++;
    // Ahora 'indice' apunta al inicio del bitmap
    for (size_t i = 0; i < nuevoBitmap.size() && (indice + i) < datos.size(); ++i) {
        datos[indice + i] = nuevoBitmap[i];
    }
}
/*
bool suficienteEspacio(int bloquecomp, BufferManager& buffer, string m){
    int tamBloque=(buffer.disco->sectoresPorBloque)*(buffer.disco->capSector);
    if(m.size()+buffer.Bloques[bloquecomp].size()>tamBloque){
        return false;
    }else{
        return true;
    }
}
*/
void insertaresquema(BufferManager& buffer) {
    string nombreEsquema;
    char variableFijo;
    int contadorAtributos = 0;
    cout << "Ingrese el nombre del esquema: ";
    cin >> nombreEsquema;
    cout<<"Fijo o variable? (f/v): ";
    cin>> variableFijo;
    cout<<"Cuantos atributos tendra el esquema?";
    cin>> contadorAtributos;

    string ingreso=nombreEsquema+"#"+variableFijo+"#"; //nombre del esquema + tipo de variable
    string aux;
    if(variableFijo=='f'){
        for(int i=0;i<contadorAtributos;i++){ //int,float,varchar
            cout<<"Ingrese nombre del atributo:";
            cin>>aux;
            ingreso = ingreso + aux + "#";
            cout<<"Ingrese tipo del atributo:";
            cin>>aux;
            ingreso = ingreso + aux + "#";
            cout<<"Ingrese cuantos caracteres(bytes) ocupa el atributo:"; //cantidad maxima de caracteres
            cin>>aux;
            ingreso = ingreso + aux + "@"; //fin de esquema
        }
        //revisar si hay bloque disponible en base al bitmap
        buffer.cargarBloque(0); //cargar bloque 0 que es el bitmap
        vector<char>& datos = buffer.Bloques[0];
        string bitmapt= extraerBitmap(datos);
        if(bitmapt.find('0') == string::npos){
            cout<<"TODOS BLOQUES LLENOS"<<endl;
            return;
        }else{
            int indice=bitmapt.find('0');
            bitmapt[indice]='1';
            actualizarBitmapEnDatos(datos,bitmapt);
            string lineaMeta = nombreEsquema + "#" + to_string(indice) + "\n";
            if(datos.size())
            datos.insert(datos.end(), lineaMeta.begin(), lineaMeta.end());
            buffer.escribirBloque(0);
        }
    }else{
        //logica para variable, aun no implementada
        return;
    }
}


int main() {
    int opc = 0;
    bool ejecutando = true;

    cout << "Bienvenido al sistema de gestion de discos\n";
    cout << "1) Crear disco\n";
    cout << "2) Continuar en disco\n";
    cout << "Seleccione una opcion: ";
    cin >> opc;

    switch(opc) {
        case 1:
            disco.borrarDisco();
            disco.pedirDatos();
            disco.crearDisco();
            break;
        case 2:
            disco.recuperarDatosDisco();
            break;
        default:
            cout << "Opcion invalida. Saliendo.\n";
            return 1;
    }
    MicroControlador microControlador = MicroControlador(&disco);
    BufferManager buffer = BufferManager(&disco, &microControlador);
    ejecutando = true;
    while (ejecutando) {
        cout << "\n1) Mostrar informacion del disco\n";
        cout << "2) Salir\n";
        cout << "3) Mostrar arbol del disco\n";
        cout << "4) Insertar esquema\n";
        cout << "5) Insertar datos\n";
        cout << "Seleccione una opcion: ";
        cin >> opc;
        switch (opc) {
            case 1:
                disco.mostrarInfo();
                break;
            case 2:
                ejecutando = false;
                break;
            case 3:
                disco.mostrarArbol("Disco");
                break;
            case 4:
                insertaresquema(buffer);
                break;
            default:
                cout << "Opcion invalida.\n";
        }
    }
    return 0;
}

//g++ main.cpp Microcontrolador.cpp DISCO.cpp BufferManager.cpp -o main