/***********************************************************************************
 * AUTHORS
 *  - Carlos Segura Gonzalez
 *  - Eduardo Segredo Gonzalez
 * 
 * DATE
 *    May 2014
 *
 * DESCRIPTION
 *		Mono-Objective Genetic Algorithm with several survival selection operators
 *     
 **********************************************************************************/


#ifndef MONOGA2_H
#define MONOGA_H

#include "Individual.h"
#include "EA.h"

#include <vector>

using namespace std;

#define SURVIVAL_SELECTION_SS 0
#define SURVIVAL_SELECTION_GENERATIONAL_ELIT 1
#define SURVIVAL_SELECTION_REPLACEWORST_FROMPARENTANDOFFSPRING 2
#define SURVIVAL_SELECTION_CROWDING_COMPLETE 3 //Este es como el crowding_RestrictedTournamentSelection con ventana igual al tam. de poblacion y no steady state. En este unos hijos pueden sustituir a otros hijos, etc. etc. y no es un metodo sacado de otro articulo, asi que creo que mejor usar el Restricted Tournament Selection con w = N
#define SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5 4 //Este se conoce como Deterministic Crowding, a diferencia del crowding original de DeJong, toma en cuenta el fitness
#define SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PARENT_BINARY_TOURNAMENT 5
#define SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5_PROBABILISTIC 6
#define SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PROBABILISTIC_PARENT_BINARY_TOURNAMENT 7
#define SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PARENT_RANDOM 8
#define SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PROBABILISTIC_PARENT_RANDOM 9
#define SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5_SCALED_PROBABILISTIC 10 
#define SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_SCALED_PROBABILISTIC_PARENT_BINARY_TOURNAMENT 11
#define SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_SCALED_PROBABILISTIC_PARENT_RANDOM 12 
#define SURVIVAL_SELECTION_CLEARING_PARENT_RANDOM_ELITIST_WINNERS_ABOVE_AVERAGE 13
#define SURVIVAL_SELECTION_CLEARING_PARENT_BINARY_TOURNAMENT_ELITIST_WINNERS_ABOVE_AVERAGE 14
#define SURVIVAL_SELECTION_OBECTIVESCOMBINED_PARENT_BINARY_TOURNAMENT 15 //Vidal: A hybrid GA with adaptive diversity management for a large class of vehicle routing problems with time-windows
#define SURVIVAL_SELECTION_CENTRAL_PART_OF_FRONT_PARENT_BINARY_TOURNAMENT 16
#define SURVIVAL_SELECTION_DIVERSITY_ADAPTIVE_GENERALIZED_CROWDING 17
#define SURVIVAL_SELECTION_CDRW_GENERATIONAL_PARENT_BINARY_TOURNAMENT 18 //Esquema de Lozano
#define SURVIVAL_SELECTION_BEST_NO_DIVERSITY_PENALIZED_PARENT_BINARY_TOURNAMENT 19

#define POPULATION_TYPE_FIXED 0
#define POPULATION_TYPE_DECREASING 1
#define POPULATION_TYPE_SAW_TOOTH 2

#define MUTATION_TYPE_NORMAL 0
#define MUTATION_TYPE_ILS 1

#define LOCAL_SEARCH_ALWAYS 0
#define LOCAL_SEARCH_RANDOM 1
#define LOCAL_SEARCH_BEST 2

class MonoGA2 : public EA {

	public:
		bool init(const vector<string> &params);
		void getSolution(MOFront *p);
		void printInfo(ostream &os) const;
	private:
		void runGeneration();
		void evaluateOffspring(int from);
		void sortPopulation();
		void binaryTournamentSelection(int &p1, int &p2);
		void randomSelectionWithoutReplacement(int &p1, int &p2);
		void parentSelection(int &p1, int &p2);
		void createOffSpring();
		void createOffSprings();
		void survivalSelection();
		void createOffspringWithRandomSelectedParents();
		void clearParents();
		void multiObjectivize(vector <Individual*> *p, vector<Individual*> *off, vector<Individual*> *reference);
		bool supportsMultiObjectivization() { return true; }
		void reducePopulationSize();
		double getPopulationEntropy();
		double getPopulationEntropy2();
		void updateMinPopulationDistances();
		
		vector<Individual *> offSpring;
		double pm, pc;
		double w;
		int survivalSelectionType;
		int mutationType;
		double k, sigma;
		int NElit, NClose;
		map<string, string> parameters;
		Individual *best;
		vector< double > minPopulationDistances;
		vector< vector < double > > populationDistances;

		double lsProp;
		int NInit, NStages, currentPopStage, populationType;
		int alleles;
		int ILSIterations;
		int localSearchType;
		double initialScaleFactor;//Generalized Crowding
		double initialEntropy;
		int meanPopSize, T, D;
		vector<double> entropyInformation;
};

bool sortByObj0(const Individual *i1, const Individual *i2);

#endif
