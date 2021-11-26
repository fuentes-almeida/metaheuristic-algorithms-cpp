/***********************************************************************************
 * AUTHORS
 *   - Carlos Segura Gonzalez
 *
 **********************************************************************************/

#include <iostream>
#include <math.h>
#include <limits.h>
#include <sstream>
#include "MonoEA.h"
#include "utils.h"
#include "MultiObjNearestIndDist.h"

long optimum;//Best attainable value: the execution is stopped when this value is reached

// Algorithm initialization
bool MonoEA::init(const vector<string> &params){
	//Fill a map with all the parameters, with the aim of facilitating the lecture of parameters
	map<string, string> parameters;
	if ((params.size() % 3) != 0){
		cerr << "Error en los parametros de MonoEA. Deben ser multiplos de 3" << endl;
		return false;
	}
	for (unsigned int i = 0; i < params.size(); i+=3){
		parameters[params[i]] = params[i+2];
		if (params[i+1] != "="){
			cerr << "Error en los parametros de MonoDE. Deben estar formados por grupos de la forma parametro = valor" << endl;
			return false;
		}
	}

	int popSize;
	readParameter(parameters, "N", popSize);
	setPopulationSize(popSize);
	readParameter(parameters, "pm", pm); 
	readParameter(parameters, "pc", pc);
	readParameter(parameters, "Optimum", optimum);
	string survivalSelectionStr;
	readParameter(parameters, "SurvivalSelection", survivalSelectionStr);
	if (survivalSelectionStr == "GenerationalElit"){
		survivalSelectionType = SURVIVAL_SELECTION_GENERATIONAL_ELIT;
	} else if (survivalSelectionStr == "MultiDyn"){
		//Inicializar MultiObjNearestIndDist, que controla el objetivo auxiliar y las penalizaciones
		survivalSelectionType = SURVIVAL_SELECTION_MULTIDYN;
		MultiObjNearestIndDist *multi = new MultiObjNearestIndDist();
		string distanceInit, distanceEnd;
		readParameter(parameters, "distanceInit", distanceInit);
		readParameter(parameters, "distanceEnd", distanceEnd);
		vector<string> multiParams;
		multiParams.push_back("distanceInit");
		multiParams.push_back("=");
		multiParams.push_back(distanceInit);
		multiParams.push_back("distanceEnd");
		multiParams.push_back("=");
		multiParams.push_back(distanceEnd);
		multi->init(multiParams);
		getSampleInd()->setMultiObjectivizationPlugins(vector<MultiObjectivization *>(1, multi));
		setMultiObjectivizationPlugins(vector<MultiObjectivization *>(1, multi));
		getSampleInd()->setNumberOfObj(2);
	} else {
		cout << "Error SurvivalSelection no reconocido: " << survivalSelectionStr << endl;
		return false;
	}

	if (parameters.size()){
		cerr << "Error: hay parametros que no son usuaron como: " << parameters.begin()->first << endl;
		exit(-1);
	}
	return true;
}

void MonoEA::evaluateOffspring(){
	for (unsigned int i = 0; i < offSpring.size(); i++){
		offSpring[i]->evaluate();
	}
}

// MonoEA generation
void MonoEA::runGeneration(){
	for (unsigned int i = 0; i < population->size(); i++){
		if ((*population)[i]->getObj(0) == optimum){
			stringstream str;
			str << "Optimo alcanzado en el tiempo " << getElapsedTime() << endl;
			outputPrinter->print(str.str());
			outputPrinter->printSolution(this, true);
			exit(-1);
		}
	}
	offSpring.clear();
	createOffSprings();
	evaluateOffspring();
	survivalSelection();
}

void MonoEA::binaryTournamentSelection(int &p1, int &p2){//Supone que population esta ordenada
	int opt1 = (int) (((double)(population->size()))*rand()/(RAND_MAX+1.0));
	int opt2 = (int) (((double)(population->size()))*rand()/(RAND_MAX+1.0));
	if ((*population)[opt1]->isBetterEq(*(*population)[opt2])){
		p1 = opt1;
	} else {
		p1 = opt2;
	}

	opt1 = (int) (((double)(population->size()))*rand()/(RAND_MAX+1.0));
	opt2 = (int) (((double)(population->size()))*rand()/(RAND_MAX+1.0));
	if ((*population)[opt1]->isBetterEq(*(*population)[opt2])){
		p2 = opt1;
	} else {
		p2 = opt2;
	}
}

