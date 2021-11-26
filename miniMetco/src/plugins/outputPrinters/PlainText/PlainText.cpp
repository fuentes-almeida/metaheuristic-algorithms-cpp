#include "MOFrontVector.h"
#include "PlainText.h"

void PlainText::printSolution(EA *ga, bool end){
	outputFile.precision(18);
	if (end){
		ga->printInfo(outputFile);
		outputFile << endl << "Time =  " << ga->getElapsedTime() << endl << endl;
	}

	if (ga->getCritStop() == ga->getTypeStoppingCriterion("TIME")) {
		if (end){
			outputFile << "Current Evaluation = " << ga->getPerformedEvaluations() << endl;
		} else {
			outputFile << "Current Evaluation = " << ga->getNextPrint() << "(" << ga->getNextPrint() << ")" << endl;
		}
		if (ga->isGeneratingArchive()){
			outputFile << ga->getLocalSolution();
		} else {
			/*MOFront *p = new MOFrontVector(ga->getSampleInd(), false, false);
			ga->getSolution(p);
			outputFile << p;
			delete p;*/

			//Suponemos que hay elitismo: lo que se imprime es la poblacion actual
			int bestIndex = 0;
			for (int i = 1; i < ga->getPopulationSize(); i++){
				if (ga->getIndividual(i)->isBetterEq(*ga->getIndividual(bestIndex))){
					bestIndex = i;
				}
			}
			outputFile << "Front Size = " << ga->getPopulationSize() << endl;

			Individual *ind = ga->getIndividual(bestIndex);
			ind->print(outputFile);
			/*for (int j = 0; j < ind->getNumberOfVar(); j++){
				outputFile << ind->getVar(j) << " ";
			}
			outputFile << ind->getObj(0) << endl;*/

			for (int i = 0; i < ga->getPopulationSize(); i++){
				if (i == bestIndex){
					continue;
				}
				ind = ga->getIndividual(i);
				ind->print(outputFile);
				/*
				for (int j = 0; j < ind->getNumberOfVar(); j++){
					outputFile << ind->getVar(j) << " ";
				}
				outputFile << ind->getObj(0) << endl;*/
			}
			outputFile << endl << flush;
		}
	} else if (ga->getCritStop() == ga->getTypeStoppingCriterion("EVALUATIONS")){
		if (end){
			outputFile << "Current Evaluation = " << ga->getPerformedEvaluations() << endl;
		} else {
			outputFile << "Current Evaluation = " << ga->getNextPrint() << "(" << ga->getPerformedEvaluations() << ")" << endl;
		}
		if (ga->isGeneratingArchive()){
			outputFile << ga->getLocalSolution();
		} else {
			//cerr << "Va a hacer el new" << endl << flush;
			MOFront *p = new MOFrontVector(ga->getSampleInd(), false, false);
			//cerr << "Va a hacer el getSolution" << endl << flush;
			ga->getSolution(p);
			//cerr << "Va a imprimir en archivo" << endl << flush;
			outputFile << p;
			//cerr << "Va a borrar" << endl << flush;
			delete p;
			//cerr << "Borro" << endl << flush;
		}
	}
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
