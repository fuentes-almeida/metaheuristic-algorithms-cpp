/*********************************************************************************************
 * AUTHORS
 *   Carlos Segura González
 *
 * DESCRIPTION
 * 	Clase que sirve para integrar objetivos auxiliares
 *********************************************************************************************/

#ifndef MULTIOBJECTIVIZATION_H 
#define MULTIOBJECTIVIZATION_H

using namespace std;

#include <vector>
#include <string>
#include "Individual.h"

class Individual;

class MultiObjectivization {
	public:
		virtual void convertToMultiObjective(vector<Individual *> *parents, vector<Individual *> *reference, int ordObj, int numGeneration, double completedRatio) = 0; 
		virtual int getOptDirection() = 0;
};

#endif