void MonoEA::createOffSpring(){//Crear 2 hijos
	int p1, p2;
	binaryTournamentSelection(p1, p2);
	Individual *of1 = (*population)[p1]->internalClone();
	Individual *of2 = (*population)[p2]->internalClone();
	double vcross = rand() / (RAND_MAX + 1.0);
	if (vcross < pc) {  
		of1->crossover(of2);
	}
	//Mutacion
	Individual *copyOf1 = of1->internalClone();
	of1->mutation(pm);
	of1->localSearch();
	if (!of1->isBetterEq(*copyOf1)){
		delete of1;
		of1 = copyOf1;
	} else {
		delete copyOf1;
	}
	Individual *copyOf2 = of2->internalClone();
	of2->mutation(pm);
	of2->localSearch();
	if (!of2->isBetterEq(*copyOf2)){
		delete of2;
		of2 = copyOf2;
	} else {
		delete copyOf2;
	}
	offSpring.push_back(of1);
	offSpring.push_back(of2);
}

void MonoEA::createOffSprings(){
	if (survivalSelectionType == SURVIVAL_SELECTION_GENERATIONAL_ELIT){
		int bestIndex = 0;
		for (unsigned int i = 1; i < population->size(); i++){
			if ((*population)[i]->isBetterEq(*(*population)[bestIndex])){
				bestIndex = i;
			}
		}
		offSpring.push_back((*population)[bestIndex]->internalClone());	
	}
	while(offSpring.size() < getPopulationSize()){
		createOffSpring();
	}
}
	
void MonoEA::survivalSelection(){
	if (survivalSelectionType == SURVIVAL_SELECTION_GENERATIONAL_ELIT){//El elitismo se hizo pasando al mejor a los offspring
		for (unsigned int i = 0; i < population->size(); i++){
			delete (*population)[i];
		}
		(*population) = offSpring;
	} else if (survivalSelectionType == SURVIVAL_SELECTION_MULTIDYN){
		//Join population and offspring
		for (unsigned int i = 0; i < offSpring.size(); i++){
			population->push_back( (offSpring)[i] );
		}

		vector< Individual* > survivors;
		vector<int> indexesBest(1, 0);
		double currentBestValue = (*population)[0]->getObj(0);
		//Select the individual with best "original" objective
		for (unsigned int i = 1; i < population->size(); i++){
			if ((*population)[i]->getObj(0) == currentBestValue){
				indexesBest.push_back(i);
			} else if ( ((*population)[0]->getInternalOptDirection(0) == MINIMIZE) && ((*population)[i]->getObj(0) < currentBestValue)){
				indexesBest.clear();
				indexesBest.push_back(i);
				currentBestValue = (*population)[i]->getObj(0);
			} else if ( ((*population)[0]->getInternalOptDirection(0) == MAXIMIZE) && ((*population)[i]->getObj(0) > currentBestValue)){
				indexesBest.clear();
				indexesBest.push_back(i);
				currentBestValue = (*population)[i]->getObj(0);
			}
		}
		int selectedIndex = indexesBest[generateRandom(0, indexesBest.size() - 1)];

		survivors.push_back((*population)[selectedIndex]);

		(*population)[selectedIndex] = (*population)[population->size() - 1];
		population->pop_back();

		while (survivors.size() != (unsigned int) getPopulationSize()){
			multiObjectivize(population, population, &survivors);
			vector<int> nonDominatedIndexes;
			markNonDominatedNoRep(*population, nonDominatedIndexes);
			//Select a random individual from the first front
			int selectedIndex = nonDominatedIndexes[generateRandom(0, nonDominatedIndexes.size() - 1)];
			survivors.push_back((*population)[selectedIndex]);
			//Recover original objective
			survivors[survivors.size()-1]->setObj(0, survivors[survivors.size()-1]->getAuxData(0));
			//Remove element from population. 
			(*population)[selectedIndex] = (*population)[population->size() - 1];
			population->resize(population->size() - 1);
		}
		//Eliminate remaining individuals
		for (unsigned int i = 0; i < population->size(); i++){
			delete (*population)[i];
		}
		population->clear();
		for (unsigned int i = 0; i < survivors.size(); i++){
			population->push_back(survivors[i]);
		}
	} else {
		cerr << "Error interno: survivalSelectionType no reconocido" << endl;
		exit(-1);
	}
}

//Calculo del objetivo auxiliar y penalizacion
void MonoEA::multiObjectivize(vector <Individual*> *p, vector<Individual*> *off, vector<Individual*> *reference){
	int start = getSampleInd()->getNumberOfObj() - getNumMultiObjectivizationPlugins();
	for (unsigned int i = start; i < getSampleInd()->getNumberOfObj(); i++){
		getMultiObjectivizationPlugin(i - start)->convertToMultiObjective(p, reference, i, getGeneration(), getCompletedRatio());
	}
}
