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
    int atendido;           //si fue atendido el paciente, 0 si no
    int sesiones;           //cantidad de sesiones
};

struct maquina
{
    int id;
    int disponibilidad;         //1 = disponible ; 0 = no
    int no;
};

struct doctor                   //turnos de los doctores
{
    int id;
    int disponibilidad;
    string horario;
    int no;
};

void AgregarPacientes(vector<paciente>& pacientes, int cantidad, int urgente, int paliativo, int radical){        //Retorna un vector con los pacientes y sus carac inicializados
    int i, j, k;
    for (i = 0; i < cantidad ; i++ ){
        pacientes.push_back(paciente());
        pacientes[i].id = i+1;
        pacientes[i].tiempo = 0;
        pacientes[i].atendido = 0;
        pacientes[i].sesiones = 0;
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

void CambiarEstados(vector<paciente>& pacientes, vector<doctor>& doctors, vector<maquina>& maquinas, int idPaciente, int idDoctor, int idMaquina){
    pacientes[idPaciente].atendido = 1;
    //pacientes[idPaciente].tiempo = 0;
    pacientes[idPaciente].sesiones++;
    doctors[idDoctor].disponibilidad = 0;
    maquinas[idMaquina].disponibilidad = 0;
}

void PlanificacionSA(vector<vector <int>>& matrizdoctor, vector<vector <int>>& matrizmaquina){
    int movimiento, i, j;
    //for
}

int calcular_tiempos(vector<vector <int>>& matrizdoctor){
    int tiempototal=0, i, j;
    for (i=0; i < matrizdoctor.size(); i++){
        for (j = 0; j < matrizdoctor[i].size(); j++)
        {
            if(matrizdoctor[i][j] != 0){
                break;
            }
            if ((j+1)%16 == 0){
                tiempototal += 1;
            }
        }   
    }
    cout << "el conteo termina en " << tiempototal << " dias esperados" << endl;
}

void PlanificacionGreedy(vector<paciente>& pacientes, vector<maquina>& maquinas, vector<doctor>& doctors){
    vector<vector<int>>matrizdoctor;
    vector<vector<int>>matrizmaquina;
    ofstream tabla1("paciente-doctor.txt");
    ofstream tabla2("paciente-maquina.txt");
    int semanas,dias,bloques,i,j,paciente,maquina,doctor,flag,tiempototal;
    for(paciente=0; paciente < pacientes.size(); paciente++){
        vector<int>temp1(320,0);
        vector<int>temp2(320,0);
        matrizdoctor.push_back(temp1);
        matrizmaquina.push_back(temp2);
    }
    tiempototal = 0;
    for (semanas=0; semanas < 4; semanas++){
        for(dias=0; dias < 5; dias++){
            for(bloques=0; bloques < 16; bloques++){
                for(paciente=0; paciente < pacientes.size(); paciente++){
                    for(doctor=0; doctor < doctors.size(); doctor++){
                        for(maquina=0; maquina < maquinas.size(); maquina++){
                            if(pacientes[paciente].categoria == 1 && pacientes[paciente].tiempo >= 1 && pacientes[paciente].atendido == 0 && doctors[doctor].disponibilidad == 1 && maquinas[maquina].disponibilidad == 1){
                                if(doctors[doctor].horario[0] == '1' && dias == 0 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[0] == '2' && dias == 0 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[0] == '3' && dias == 0){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[2] == '1' && dias == 1 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[2] == '2' && dias == 1 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[2] == '3' && dias == 1){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[4] == '1' && dias == 2 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[4] == '2' && dias == 2 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[4] == '3' && dias == 2){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[6] == '1' && dias == 3 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[6] == '2' && dias == 3 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[6] == '3' && dias == 3){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[8] == '1' && dias == 4 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[8] == '2' && dias == 4 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[8] == '3' && dias == 4){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                            }
                            
                            if(pacientes[paciente].categoria == 2 && pacientes[paciente].tiempo >= 2 && pacientes[paciente].atendido == 0 && doctors[doctor].disponibilidad == 1 && maquinas[maquina].disponibilidad == 1){
                                if(doctors[doctor].horario[0] == '1' && dias == 0 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[0] == '2' && dias == 0 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[0] == '3' && dias == 0){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[2] == '1' && dias == 1 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[2] == '2' && dias == 1 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[2] == '3' && dias == 1){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[4] == '1' && dias == 2 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[4] == '2' && dias == 2 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[4] == '3' && dias == 2){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[6] == '1' && dias == 3 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[6] == '2' && dias == 3 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[6] == '3' && dias == 3){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[8] == '1' && dias == 4 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[8] == '2' && dias == 4 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[8] == '3' && dias == 4){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                            }

                            if(pacientes[paciente].categoria == 3 && pacientes[paciente].tiempo >= 14 && pacientes[paciente].atendido == 0 && doctors[doctor].disponibilidad == 1 && maquinas[maquina].disponibilidad == 1){
                                if(doctors[doctor].horario[0] == '1' && dias == 0 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[0] == '2' && dias == 0 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[0] == '3' && dias == 0){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[2] == '1' && dias == 1 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[2] == '2' && dias == 1 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[2] == '3' && dias == 1){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[4] == '1' && dias == 2 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[4] == '2' && dias == 2 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[4] == '3' && dias == 2){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[6] == '1' && dias == 3 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[6] == '2' && dias == 3 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[6] == '3' && dias == 3){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[8] == '1' && dias == 4 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[8] == '2' && dias == 4 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                                if(doctors[doctor].horario[8] == '3' && dias == 4){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                    cout << "entre if 1" << endl;
                                }
                            }
                        }
                    }
                }
                for (doctor=0; doctor < doctors.size(); doctor++){
                    doctors[doctor].disponibilidad = 1;
                }
                for (maquina=0; maquina < maquinas.size(); maquina++){
                    maquinas[maquina].disponibilidad = 1;
                }
            }
            cout << "dia terminado" << endl;
            for(i=0; i<pacientes.size(); i++){
                if (pacientes[i].atendido == 0){
                    cout << "al paciente " << i << "le agregue 1 dia de espera" << endl;
                    pacientes[i].tiempo++;
                }
            }
        }
        cout << "semana terminada, +2 a dias de espera" << endl;
        for(i=0; i < pacientes.size(); i++){
            if(pacientes[i].atendido == 0){
                cout << "al paciente " << i << "le agregue 2 dias x el finde" << endl;
                pacientes[i].tiempo += 2;
            }
        }
    }
    calcular_tiempos(matrizdoctor);
    for (i=0; i < matrizdoctor.size(); i++){
        for(j=0; j < matrizdoctor[i].size(); j++){
            tabla1 << matrizdoctor[i][j] << " ";                //verificar si el " " me destruye el orden de la tabla al generar los vecinos
        }
        tabla1 << endl;
    }
    for (i=0; i < matrizmaquina.size(); i++){
        for(j=0; j < matrizmaquina[i].size(); j++){
            tabla2 << matrizmaquina[i][j] << " ";
        }
        tabla2 << endl;
    }
    tabla1.close();
    tabla2.close();
}




int main(){
    std::ifstream archivo("Caso1");
    vector<paciente>pacientes;
    vector<string>doctores;
    vector<maquina>maquinas;
    vector<doctor>doctors;
    vector<vector<int>>matriz;
    string linea, bloques, ultima;
    stringstream linea1, linea2;
    int i,j;
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
        doctors[i].horario = bloques;
        doctors[i].disponibilidad = 1;
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
        cout << doctors[i].id << "su horario es: " << doctors[i].horario << endl;
    }
    for (i=0; i < nMaquinas; i++){
        cout << maquinas[i].id << "su disponibilidad es: " << maquinas[i].disponibilidad << endl;
    }
    PlanificacionGreedy(pacientes, maquinas, doctors);
    // for (i =0; i < nPacientes; i++){
    //     vector<int>temp;
    //     for (j=0; j < 80; j++){
    //         temp.push_back(i);
    //     }
    //     matriz.push_back(temp);
    // }

    // ofstream tabla1("paciente-doctor.txt");
    // ofstream tabla2("paciente-maquina.txt");

    // for (i=0; i < matriz.size(); i++){
    //     for (j=0; j < matriz[i].size(); j++){
    //         tabla1 << matriz[i][j] << "  ";
    //     }
    //     tabla1 << endl;
    // }



    // tabla1.close();
    // tabla2.close();
    archivo.close();
    return 0;
}