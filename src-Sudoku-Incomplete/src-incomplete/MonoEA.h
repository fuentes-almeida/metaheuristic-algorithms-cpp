/***********************************************************************************
 * AUTHORS
 *  - Carlos Segura Gonzalez
 * 
 * DESCRIPTION
 *	- Algoritmo Evolutivo con soporte para dos tipos de supervivencia:
 *	  * Generacional con elitismo
 *	  * MULTI_DYN aplicado en "The Importance of Diversity in the Application of
 *	  Evolutionary Algorithms to the Sudoku Problem"
 *	- Utiliza torneo binario para la seleccion de padres
 *	- Recibe como parametros
 *	  N -> tam. poblacion
 *    SurvivalSelection: puede ser GenerationalElit o MultiDyn
 *    pm -> probabilidad mutacion
 *    pc -> probabilidad cruce
 *    Optimum: cuando se alcance este valor se detiene la ejecucion
 *
 *    Si la supervivencia es MultiDyn tambien recibe los parametros distanceInit y
 *    distanceEnd que se usa para controlar las penalizaciones
 **********************************************************************************/


#ifndef MONOEA_H
#define MONOEA_H

#include "Individual.h"
#include "EA.h"

#include <vector>

using namespace std;

#define SURVIVAL_SELECTION_GENERATIONAL_ELIT 1
#define SURVIVAL_SELECTION_MULTIDYN 16

class MonoEA : public EA {
	public:
		bool init(const vector<string> &params);
	private:
		void runGeneration();
		void evaluateOffspring();
		void binaryTournamentSelection(int &p1, int &p2);
		void createOffSpring();
		void createOffSprings();
		void survivalSelection();
		void multiObjectivize(vector <Individual*> *p, vector<Individual*> *off, vector<Individual*> *reference);
		
		vector<Individual *> offSpring;
		double pm, pc;
		int survivalSelectionType;
};

#endif
