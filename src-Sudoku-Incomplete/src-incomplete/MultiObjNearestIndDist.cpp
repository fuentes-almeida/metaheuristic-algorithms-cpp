/***********************************************************************************
 * AUTHORS
 *   - Carlos Segura González
 ***********************************************************************************/


#include "MultiObjNearestIndDist.h"
#include "utils.h"

#include <map>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <limits.h>

using namespace std;

void MultiObjNearestIndDist::convertToMultiObjectiveWithRef(vector<Individual *> *population, vector<Individual *> *reference, int ordObj) {
	if (reference->size() == 1){//Solo esta el best en la pob. seleccionada
		//Almacenamos el objetivo como dato auxiliar para despues poder recuperarlo 
		for (unsigned int i = 0; i < population->size(); i++){
			(*population)[i]->setAuxDataSize(1);
			(*population)[i]->setAuxData(0, (*population)[i]->getObj(0));
			if (((*population)[i]->getObj(0) == 0) || ((*population)[i]->getObj(0) == DBL_MAX)){
				cerr << "Error interno: en obj contiene un 0 o DBL_MAX (llega penalizado)" << endl;
				exit(-1);
			}
		}
		//Calculamos el objetivo auxiliar 
		for (unsigned int i = 0; i < population->size(); i++){
			double actualDist = (*population)[i]->getDistance(*(*reference)[0]);
			(*population)[i]->setObj(ordObj, actualDist);
		}
	} else {
		//Actualizamos distancia considerando el ultimo elegido
 		for (unsigned int i = 0; i < population->size(); i++) {
			double minDist = (*population)[i]->getObj(ordObj);	
			unsigned int j = reference->size() - 1;
			double actualDist = (*population)[i]->getDistance(*(*reference)[j]);
			minDist = min(actualDist, minDist);
			(*population)[i]->setObj(ordObj, minDist);
		}
	}

	//Penalizamos por cercania
	for (unsigned int i = 0; i < population->size(); i++){
		bool okCond1 = ((*population)[i]->getObj(ordObj) > distance);
		if (!okCond1){
			if ((*population)[i]->getInternalOptDirection(0) == MAXIMIZE){
				(*population)[i]->setObj(0, 0);
			} else {
				(*population)[i]->setObj(0, DBL_MAX);
			}
		}
	}
}

void MultiObjNearestIndDist::convertToMultiObjective(vector<Individual *> *population, vector<Individual *> *reference,  int ordObj, int numberGeneration, double completedRatio) {
	if (completedRatio > 1){
		completedRatio = 1;
	}
	distance = distanceInit + (distanceEnd - distanceInit) * completedRatio;
	convertToMultiObjectiveWithRef(population, reference, ordObj);
}



bool MultiObjNearestIndDist::init(const vector<string> &params){
	map<string, string> parameters;
	if ((params.size() % 3) != 0){
		cerr << "El numero de parametros en MultiObjNearestIndDist tiene que ser multiplo de 3" << endl;
		return false;
	}
	for (unsigned int i = 0; i < params.size(); i+=3){
		if (params[i+1] != "="){
			cerr << "Error en los parametros de MultiObjNearestIndDist : se esperaba un igual" << endl;
			return false;
		}
		parameters[params[i]] = params[i+2];
	}
	readParameter(parameters, "distanceInit", distanceInit);
	readParameter(parameters, "distanceEnd", distanceEnd);
	return true;
}
