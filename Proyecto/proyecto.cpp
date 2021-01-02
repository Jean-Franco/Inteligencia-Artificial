#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
using namespace std;

struct paciente{
    int id;
    int categoria;          //1 = urgente ; 2 = paliativo ; 3 = radical
    int tiempo;             //tiempo de espera

};

struct maquina
{
    int id;
    int disponibilidad;         //1 = disponible ; 0 = no
};

struct doctor                   //turnos de los doctores
{
    int id;
    string lunes;
    string martes;
    string miercoles;
    string jueves;
    string viernes;
};

void AgregarPacientes(vector<paciente>& pacientes, int cantidad, int urgente, int paliativo, int radical){        //Retorna un vector con los pacientes y sus carac inicializados
    int i, j, k;
    for (i = 0; i < cantidad ; i++ ){
        pacientes.push_back(paciente());
        pacientes[i].id = i+1;
        pacientes[i].tiempo = 0;
    }
    for (i = 0; i < urgente ; i++){     //10
        pacientes[i].categoria = 1;
    }
    for (j = (0 + i); j < paliativo+urgente ; j++){         //3
        pacientes[j].categoria = 2;
    }
    for (k = (0 + j); k < radical+paliativo+urgente ; k++){           //2
        pacientes[k].categoria = 3;
    }
}

void AgregarMaquinas(vector<maquina>& maquinas, int cantidad){
    int i;
    for (i=0; i < cantidad; i++){
        maquinas.push_back(maquina());  
        maquinas[i].id = i+1;
        maquinas[i].disponibilidad = 1;
    }
}


int main(){
    std::ifstream archivo("Caso1");
    vector<paciente>pacientes;
    vector<string>doctores;
    vector<maquina>maquinas;
    vector<doctor>doctors;
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
    linea1>>nMaquinas>>nDoctores>>nPacientes;
        
    
    for (i = 0; i < nDoctores ; i++){           //Agregar docs y sus turnos x dia
        getline(archivo, bloques);
        doctors.push_back(doctor());
        doctors[i].id = i+1;
        doctors[i].lunes = bloques[0];
        doctors[i].martes = bloques[2];
        doctors[i].miercoles = bloques[4];
        doctors[i].jueves = bloques[6];
        doctors[i].viernes = bloques[8];

    }
    getline(archivo,ultima);
    linea2 <<  ultima;
    linea2>>nRadicales>>nPaliativos>>nUrgentes;
    AgregarPacientes(pacientes, nPacientes, nUrgentes, nPaliativos, nRadicales);
    AgregarMaquinas(maquinas, nMaquinas);
    for (i=0; i < nPacientes; i++){
        cout << pacientes[i].id << "su tiempo es: " << pacientes[i].tiempo << "y su categoria es: " << pacientes[i].categoria << endl;
    }
    for (i=0; i < nDoctores; i++){
        cout << doctors[i].id << "su horario es: " << doctors[i].lunes << " " << doctors[i].martes << " " << doctors[i].miercoles << " " << doctors[i].jueves << " " << doctors[i].viernes << endl;
    }
    for (i=0; i < nMaquinas; i++){
        cout << maquinas[i].id << "su disponibilidad es: " << maquinas[i].disponibilidad << endl;
    }


    archivo.close();
    return 0;
}