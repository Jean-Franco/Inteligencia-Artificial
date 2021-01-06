#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <math.h>
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
        pacientes[i].movido = 0;
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

void AgregarMaquinas(vector<maquina>& maquinas, int cantidad){  //retorna un vector con las maquinas y sus carac inicializados
    int i;
    for (i=0; i < cantidad; i++){
        maquinas.push_back(maquina());  
        maquinas[i].id = i+1;
        maquinas[i].disponibilidad = 1;
    }
}

int verificarHorario(vector<vector <int>>& matrizdoctor, vector<paciente>& pacientes){      //cuando se genera un vecino, verifica si este es factible, retorna 1 si lo es, 0 en caso contrario
    int i, j, k, contadordias;
    for(i=0; i < matrizdoctor.size(); i++){
        contadordias = 0;
        for(j=0; j < matrizdoctor[i].size(); j++){
            if(j%16 == 0 && j!= 0){
                contadordias++;
            }
            if(matrizdoctor[i][j] != 0){
                if(pacientes[i].categoria == 3 && j >= 63 && j <= 79 && contadordias >= 14 && contadordias < 28){
                    return 0;
                }
                if(pacientes[i].categoria == 3 && j >= 143 && j <= 159 && contadordias >= 14 && contadordias < 28){
                    return 0;
                }
                if(pacientes[i].categoria == 3 && j >= 223 && j <= 239 && contadordias >= 14 && contadordias < 28){
                    return 0;
                }
                if(pacientes[i].categoria == 3 && j >= 303 && j <= 319 && contadordias >= 14 && contadordias < 28){
                    return 0;
                }
                if(pacientes[i].categoria == 2 && contadordias >= 2 && contadordias < 14){
                    return 0;
                }
                if(pacientes[i].categoria == 1 && contadordias >= 1 && contadordias < 2){
                    return 0;
                }
            }
        }
    }
    return 1;
}

//Automatizacion del cambio del estado de los objetos involucrados, una vez se agrega un paciente, doctor y maquina a la planificacion
void CambiarEstados(vector<paciente>& pacientes, vector<doctor>& doctors, vector<maquina>& maquinas, int idPaciente, int idDoctor, int idMaquina){
    pacientes[idPaciente].atendido = 1;
    pacientes[idPaciente].sesiones++;
    doctors[idDoctor].disponibilidad = 0;
    maquinas[idMaquina].disponibilidad = 0;
}

//movimiento para generar vecinos que consta de realizar un swap entre pacientes de la misma semana y del mismo bloque, para ahorrar miles de verificaciones de horarios de los doctores
int Movimiento(vector<vector <int>>& matrizdoctor, vector<vector <int>>& matrizmaquina, vector<paciente>& pacientes){  
    int i, j, k, l, temp1, temp2, temp3, temp4, semanas1, dias1, bloques1, dias2, bloques2, cont;  
    cont = 0;
    semanas1 = 1;
    for(i=0; i < matrizdoctor.size() ; i++){
        bloques1 = 0;
        dias1 = 0;
        semanas1 = 1;
        for (j=0; j < matrizdoctor[i].size(); j++){
            if(j%16 == 0 && j != 0){
                dias1++;
                bloques1 = 0;
                if(dias1 == 4){
                    semanas1++;
                    dias1 = 0;
                }
            }
            if(matrizdoctor[i][j] != 0){
                for(k=(i+1); k < matrizdoctor.size() ; k++){
                    dias2 = 0;
                    bloques2 = 0;
                    if(dias1 != 4){
                        for(l=(j+16); l < matrizdoctor[k].size(); l++){
                            if(l%16 == 0 && l != 0){
                                bloques2 = 0;
                            }
                            if(matrizdoctor[k][l] != 0 && l < 80*semanas1 && bloques1 == bloques2 && pacientes[i].movido == 0 && pacientes[k].movido == 0){
                                temp1 = matrizdoctor[i][j];
                                matrizdoctor[i][j] = matrizdoctor[k][j];
                                matrizdoctor[k][j] = temp1;
                                temp2 = matrizdoctor[i][l];
                                matrizdoctor[i][l] = matrizdoctor[k][l];
                                matrizdoctor[k][l] = temp2;
                                temp1 = matrizmaquina[i][j];
                                matrizmaquina[i][j] = matrizmaquina[k][j];
                                matrizmaquina[k][j] = temp1;
                                temp2 = matrizmaquina[i][l];
                                matrizmaquina[i][l] = matrizmaquina[k][l];
                                matrizmaquina[k][l] = temp2;
                                pacientes[i].movido = 1;
                                pacientes[k].movido = 1;
                                return 1;
                            }
                            bloques2++;
                        }
                    }
                    else
                    {
                        break;
                    }               
                }
            }
            bloques1++;
        }
    }
}

