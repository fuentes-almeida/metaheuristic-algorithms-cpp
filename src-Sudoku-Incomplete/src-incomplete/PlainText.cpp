/***********************************************************************************
 * AUTHORS
 *   - Carlos Segura González
 ***********************************************************************************/

#include "PlainText.h"

void PlainText::print(const string &str){
	outputFile << str << endl;
}

void PlainText::printSolution(EA *ga, bool end){
	outputFile.precision(18);
	outputFile << "Current Time = " << ga->getElapsedTime() << endl;
	//Imprimimos la poblacion actual
	int bestIndex = 0;
	for (int i = 1; i < ga->getCurrentPopulationSize(); i++){
		if (ga->getIndividual(i)->isBetterEq(*ga->getIndividual(bestIndex))){
			bestIndex = i;
		}
	}
	outputFile << "Population Size = " << ga->getCurrentPopulationSize() << endl;
	Individual *ind = ga->getIndividual(bestIndex);
	ind->print(outputFile);

	for (int i = 0; i < ga->getCurrentPopulationSize(); i++){
		if (i == bestIndex){
			continue;
		}
		ind = ga->getIndividual(i);
		ind->print(outputFile);
	}
	outputFile << endl << flush;
}

bool PlainText::init(const vector<string> &params){
	if (params.size() != 1){
		cerr << "Incorrect parameters to module PlainText: outputFile" << endl;
		return false;
	}
	outputFile.open(params[0].c_str(), ios::in|ios::out|ios::trunc);
	if (!outputFile.is_open()){
		cerr << "No pudo abrir fichero: " << params[0] << endl;
	}
	return true;
}

void PlainText::finish(){
	outputFile.close();
}
