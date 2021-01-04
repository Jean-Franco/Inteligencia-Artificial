#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
using namespace std;


struct paciente{
    int id;
    int categoria;          //1 = urgente ; 2 = paliativo ; 3 = radical
    int tiempo;             //tiempo de espera
    int atendido;           //si fue atendido el paciente, 0 si no
    int sesiones;           //cantidad de sesiones
    int movido;             //para el movimiento en SA y que no se repita por el random, 1 si se movio, 0 si no
};

struct maquina
{
    int id;
    int disponibilidad;         //1 = disponible ; 0 = no
};

struct doctor                   //turnos de los doctores
{
    int id;
    int disponibilidad;
    string horario;
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

// int verificarHorario(vector<vector <int>>& matrizdoctor, vector<vector <int>>& matrizmaquina, vector<doctor>& doctors){
//     int i, j, k;
//     for(i=0; i < matrizdoctor.size(); i++){
//         for(j=0; j < matrizdoctor[i].size(); j++){
//             if(matrizdoctor[i][j] != 0){
//                 for(k=0; k < doctors.size() ; k++){
//                     if(matrizdoctor[i][j] == doctors[k].id){
//                         if()
//                     }
//                 }
//             }
//         }
//     }
//     return 0;
// }

void CambiarEstados(vector<paciente>& pacientes, vector<doctor>& doctors, vector<maquina>& maquinas, int idPaciente, int idDoctor, int idMaquina){
    pacientes[idPaciente].atendido = 1;
    pacientes[idPaciente].sesiones++;
    doctors[idDoctor].disponibilidad = 0;
    maquinas[idMaquina].disponibilidad = 0;
}

void Movimiento(vector<vector <int>>& matrizdoctor, vector<vector <int>>& matrizmaquina, vector<paciente>& pacientes, vector<doctor>& doctors, int random1, int random2){  //movimiento para generar vecinos que consta de realizar un swap entre pacientes
    int i, j, k, l, temp1, temp2;
    for(i=0; i < matrizdoctor.size(); i++){
        if (random1 == i){
            for(j=0; j < matrizdoctor.size(); j++){
                if(random2 == j){
                    for(k=0; k < matrizdoctor[i].size(); k++){
                        if(matrizdoctor[i][k] != 0){
                            for(l=0; l < matrizdoctor[j].size(); l++){
                                if(matrizdoctor[j][l] != 0){                                   
                                    if(pacientes[i].movido == 0 && pacientes[j].movido == 0){
                                        temp1 = matrizdoctor[i][k];
                                        matrizdoctor[i][k] = matrizdoctor[j][k];
                                        matrizdoctor[j][k] = temp1;
                                        temp2 = matrizdoctor[i][l];
                                        matrizdoctor[i][l] = matrizdoctor[j][l];
                                        matrizdoctor[j][l] = temp2;
                                        pacientes[i].movido = 1;
                                        pacientes[j].movido = 1;
                                        // if (verificarHorario(matrizdoctor, matrizmaquina, doctors) == 1){
                                        //     return "movimiento factible";
                                        // }
                                        // else{
                                        //     return "movimiento infactible";
                                        // }
                                    }
                                }
                            }
                        }
                    }
                }
            }            
        }
    }
}


void PlanificacionSA(vector<vector <int>>& matrizdoctor, vector<vector <int>>& matrizmaquina, vector<paciente>& pacientes, vector<doctor>& doctors){
    int paciente1, paciente2, i, j, k, l, Temperatura, temp;
    srand (1);
    paciente1 = rand() % matrizdoctor.size();
    paciente2 = rand() % matrizdoctor.size();
    Movimiento(matrizdoctor, matrizmaquina, pacientes, doctors, paciente1, paciente2);
}

int calcular_tiempos(vector<vector <int>>& matrizdoctor){       //dada una matriz, le calcula el tiempo total de espera
    int tiempototal=0, i, j;
    for (i=0; i < matrizdoctor.size(); i++){
        for (j = 0; j < matrizdoctor[i].size(); j++){
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
                                }
                                if(doctors[doctor].horario[0] == '2' && dias == 0 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[0] == '3' && dias == 0){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[2] == '1' && dias == 1 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[2] == '2' && dias == 1 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[2] == '3' && dias == 1){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[4] == '1' && dias == 2 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[4] == '2' && dias == 2 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[4] == '3' && dias == 2){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[6] == '1' && dias == 3 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[6] == '2' && dias == 3 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[6] == '3' && dias == 3){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[8] == '1' && dias == 4 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[8] == '2' && dias == 4 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[8] == '3' && dias == 4){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                            }
                            
                            if(pacientes[paciente].categoria == 2 && pacientes[paciente].tiempo >= 2 && pacientes[paciente].atendido == 0 && doctors[doctor].disponibilidad == 1 && maquinas[maquina].disponibilidad == 1){
                                if(doctors[doctor].horario[0] == '1' && dias == 0 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[0] == '2' && dias == 0 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[0] == '3' && dias == 0){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[2] == '1' && dias == 1 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[2] == '2' && dias == 1 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[2] == '3' && dias == 1){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[4] == '1' && dias == 2 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[4] == '2' && dias == 2 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[4] == '3' && dias == 2){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[6] == '1' && dias == 3 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[6] == '2' && dias == 3 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[6] == '3' && dias == 3){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[8] == '1' && dias == 4 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[8] == '2' && dias == 4 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[8] == '3' && dias == 4){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                            }

                            if(pacientes[paciente].categoria == 3 && pacientes[paciente].tiempo >= 14 && pacientes[paciente].atendido == 0 && doctors[doctor].disponibilidad == 1 && maquinas[maquina].disponibilidad == 1 && dias != 4){
                                if(doctors[doctor].horario[0] == '1' && dias == 0 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[0] == '2' && dias == 0 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[0] == '3' && dias == 0){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[2] == '1' && dias == 1 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[2] == '2' && dias == 1 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[2] == '3' && dias == 1){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[4] == '1' && dias == 2 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[4] == '2' && dias == 2 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[4] == '3' && dias == 2){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[6] == '1' && dias == 3 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[6] == '2' && dias == 3 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[6] == '3' && dias == 3){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[8] == '1' && dias == 4 && bloques < 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[8] == '2' && dias == 4 && bloques >= 8){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
                                }
                                if(doctors[doctor].horario[8] == '3' && dias == 4){
                                    matrizdoctor[paciente][bloques + (16*pacientes[paciente].tiempo)] = doctors[doctor].id;
                                    matrizmaquina[paciente][bloques + (16*pacientes[paciente].tiempo)] = maquinas[maquina].id;
                                    CambiarEstados(pacientes, doctors, maquinas, paciente, doctor, maquina);
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
            for(i=0; i<pacientes.size(); i++){
                if (pacientes[i].atendido == 0){
                    pacientes[i].tiempo++;
                }
            }
        }
        for(i=0; i < pacientes.size(); i++){
            if(pacientes[i].atendido == 0){
                pacientes[i].tiempo += 2;
            }
        }
    }
    calcular_tiempos(matrizdoctor);
    //PlanificacionSA(matrizdoctor, matrizmaquina, pacientes, doctors);
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
    std::ifstream archivo("Caso3");
    vector<paciente>pacientes;
    vector<maquina>maquinas;
    vector<doctor>doctors;
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
    PlanificacionGreedy(pacientes, maquinas, doctors);
    archivo.close();
    return 0;
}