//funcion que genera todos los vecinos de la primera solucion greedy y la agrega al conjunto de planificaciones si este es factible
void generar_vecinos(vector<vector <int>>& matrizdoctor, vector<vector <int>>& matrizmaquina, vector<paciente>& pacientes, vector<vector<vector <int>>>& planificacionesD, vector<vector<vector <int>>> planificacionesM){
    int paciente1, paciente2, i, j, k, l, Temperatura;
    for(i=0; i < pacientes.size() ; i++){
        if(pacientes[i].movido == 0){
            Movimiento(matrizdoctor, matrizmaquina, pacientes);
            if (verificarHorario(matrizdoctor, pacientes) == 1){
                planificacionesD.push_back(matrizdoctor);
                planificacionesM.push_back(matrizmaquina);
            }
        }
    }   
}

//funcion de evaluacion que calcula los dias de espera total de una planificacion
int calcular_tiempos(vector<vector <int>>& matrizdoctor){
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
    return tiempototal;
}

//genera la solucion inicial greedy verificando las condiciones minimas de los pacientes en conjunto con los horarios de los doctores y luego la trabaja con Simulated Annealing
void PlanificacionGreedy(vector<paciente>& pacientes, vector<maquina>& maquinas, vector<doctor>& doctors){
    vector<vector<int>>matrizdoctor;
    vector<vector<int>>matrizmaquina;
    vector<vector<vector <int>>> planificacionesD;
    vector<vector<vector <int>>> planificacionesM;
    ofstream tabla1("paciente-doctor.txt");
    ofstream tabla2("paciente-maquina.txt");
    int semanas,dias,bloques,i,j,k,paciente,maquina,doctor;
    for(paciente=0; paciente < pacientes.size(); paciente++){
        vector<int>temp1(320,0);
        vector<int>temp2(320,0);
        matrizdoctor.push_back(temp1);
        matrizmaquina.push_back(temp2);
    }
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
    planificacionesD.push_back(matrizdoctor);
    planificacionesM.push_back(matrizmaquina);
    
        //Implementacion SA

    float temperatura = 10;
    float probabilidad;
    float alpha = 0.97;
    srand(time(NULL));
    int paso;
    vector<vector<vector <int>>> solucion_actual, mejor_solucion, solucion_candidata, solucion_actual_maquinas,mejor_solucion_maquinas, solucion_candidata_maquinas;
    solucion_actual.push_back(planificacionesD[0]);
    solucion_actual_maquinas.push_back(planificacionesM[0]);
    mejor_solucion.push_back(solucion_actual[0]);
    mejor_solucion_maquinas.push_back(solucion_actual_maquinas[0]);
    for (paso=0; paso < planificacionesD.size() ; paso++){
        generar_vecinos(matrizdoctor, matrizmaquina, pacientes, planificacionesD, planificacionesM);
        solucion_candidata.push_back(planificacionesD[paso]);
        solucion_candidata_maquinas.push_back(planificacionesM[paso]);
        if(calcular_tiempos(solucion_candidata[0]) < calcular_tiempos(solucion_actual[0]) && calcular_tiempos(solucion_candidata[0]) != 0){
            solucion_actual.pop_back();
            solucion_actual.push_back(solucion_candidata[0]);
            solucion_actual_maquinas.pop_back();
            solucion_actual_maquinas.push_back(solucion_candidata_maquinas[0]);
        }
        else{
            float random = rand()/RAND_MAX;
            probabilidad = exp(((calcular_tiempos(solucion_candidata[0]) - calcular_tiempos(solucion_actual[0]))/temperatura));
            if (random < probabilidad)
            {
                solucion_actual.pop_back();
                solucion_actual.push_back(solucion_candidata[0]);
                solucion_actual_maquinas.pop_back();
                solucion_actual_maquinas.push_back(solucion_candidata_maquinas[0]);
            }
        }
        if(calcular_tiempos(solucion_candidata[0]) < calcular_tiempos(solucion_actual[0]) && calcular_tiempos(solucion_candidata[0]) != 0){
            mejor_solucion.pop_back();
            mejor_solucion.push_back(solucion_candidata[0]);
            mejor_solucion_maquinas.pop_back();
            mejor_solucion_maquinas.push_back(solucion_candidata_maquinas[0]);
        }
        temperatura=alpha*temperatura;
    }
    cout << "La mejor planificacion consta de un tiempo total de: " << calcular_tiempos(mejor_solucion[0]) << " dias." << endl;
    for(i=0; i < mejor_solucion.size() ; i++){
        for (j=0; j < mejor_solucion[i].size(); j++){
            for(k=0; k < mejor_solucion[i][j].size(); k++){
                tabla1 << mejor_solucion[i][j][k] << " ";
            }
            tabla1 << endl;
        }
    }
    for (i=0; i < mejor_solucion_maquinas.size() ; i++){
        for (j=0; j < mejor_solucion_maquinas[i].size(); j++){
            for(k=0; k < mejor_solucion_maquinas[i][j].size(); k++){
                tabla2 << mejor_solucion_maquinas[i][j][k] << " ";
            }
            tabla2 << endl;
        }

    }

    tabla1.close();
    tabla2.close();
}

int main(int argc, char** argv){
    string file = argv[1];
    std::ifstream archivo(file);
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