#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main(){
    std::ifstream archivo("caso1");
    vector<string>doctores;
    string linea, bloques, ultima;
    stringstream linea1, linea2;
    int i;
    int nDoctores, nMaquinas, nPacientes, nRadicales, nPaliativos, nUrgentes;
    if(!archivo) 
    {
        cout<<"Error opening output file"<<endl;
        system("pause");
        return -1;
    }
    getline(archivo,linea);
    linea1 <<  linea;
    while (linea1>>nMaquinas>>nDoctores>>nPacientes)
    {
        cout << nMaquinas << endl;
        cout << nDoctores << endl;
        cout << nPacientes << endl;
        
    }
    for (i = 0; i < nDoctores ; i++){
        getline(archivo, bloques);
        doctores.push_back(bloques);
    }
    getline(archivo,ultima);
    linea2 <<  ultima;
    while (linea2>>nRadicales>>nPaliativos>>nUrgentes)
    {
        cout << nRadicales << endl;
        cout << nPaliativos << endl;
        cout << nUrgentes << endl;
        
    }
    archivo.close();
    return 0;
}