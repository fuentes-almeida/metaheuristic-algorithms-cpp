/***********************************************************************
* Autor: Carlos Segura González
************************************************************************/

#include "Sudoku.h"
#include "utils.h"
#include <iostream>
#include <stdlib.h>
#include <sys/time.h>
#include <algorithm>

Sudoku_Problem* Sudoku::instance = NULL;
vector< pair < pair<int, int>, pair<int, int> > > neighbours;//Posibles vecinos

//Creacion de una solucion aleatoria
void Sudoku::restart(){
	for (int i = 0; i < 9; i++){
		data[i] = instance->getDimensionChoices(i);
		random_shuffle(data[i].begin(), data[i].end());
	}
	evaluate();
	localSearch();
}

//Evaluar una solucion y fijar el valor con setObj
//Si no hay penalizacion, el coste se fija a -1 como indicador de que se llego al "optimo"
void Sudoku::evaluate(){
	int penalty = instance->getPenalizations(data);
	setObj(0, (penalty == 0)?-1:penalty);
}

//Clonar una solucion: copiar las variables		
Individual *Sudoku::clone() const{
	Sudoku *newInd = new Sudoku();
	for (int i = 0; i < 9; i++){
		newInd->data[i] = data[i];
	}
	return newInd;
}

//Imprimir una solucion en el ostream out
void Sudoku::print(ostream &out) const{
	instance->print(out, data);
	out << getObj(0) << endl;
}

//Inicializacion: carga de la instancia, y creacion de un vector
//con la lista de todos los intercambios que se pueden hacer para crear vecinos
bool Sudoku::init(const vector<string> &params){
	if ((params.size() % 3) != 0){
		cerr << "Error en los parametros de Sudoku. Deben ser multiplos de 3" << endl;
		return false;
	}
	map<string, string> parameters;
	for (unsigned int i = 0; i < params.size(); i+=3){
		parameters[params[i]] = params[i+2];
		if (params[i+1] != "="){
			cerr << "Error en los parametros de Sudoku. Deben estar formados por grupos de la forma parametro = valor" << endl;
			return false;
		}
	}

	if (instance == NULL){
		string extraPenaltyStr;
		readParameter(parameters, "ExtraPenalty", extraPenaltyStr);
		string instance_file;
		readParameter(parameters, "Instance_file", instance_file);
		instance = new Sudoku_Problem();
		instance->initiallize(extraPenaltyStr, instance_file);
		for (int i = 0; i < 9; i++){
			for (int j = 0; j < instance->getDimensionChoicesSize(i); j++){
				for (int k = j + 1; k < instance->getDimensionChoicesSize(i); k++){
					pair<int, int> first = pair<int, int>(i, j);
					pair<int, int> second = pair<int, int>(i, k);
					neighbours.push_back(pair < pair<int, int>, pair<int, int> >(first, second));
				}
			}
		}
	}
	setNumberOfObj(1);
	return true;
}

//Mutar una solucion
//Cada valor se muta con probabilidad pm. En caso de mutar, simplemente se hace un
//swap con cualquier elemento de su permutacion elegido de forma aleatoria
void Sudoku::mutation(double pm){
	//TODO: recorrer todas las variables del individuo y con probabilidad pm intercambiarla por
	//otra aleatoria de su bloque. Puede generar un numero real aleatorio en [0, 1] con
	//generateRandomDouble0_Max y un entero entre [0, N] con getRandomInteger0_N(N).
}
	
//Cruzar al individuo actual con el individuo i2
//Se hace mutacion uniforme, en la cual se intercambian bloques enteros
void Sudoku::crossover(Individual *i2){
	//TODO: implementar un cruce uniforme, es decir, intercambia los bloques enteros entre this e i2
	//con probabilidad 0.5
}

//Busqueda local
//Se realiza una busqueda local completa, cambiandonos a cada vecinos que encontremos
//que es mejor que el actual
//Para cada vecinos, hacemos el cambio, evaluamos, y si no es mejor deshacemos
//Esto lo repetimos hasta que no existan vecinos mejores que el actual
void Sudoku::localSearch(){
	int currentPenalizations = instance->getPenalizations(data);
	bool changed = true;
	while(changed){
		changed = false;
		for (int i = (int)(neighbours.size()) -1; i >= 0; i--){
			//TODO: elegir cualquiera de los vecinos (cada vecino indica que parejas hay que cambiar),
			//cambiarlas, y aceptarlo si mejora. Si se acepta alguno hay que poner el changed = true
			//El vecino utilizado lo odemos colocar al final
			//Para generar numero aleatorios, en utils.h esta la funcion getRandomInteger0_N
		}
	}
	evaluate();
}

//Calculo de distancia entre individuos: simplemente es un conteo de cuantas 
//casillas son diferentes
double Sudoku::getDistance(Individual &ind2) {
	//TODO contar el número de variables que son diferentes en los 2 individuos.
	//Puede acceder a ind2 como un objetivo de tipo Sudoku haciendo typecase; por ejemplo
	//para acceder a las variables j del bloque i: ((Sudoku*)(&ind2))->data[i][j])
}
