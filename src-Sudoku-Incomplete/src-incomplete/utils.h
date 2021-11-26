/*********************************************************************************
 * Autor
 *   Carlos Segura Gonzalez
 * Descripcion
 *   Funciones auxiliares usadas en varias partes del codigo: calculo de no 
 *   dominados, generacion de numeros aleatorios y carga de parametros
 ********************************************************************************/

#ifndef _UTILS_H_
#define _UTILS_H_

#include <map>
#include <string>
#include <iostream>
#include <stdlib.h>

#include "Individual.h"
using namespace std;

#define FIRST_DOMINATES -1
#define SECOND_DOMINATES 1
#define NON_DOMINANCES_EQUALS 2
#define NON_DOMINANCES_NOT_EQUALS 3

template <typename T>
void readParameter(map<string,string> &parameters, const string &parameter, T &value){
	if (parameters.count(parameter) == 0){
		cerr << "Parametro " << parameter << " no definido" << endl;
		exit(-1);
	}
	value = atof(parameters[parameter].c_str());
	parameters.erase(parameter);
}

void readParameter(map<string,string> &parameters, const string &parameter, string &value);
int generateRandom(const int minValue, const int maxValue);
int dominanceTest (const Individual *ind1, const Individual *ind2);
void markNonDominatedNoRep(const vector<Individual*> &pop, vector<int> &nonDominatedIndexes);
double generateRandomDouble0_Max(double maxValue);
int getRandomInteger0_N(int n);
#endif
