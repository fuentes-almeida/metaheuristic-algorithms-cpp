/*********************************************************************************************
 * AUTHORS
 *   Carlos Segura González
 *
 *********************************************************************************************/


#include "LocalScoreAlgorithm.h"

LocalScoreAlgorithm::LocalScoreAlgorithm() : ScoreAlgorithm(LOCAL_SCORE_ALGORITHM) {
}

long double LocalScoreAlgorithm::score(MOFront *){
	cerr << "Error interno. Se llama al metodo de score pasando un archivo, pero el metodo no esta implementado" << endl;
}

long double LocalScoreAlgorithm::score(vector<Individual*> *){
	cerr << "Error interno. Se llama al metodo de score pasando población, pero el metodo no esta implementado" << endl;
}

