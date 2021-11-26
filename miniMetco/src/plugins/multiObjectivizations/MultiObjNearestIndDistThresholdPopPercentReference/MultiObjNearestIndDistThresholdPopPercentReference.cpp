#include "MultiObjNearestIndDistThresholdPopPercentReference.h"

#include <map>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <limits.h>

using namespace std;

void readParameter(map<string,string> &parameters, const string &parameter, double &value){
	if (parameters.count(parameter) == 0){
		cerr << "Parametro " << parameter << " de MonoDE no definido" << endl;
		exit(-1);
	}
	value = atof(parameters[parameter].c_str());
}

void readParameter(map<string,string> &parameters, const string &parameter, int &value){
	if (parameters.count(parameter) == 0){
		cerr << "Parametro " << parameter << " de MonoDE no definido" << endl;
		exit(-1);
	}
	value = atoi(parameters[parameter].c_str());
}

void readParameter(map<string,string> &parameters, const string &parameter, string &value){
	if (parameters.count(parameter) == 0){
		cerr << "Parametro " << parameter << " de MonoDE no definido" << endl;
		exit(-1);
	}
	value = parameters[parameter];
}

bool sortByIncObj0(const Individual *a, const Individual *b){
	return (a->getObj(0) < b->getObj(0));
}

bool sortByDecObj0(const Individual *a, const Individual *b){
	return (a->getObj(0) > b->getObj(0));
}

void MultiObjNearestIndDistThresholdPopPercentReference::convertToMultiObjectiveWithRef(vector<Individual *> *population, vector<Individual *> *reference, int ordObj, double completeRatio) {
	if (reference->size() == 1){
		//Es la primera llamada, asi que hay que penalizar en funcion del objetivo (ya se queda penalizado, porque eso no cambia en las siguientes) y asumimos que una
		//Almacenamos el objetivo como datos auxiliar para despues poder recuperarlo (penalizados por cercania)
		for (int i = 0; i < population->size(); i++){
			(*population)[i]->setAuxDataSize(1);
			(*population)[i]->setAuxData(0, (*population)[i]->getObj(0));
			if (((*population)[i]->getObj(0) == 0) || ((*population)[i]->getObj(0) == DBL_MAX)){
				cerr << "Error interno: en obj contiene un 0 o DBL_MAX (llega penalizado)" << endl;
				exit(-1);
			}
		}
		//Ordenamos por el objetivo original
		if ((*reference)[0]->getInternalOptDirection(0) == MAXIMIZE){
			sort(population->begin(), population->end(), sortByDecObj0);
		} else {
			sort(population->begin(), population->end(), sortByIncObj0);
		}
		
		int penalized = population->size() * pThreshold;
		for (int i = 0; i < penalized; i++){
			(*population)[population->size() - 1 - i]->setObj(ordObj, 0);//Penalizamos indicando que no contribuye en diversidad
		}

		//Calculamos el objetivo auxiliar para el resto
		for (int i = 0; i <= population->size() - 1 - penalized; i++){
			double actualDist = (*population)[i]->getDistance(*(*reference)[0]);
			(*population)[i]->setObj(ordObj, actualDist);
		}
	}

	//Actualizamos distancia considerando el ultimo elegido
 	for (unsigned int i = 0; i < population->size(); i++) {
		double minDist = (*population)[i]->getObj(ordObj);	
		unsigned int j = reference->size() - 1;
		double actualDist = (*population)[i]->getDistance(*(*reference)[j]);
		minDist = min(actualDist, minDist);
		(*population)[i]->setObj(ordObj, minDist);
	}

	//Penalizamos por cercania
	for (int i = 0; i < population->size(); i++){
		if ((*population)[i]->getObj(ordObj) > distance){//Tal vez hubiese sido mejor >= distance, pero bueno ya que los resultados estan asi, tampoco creo que la diferencia sea grande
			(*population)[i]->setObj(0, (*population)[i]->getAuxData(0));
		} else {//Penaliza los menores o iguales que distance (por eso ni al poner 0 entran los clones) Si queremos que acepte clones habria que ponerlo negativo
			if ((*population)[i]->getOptDirection(0) == MAXIMIZE){
				(*population)[i]->setObj(0, 0);
			} else {
				(*population)[i]->setObj(0, DBL_MAX);
			}
		}
	}
}

