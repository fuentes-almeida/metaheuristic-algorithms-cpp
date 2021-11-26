/********************************************************************************
 * Autor
 *    Carlos Segura Gonzalez
 * ******************************************************************************/

#include "utils.h"

#include <map>
#include <string>
#include <iostream>
#include <stdlib.h>
using namespace std;

void readParameter(map<string,string> &parameters, const string &parameter, string &value){
	if (parameters.count(parameter) == 0){
		cerr << "Parametro " << parameter << " no definido" << endl;
		exit(-1);
	}
	value = parameters[parameter];
	parameters.erase(parameter);
}

//Generar un entero [minValue, maxValue]
int generateRandom(const int minValue, const int maxValue){
	return (int) (((double)(maxValue + 1 - minValue)*rand()/(RAND_MAX+1.0))) + minValue;
}

int dominanceTest (const Individual *ind1, const Individual *ind2) {
	int flag      ; 
	int dominate1 ;
	int dominate2 ;
	dominate1 = 0 ;
	dominate2 = 0 ;
	
	for(unsigned int i = 0; i < ind1->getNumberOfObj(); i++) {
		if (ind1->getInternalOptDirection(i) == MAXIMIZE){ 
			if (ind1->getObj(i) > ind2->getObj(i)) 
				flag = -1 ;
			else if (ind1->getObj(i) < ind2->getObj(i)) 
				flag = 1 ;
			else 
				flag = 0 ;
		} else {                          // Objetivo a minimizar
			if (ind1->getObj(i) < ind2->getObj(i)) 
				flag = -1;
			else if (ind1->getObj(i) > ind2->getObj(i))
				flag = 1 ;
			else
				flag = 0 ;
		}
		
		if (flag == -1)
			dominate1 = 1 ;
		if (flag == 1)
			dominate2 = 1 ;
	}
    
  if ((dominate1 == 0) && (dominate2 == 0)){//iguales
    return NON_DOMINANCES_EQUALS;  // Non dominated solutions
  }
	if ((dominate1 == 1) && (dominate2 == 1)){//no iguales
		return NON_DOMINANCES_NOT_EQUALS;
	}
  if (dominate1 == 1) {
    return FIRST_DOMINATES; // solution1 dominates
  }
  return SECOND_DOMINATES;    // solution2 dominates
}

//Deteccion de conjunto de individuos no dominados
void markNonDominatedNoRep(const vector<Individual*> &pop, vector<int> &nonDominatedIndexes){
	nonDominatedIndexes.clear();
	vector<bool> dominated(pop.size(), false);
	for (unsigned int i = 0; i < pop.size(); i++){
		if (dominated[i])
			continue;
		for (unsigned int j = i + 1; j < pop.size(); j++){
			if (dominated[j])
				continue;
			int dominance = dominanceTest(pop[i], pop[j]);
			if (dominance == FIRST_DOMINATES){
				dominated[j] = true;
			} else if (dominance == SECOND_DOMINATES) {
				dominated[i] = true;
				break;
			} else if (dominance == NON_DOMINANCES_EQUALS){
				dominated[j] = true;
			}
		}
	}
	//Almacenamos los indices de los no dominados
	for (unsigned int i = 0; i < pop.size(); i++){
		if (dominated[i] == false){
			nonDominatedIndexes.push_back(i);
		}
	}
}

double generateRandomDouble0_Max(double maxValue){
	return (double)(rand()) / RAND_MAX * maxValue;
}

int getRandomInteger0_N(int n){
	return (int) ((n + 1.0)*rand()/(RAND_MAX+1.0));
}
