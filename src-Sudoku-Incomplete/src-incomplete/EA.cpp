/****************************************************************************************
 * AUTORES
 *   - Carlos Segura Gonzalez
 ****************************************************************************************/

#include "EA.h"
#include <sys/time.h>

EA::EA() {
	sampleInd = NULL;
	outputPrinter = NULL;
	population = new vector<Individual*>;
	pSize = 0;
	printPeriod = nextPrint = currentGeneration = 0;
	//Almacenamos el tiempo inicial
	struct timeval actualTime; 
	gettimeofday(&actualTime, NULL);
	startTime = (double) (actualTime.tv_sec) + (double) (actualTime.tv_usec)/1.0e6;
}

EA::~EA() {
	for (unsigned int i = 0; i < population->size(); i++)
		delete (*population)[i];
	delete population;
}

/* ******************************************************************************
 * Este método devuelve true cuando se ha alcanzado el criterio de parada local
 * del EA: el criterio se establece por tiempo (en segundos)
 * ******************************************************************************/
bool EA::hasFinished() {
	struct timeval actualTime; 
	gettimeofday(&actualTime, NULL);
	double at = (double) (actualTime.tv_sec) + (double) (actualTime.tv_usec)/1.0e6;
	return (at >= startTime + critStopValue);
}

//Ratio de tiempo transcurrido frente al tiempo total
double EA::getCompletedRatio(){
	double elapsed = getElapsedTime();
	if (elapsed >= critStopValue){
		return 1;
	} else {
		return (elapsed / critStopValue);
	}
}

// Realiza las impresiones intermedias 
void EA::checkPrint() {
	if (printPeriod == 0)
		return;
	if (getElapsedTime() >= nextPrint){
		outputPrinter->printSolution(this, false);
		nextPrint += printPeriod;
	} 
}

//Ejecutar generaciones hasta que se acalnce el criterio de parada
void EA::run() {
	fillPopWithNewIndsAndEvaluate();
	do {
		checkPrint();
		runGeneration();
		currentGeneration++;
	} while (!hasFinished());
	checkPrint();
}

/************************************************************************************
 * Rellena la poblacion hasta llegar al tamaño de la población, con individuos que 
 * se crean llamando al metodo restart del individuo
 * *********************************************************************************/
void EA::fillPopWithNewIndsAndEvaluate() {
	if (pSize == 0) {
		cerr << "Error: fillPopWithNewInds called but pSize has not been fixed. Using default value: 100" << endl;
		exit(-1);
	}
	for (unsigned int i = population->size(); i < getPopulationSize(); i++) {
		Individual *ind = sampleInd->internalClone();
		ind->restart();
		ind->evaluate();
		population->push_back (ind);
	}
}

double EA::getElapsedTime() {
	struct timeval actualTime; 
	gettimeofday(&actualTime, NULL);
	double at = (double) (actualTime.tv_sec) + (double) (actualTime.tv_usec) / 1.0e6;
	return (at - startTime);
}