void MultiObjNearestIndDistThresholdPopPercentReference::convertToMultiObjective(vector<Individual *> *population, vector<Individual *> *offSpring, vector<Individual *> *reference,  int ordObj, int numberGeneration, double completedRatio) {
	if (completedRatio > 1){
		completedRatio = 1;
	}
	if (DUpdateType == LINEAR){
		distance = distanceInit + (distanceEnd - distanceInit) * completedRatio;
	} else if (DUpdateType == LINEAR_PIECEWISE){
		if (completedRatio <= p1){
			distance = distanceInit - completedRatio * (distanceInit / p1 * 0.1);
		} else if (completedRatio <= p2){
			distance = distanceInit * (-0.8 * completedRatio + 0.9 * p2 - 0.1 * p1) / (p2 - p1);
		} else {
			distance = (0.1 * distanceInit * (1.0 - completedRatio)) / (1 - p2);
		}
	}
	//cout << "Distancia: " << distance << endl;
	pThreshold = pThresholdInit + (pThresholdEnd - pThresholdInit) * completedRatio;
	//cout << "Distancia de corte: " << distance << endl;
	//cout << "Threshold de corte: " << pThreshold << endl;

	if ((reference != population) || (reference != offSpring)){//Por ahora no se usa offSpring ya que en el NSGA-II juntamos todo
		convertToMultiObjectiveWithRef(population, reference, ordObj, completedRatio);
		return;
	} else {
		cout << "No implementado" << endl;
		exit(-1);
	}
}


void MultiObjNearestIndDistThresholdPopPercentReference::setParameters(vector<double> &parameters) {
	if (parameters.size() != 1){
		cerr << "En setParameters se esperaba un unico parametro (distanceInit)" << endl;
		exit(-1);
	}
	distanceInit = parameters[0];
}

bool MultiObjNearestIndDistThresholdPopPercentReference::init(const vector<string> &params) {
	map<string, string> parameters;
	if ((params.size() % 3) != 0){
		cerr << "El numero de parametros en MultiObjNearestIndDistThresholdPopPercentReference tiene que ser multiplo de 3" << endl;
		return false;
	}
	for (int i = 0; i < params.size(); i+=3){
		if (params[i+1] != "="){
			cerr << "Error en los parametros de MultiObjNearestIndDistThresholdPopPercentReference: se esperaba un igual" << endl;
			return false;
		}
		parameters[params[i]] = params[i+2];
	}
	readParameter(parameters, "pThresholdInit", pThresholdInit);
	readParameter(parameters, "pThresholdEnd", pThresholdEnd);
  if ( (pThresholdInit < 0) || (pThresholdInit > 1) || (pThresholdEnd < 0) || (pThresholdEnd > 1) ){
    cerr << "Error, pThreshold must be in [0, 1]" << endl;
		return false;
  }

	string DUpdateTypeStr;
	readParameter(parameters, "DUpdateType", DUpdateTypeStr);
	if (DUpdateTypeStr == "Linear"){
		DUpdateType = LINEAR;
		readParameter(parameters, "distanceInit", distanceInit);
		readParameter(parameters, "distanceEnd", distanceEnd);
	} else if (DUpdateTypeStr == "LinearPiecewise"){
		DUpdateType = LINEAR_PIECEWISE;
		readParameter(parameters, "distanceInit", distanceInit);
		readParameter(parameters, "p1", p1);
		readParameter(parameters, "p2", p2);
		if (p1 > p2){
			cerr << "Error p1 no puede ser mayor que p2" << endl;
			return false;
		}
	} else {
		cout << "DUpdateType no reconocido" << endl;
		return false;
	}
  return true;
}
