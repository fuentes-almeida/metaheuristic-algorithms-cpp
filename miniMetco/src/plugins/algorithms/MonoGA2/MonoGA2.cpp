/***********************************************************************************
 * AUTHORS
 *   - Carlos Segura Gonzalez
 *
 **********************************************************************************/
 
#include <cstdlib>
#include <iostream>
#include <signal.h>
#include <math.h>
#include <limits.h>
#include <set>

#include "MonoGA2.h"

vector<int> parentIndexes;//Usado para CrowdingMahfoudA5. No está como miembro estatico de la clase, porque en algunos compiladores da problemas usar plugins y miembros estaticos

int bestValue = INT_MAX;

bool sortByObj0(const Individual *i1, const Individual *i2){
	return (i1->getInternalOptDirection(0) == MINIMIZE)?(i1->getObj(0) < i2->getObj(0)):(i1->getObj(0) > i2->getObj(0));
}

int auxIndex = 0;
bool sortByAux(const Individual *i1, const Individual *i2){
	return (i1->getAuxData(auxIndex) < i2->getAuxData(auxIndex));
}

void readParameter(map<string,string> &parameters, const string &parameter, double &value){
	if (parameters.count(parameter) == 0){
		cerr << "Parametro " << parameter << " de MonoGA2 no definido" << endl;
		exit(-1);
	}
	value = atof(parameters[parameter].c_str());
}

void readParameter(map<string,string> &parameters, const string &parameter, int &value){
	if (parameters.count(parameter) == 0){
		cerr << "Parametro " << parameter << " de MonoGA2 no definido" << endl;
		exit(-1);
	}
	value = atoi(parameters[parameter].c_str());
}

void readParameter(map<string,string> &parameters, const string &parameter, string &value){
	if (parameters.count(parameter) == 0){
		cerr << "Parametro " << parameter << " de MonoGA2 no definido" << endl;
		exit(-1);
	}
	value = parameters[parameter];
}

// Algorithm initialization
bool MonoGA2::init(const vector<string> &params){
	cout << "Empieza la carga de MonoGA2" << endl << flush;
	if ((params.size() % 3) != 0){
		cerr << "Error en los parametros de MonoGA2. Deben ser multiplos de 3" << endl;
		return false;
	}
	for (int i = 0; i < params.size(); i+=3){
		parameters[params[i]] = params[i+2];
		if (params[i+1] != "="){
			cerr << "Error en los parametros de MonoDE. Deben estar formados por grupos de la forma parametro = valor" << endl;
			return false;
		}
	}

	best = NULL;
	string survivalSelectionStr;
	readParameter(parameters, "SurvivalSelection", survivalSelectionStr);
	if (survivalSelectionStr == "SteadyState"){
		survivalSelectionType = SURVIVAL_SELECTION_SS;
	} else if (survivalSelectionStr == "GenerationalElit"){
		survivalSelectionType = SURVIVAL_SELECTION_GENERATIONAL_ELIT;
	} else if (survivalSelectionStr == "ReplaceWorstFromParentAndOffspring"){
		survivalSelectionType = SURVIVAL_SELECTION_REPLACEWORST_FROMPARENTANDOFFSPRING;
	} else if (survivalSelectionStr == "CrowdingComplete"){
		survivalSelectionType = SURVIVAL_SELECTION_CROWDING_COMPLETE;
	} else if (survivalSelectionStr == "CrowdingMahfoudA5"){
		survivalSelectionType = SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5;
	} else if (survivalSelectionStr == "CrowdingMahfoudA5Probabilistic"){
		survivalSelectionType = SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5_PROBABILISTIC;
	} else if (survivalSelectionStr == "CrowdingRestrictedTournamentSelectionParentBinaryTournament"){
		survivalSelectionType = SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PARENT_BINARY_TOURNAMENT;
	} else if (survivalSelectionStr == "CrowdingRestrictedTournamentSelectionProbabilisticParentBinaryTournament"){
		survivalSelectionType = SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PROBABILISTIC_PARENT_BINARY_TOURNAMENT;
	} else if (survivalSelectionStr == "CrowdingRestrictedTournamentSelectionParentRandom"){
		survivalSelectionType = SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PARENT_RANDOM;
	} else if (survivalSelectionStr == "CrowdingRestrictedTournamentSelectionProbabilisticParentRandom"){
		survivalSelectionType = SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PROBABILISTIC_PARENT_RANDOM;
	}	else if (survivalSelectionStr == "CrowdingMahfoudA5ScaledProbabilistic"){
		survivalSelectionType = SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5_SCALED_PROBABILISTIC;
	} else if (survivalSelectionStr == "CrowdingRestrictedTournamentSelectionScaledProbabilisticParentBinaryTournament"){
		survivalSelectionType = SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_SCALED_PROBABILISTIC_PARENT_BINARY_TOURNAMENT;
	} else if (survivalSelectionStr == "CrowdingRestrictedTournamentSelectionScaledProbabilisticParentRandom"){
		survivalSelectionType = SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_SCALED_PROBABILISTIC_PARENT_RANDOM;
	} else if (survivalSelectionStr == "ClearingParentRandomElitistWinnersAboveAverage"){
		survivalSelectionType = SURVIVAL_SELECTION_CLEARING_PARENT_RANDOM_ELITIST_WINNERS_ABOVE_AVERAGE;
	} else if (survivalSelectionStr == "ClearingParentBinaryTournamentElitistWinnersAboveAverage"){
		survivalSelectionType = SURVIVAL_SELECTION_CLEARING_PARENT_BINARY_TOURNAMENT_ELITIST_WINNERS_ABOVE_AVERAGE;
	} else if (survivalSelectionStr == "ObjectivesCombinedParentBinaryTournament"){
		survivalSelectionType = SURVIVAL_SELECTION_OBECTIVESCOMBINED_PARENT_BINARY_TOURNAMENT;
	} else if (survivalSelectionStr == "CentralPartOfFrontParentBinaryTournament"){
		survivalSelectionType = SURVIVAL_SELECTION_CENTRAL_PART_OF_FRONT_PARENT_BINARY_TOURNAMENT;
	} else if (survivalSelectionStr == "DiversityAdaptiveGeneralizedCrowding"){
		survivalSelectionType = SURVIVAL_SELECTION_DIVERSITY_ADAPTIVE_GENERALIZED_CROWDING;
	} else if (survivalSelectionStr == "CDRWGenerationalParentBinaryTournament"){
		survivalSelectionType = SURVIVAL_SELECTION_CDRW_GENERATIONAL_PARENT_BINARY_TOURNAMENT;
	} else if (survivalSelectionStr == "BestNoDiversityPenalizedParentBinaryTournament"){
		survivalSelectionType = SURVIVAL_SELECTION_BEST_NO_DIVERSITY_PENALIZED_PARENT_BINARY_TOURNAMENT;
	} else {
		cout << "Error SurvivalSelection no reconocido: " << survivalSelectionStr << endl;
		return false;
	}

	if ((survivalSelectionType == SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5_PROBABILISTIC) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PROBABILISTIC_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PROBABILISTIC_PARENT_RANDOM) || (survivalSelectionType ==  SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_SCALED_PROBABILISTIC_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType ==  SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_SCALED_PROBABILISTIC_PARENT_RANDOM)){
		if (getSampleInd()->getOptDirection(0) == MINIMIZE){
			cerr << "Error, operadores solo implementados para maximizacion" << endl;
			exit(-1);
		}
	}
	if ((survivalSelectionType == SURVIVAL_SELECTION_CLEARING_PARENT_RANDOM_ELITIST_WINNERS_ABOVE_AVERAGE) || (survivalSelectionType == SURVIVAL_SELECTION_CLEARING_PARENT_BINARY_TOURNAMENT_ELITIST_WINNERS_ABOVE_AVERAGE)){
		readParameter(parameters, "sigma", sigma);//Distancia a considerar para los nichos
		readParameter(parameters, "k", k);//número máximo de individuos en cada nicho
	}

	alleles = 400;
	if (survivalSelectionType == SURVIVAL_SELECTION_DIVERSITY_ADAPTIVE_GENERALIZED_CROWDING){
		readParameter(parameters, "InitialScaleFactor", initialScaleFactor);
		readParameter(parameters, "Alleles", alleles);
	}

	readParameter(parameters, "pm", pm); 
	readParameter(parameters, "pc", pc);

	string popTypeStr;
	readParameter(parameters, "PopType", popTypeStr);
	if (popTypeStr == "Fixed"){
		int popSize;
		readParameter(parameters, "N", popSize);
		setPopulationSize(popSize);
		NInit = popSize;
		populationType = POPULATION_TYPE_FIXED;
	} else if (popTypeStr == "Decreasing"){
		readParameter(parameters, "NInit", NInit);
		readParameter(parameters, "NStages", NStages);
		setPopulationSize(NInit);
		currentPopStage = 0;
		populationType = POPULATION_TYPE_DECREASING;
	} else if (popTypeStr == "Saw-Tooth"){
		readParameter(parameters, "MeanPopSize", meanPopSize);
		readParameter(parameters, "T", T);
		readParameter(parameters, "D", D);
		setPopulationSize(meanPopSize + D);
		populationType = POPULATION_TYPE_SAW_TOOTH;
	} else {
		cout << "Tipo de poblacion no reconocida" << endl;
		exit(-1);
	}

	if ((survivalSelectionType == POPULATION_TYPE_DECREASING) && (survivalSelectionType == SURVIVAL_SELECTION_CDRW_GENERATIONAL_PARENT_BINARY_TOURNAMENT)){
		cout << "Combinacion no implementada" << endl;
		exit(-1);
	}

	if (((survivalSelectionType == SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5_PROBABILISTIC) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5_SCALED_PROBABILISTIC) || (survivalSelectionType == SURVIVAL_SELECTION_DIVERSITY_ADAPTIVE_GENERALIZED_CROWDING)) && (((NInit % 2) == 1) || (populationType == POPULATION_TYPE_DECREASING))){
		cerr << "Error: al usuar Mahfoud_a5, el tam. de la población debe ser par" << endl;
		exit(-1);
	}
	if ((survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PROBABILISTIC_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_SCALED_PROBABILISTIC_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PARENT_RANDOM) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PROBABILISTIC_PARENT_RANDOM) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_SCALED_PROBABILISTIC_PARENT_RANDOM)){
		readParameter(parameters, "w", w);//windows size
	}

	if (survivalSelectionType == SURVIVAL_SELECTION_OBECTIVESCOMBINED_PARENT_BINARY_TOURNAMENT){
		readParameter(parameters, "NClose", NClose);
		readParameter(parameters, "NElit", NElit);
		if ((NClose > NInit) || (NElit > NInit)){
			cerr << "Error: NClose y NElit tienen que ser menores o iguales que N" << endl;
			exit(-1);
		}
	}

	string mutationTypeStr;	
	readParameter(parameters, "MutationType", mutationTypeStr);
	if (mutationTypeStr == "Normal"){
		mutationType = MUTATION_TYPE_NORMAL;
	} else if (mutationTypeStr == "ILS"){
		mutationType = MUTATION_TYPE_ILS;
		readParameter(parameters, "ILSIterations", ILSIterations);
	} else {
		cerr << "Error: MutationType no definido" << endl;
		exit(-1);
	}

	if ((populationType == POPULATION_TYPE_SAW_TOOTH) && (survivalSelectionType != SURVIVAL_SELECTION_GENERATIONAL_ELIT)){
		cerr << "Error: saw_tooth solo esta implementation para generational elit" << endl;
		exit(-1);
	}
	string localSearchTypeStr;
	readParameter(parameters, "LocalSearchType", localSearchTypeStr);
	if (localSearchTypeStr == "Always"){
		localSearchType = LOCAL_SEARCH_ALWAYS;
	} else if (localSearchTypeStr == "Random"){
		localSearchType = LOCAL_SEARCH_RANDOM;
		readParameter(parameters, "lsSize", lsProp);
	} else if (localSearchTypeStr == "Best"){
		localSearchType = LOCAL_SEARCH_BEST;
		readParameter(parameters, "lsSize", lsProp);
	} else {
		cerr << "Error: LocalSearchType no reconocido" << endl;
		exit(-1);
	}
	
	// Only mono-objective optimization is supported
	/*if (getSampleInd()->getNumberOfObj() != 1){
		cout << "Multi-Objective not supported" << endl;
		return false;
	}*/
	cout << "Termina la carga de MonoGA2" << endl << flush;

	return true;
}

void MonoGA2::evaluateOffspring(int from){
	for (int i = from; i < offSpring.size(); i++){
		evaluate(offSpring[i]);
		if ( ((offSpring[i]->getObj(0) < bestValue) && (offSpring[i]->getOptDirection(0) == MINIMIZE)) || ((offSpring[i]->getObj(0) > bestValue) && (offSpring[i]->getOptDirection(0) == MAXIMIZE)) ){
			bestValue = offSpring[i]->getObj(0);
		}
		if (offSpring[i]->isBetterEq(*best)){
			delete best;
			best = offSpring[i]->internalClone();
		}
	}
}

void MonoGA2::clearParents(){
	vector<int> parentsToDelete;
	sortPopulation();
	double meanFitness = 0;
	for (int i = 0; i < population->size(); i++){
		meanFitness += (*population)[i]->getObj(0);
	}
	meanFitness /= (population->size());
	for (int i = 0; i < population->size(); i++){
		if ((*population)[i]->getObj(0) != 0){//OJO: asumismos que el 0 no se da
			if ((*population)[i]->getOptDirection(0) == MAXIMIZE){
				if ((*population)[i]->getObj(0) > meanFitness){
					offSpring.push_back((*population)[i]->internalClone());
				}
			} else if ((*population)[i]->getOptDirection(0) == MINIMIZE){
				if ((*population)[i]->getObj(0) < meanFitness){
					offSpring.push_back((*population)[i]->internalClone());
				}
			}
			int nbWinners = 1;
			for (int j = i + 1; j < population->size(); j++){
				if (((*population)[j]->getObj(0) > 0) && ((*population)[i]->getDistance(*(*population)[j]) < sigma)){
					if (nbWinners < k){
						nbWinners++;//se podria pasar tambien al offspring, pero es opcional
					} else {
						(*population)[j]->setObj(0, 0);//OJO: asumimos que 0 no es un valor valido!!!! Tal vez deberiamos marcarlo de otra forma
						parentsToDelete.push_back(j);
					}
				}
			}
		}
	}
	sort(parentsToDelete.begin(), parentsToDelete.end());
	for (int i = ((int) parentsToDelete.size()) -1; i >= 0; i--){
		if ((*population)[parentsToDelete[i]]->getObj(0) != 0){
			cerr << "Error interno, intenta borrar a uno que no es" << endl;
			exit(-1);
		}
		delete (*population)[parentsToDelete[i]];
		swap((*population)[parentsToDelete[i]], (*population)[population->size() - 1]);
		population->resize(population->size()-1);
	}
}

void MonoGA2::reducePopulationSize(){
	int newPopSize = getPopulationSize() / 2;
	if (getPopulationSize() % 2){
		newPopSize++;
	}
	for (int i = 0; i < newPopSize; i++){
		if (i + newPopSize < getPopulationSize()){//En poblaciones impares el que esta en medio entra siempre
			if ((*population)[i+newPopSize]->isBetterEq(*(*population)[i])){//Entra el mejor de los dos
				swap((*population)[i+newPopSize], (*population)[i]);
			}
		}
	}
	for (int i = newPopSize; i < getPopulationSize(); i++){
		delete (*population)[i];
	}
	population->resize(newPopSize);
	setPopulationSize(newPopSize);
	if (survivalSelectionType == SURVIVAL_SELECTION_OBECTIVESCOMBINED_PARENT_BINARY_TOURNAMENT){
		if ((NClose > getPopulationSize()) || (NElit > getPopulationSize())){
			cerr << "Error: NClose y NElit tienen que ser menores o iguales que N" << endl;
			exit(-1);
		}
	}
}

void MonoGA2::updateMinPopulationDistances(){
	minPopulationDistances.clear();
	minPopulationDistances.insert(minPopulationDistances.begin(), population->size(), DBL_MAX);
	for (int i = 0; i < population->size(); i++){
		for (int j = i + 1; j < population->size(); j++){
			minPopulationDistances[i] = min(minPopulationDistances[i], populationDistances[i][j]);
			minPopulationDistances[j] = min(minPopulationDistances[j], populationDistances[i][j]);
		}
	}
}

double start;
int lastPrinted = 0;

// MonoGA2 generation
void MonoGA2::runGeneration(){
	//cout << "Empieza generacion" << endl << flush;
	/*set < pair<int, int> > differentPairs;
	for (int i = 0; i < getPopulationSize(); i++){
		for (int j = 0; j < (*population)[i]->getNumberOfVar() - 1; j++){
			differentPairs.insert(make_pair((*population)[i]->getVar(j), ((*population)[i]->getVar(j+1))));
		}
		differentPairs.insert(make_pair(((*population)[i]->getVar((*population)[i]->getNumberOfVar() - 1)), (*population)[i]->getVar(0)));
	}
	cout << "Pares: " << differentPairs.size() << endl;*/
	//double ent = getPopulationEntropy2();
	//cout << "Entropia: " << ent << endl;
	//sortPopulation();//Se ordena cada vez porque el paralelo podria desordenarlo TODO: ordenar solo 1 vez
	if (populationType == POPULATION_TYPE_DECREASING){
		int newStage = getCompletedRatio() * NStages;
		newStage = min(newStage, NStages - 1);
		if (newStage > currentPopStage){
			reducePopulationSize();
			currentPopStage++;
		}
	}
	if (getGeneration() == 0){
		/*struct timeval actualTime; 
		gettimeofday(&actualTime, NULL);
		start = ((double) (actualTime.tv_sec) * 1.0e6 + (double) (actualTime.tv_usec))/1.0e6;
		entropyInformation.push_back(getPopulationEntropy2());*/
		for (int i = 0; i < population->size(); i++){
			if (mutationType == MUTATION_TYPE_ILS){//Llamamos a la mutacion (que hace local search)
				Individual *best = (*population)[i]->internalClone();
				int notImprovement = 0;
				for (int j = 0; j < ILSIterations; j++){
					(*population)[i]->mutation(pm);//ILS
					//OJO: algunas mutaciones incorporan la LS, asi que esto habria que comentarlo en esos caso
					//hay que intentar unificar los codigos para que no la hagan, y llamar desde aqui...
					if (localSearchType == LOCAL_SEARCH_ALWAYS){
						(*population)[i]->localSearch();
					}
					if ((*population)[i]->getOptDirection(0) == MINIMIZE){
						if ((*population)[i]->getObj(0) + 0.1 < best->getObj(0)){//OJO sumamos 0.1, tal vez para algun problema esto no sea correcto
							notImprovement = 0;
							delete best;
							best = (*population)[i]->internalClone();
						} else {
							notImprovement++;
						}
					} else {
						if ((*population)[i]->getObj(0) - 0.1 < best->getObj(0)){
							notImprovement = 0;
							delete best;
							best = (*population)[i]->internalClone();
						} else {
							notImprovement++;
						}
					}
				}
				delete (*population)[i];
				(*population)[i] = best;
			}
		}
		best = (*population)[0]->internalClone();
		for (int i = 1; i < population->size(); i++){
			if ((*population)[i]->isBetterEq(*best)){
				delete best;
				best = (*population)[i]->internalClone();
			}
		}
		initialEntropy = getPopulationEntropy();
		if (survivalSelectionType == SURVIVAL_SELECTION_CDRW_GENERATIONAL_PARENT_BINARY_TOURNAMENT){//Calculate distances
			
			populationDistances.insert(populationDistances.begin(), population->size(), vector<double>(population->size(), 0));
			for (int i = 0; i < population->size(); i++){
				for (int j = i + 1; j < population->size(); j++){
					populationDistances[j][i] = populationDistances[i][j] = (*population)[i]->getDistance(*(*population)[j]);
				}
			}
			updateMinPopulationDistances();
		}
	}
	/*struct timeval actualTime; 
	gettimeofday(&actualTime, NULL);
	double t = ((double) (actualTime.tv_sec) * 1.0e6 + (double) (actualTime.tv_usec))/1.0e6;
	if (t - start > lastPrinted + 1){
		entropyInformation.push_back(getPopulationEntropy2());
		lastPrinted += 1;
	}*/

	offSpring.clear();
	int from = 0;
	if ((survivalSelectionType == SURVIVAL_SELECTION_CLEARING_PARENT_RANDOM_ELITIST_WINNERS_ABOVE_AVERAGE) || (survivalSelectionType == SURVIVAL_SELECTION_CLEARING_PARENT_BINARY_TOURNAMENT_ELITIST_WINNERS_ABOVE_AVERAGE)){
		clearParents();
		from = offSpring.size();//Son simples copias, asi que no hace falta reevaluarlos
	}
	createOffSprings();
	evaluateOffspring(from);
	survivalSelection();
	setPopulationSize(population->size());
}

void MonoGA2::sortPopulation(){
	sort(population->begin(), population->end(), sortByObj0);
}

void MonoGA2::randomSelectionWithoutReplacement(int &p1, int &p2){
	p1 = getRandomInteger0_N(population->size() - 1);
	do {
		p2 = getRandomInteger0_N(population->size() - 1);
	} while (p2 == p1);
}

void MonoGA2::binaryTournamentSelection(int &p1, int &p2){//Supone que population esta ordenada
	int opt1 = (int) (((double)(population->size()))*rand()/(RAND_MAX+1.0));
	int opt2 = (int) (((double)(population->size()))*rand()/(RAND_MAX+1.0));
	if ((*population)[opt1]->isBetterEq(*(*population)[opt2])){
		p1 = opt1;
	} else {
		p1 = opt2;
	}
	//do {
		opt1 = (int) (((double)(population->size()))*rand()/(RAND_MAX+1.0));
	//} while (opt1 == p1);
	//do {
		opt2 = (int) (((double)(population->size()))*rand()/(RAND_MAX+1.0));
	//} while(opt2 = p1);
	if ((*population)[opt1]->isBetterEq(*(*population)[opt2])){
		p2 = opt1;
	} else {
		p2 = opt2;
	}
}

//Empareja de manera aleatoria sin considerar fitness
void MonoGA2::createOffspringWithRandomSelectedParents(){
	if(parentIndexes.size() == 0){
		for (int i = 0; i < population->size(); i++){
			parentIndexes.push_back(i);
		}
	}
	random_shuffle(parentIndexes.begin(), parentIndexes.end());
	for (int i = 0; i < getPopulationSize(); i+=2){
		int p1 = parentIndexes[i];
		int p2 = parentIndexes[i+1];
		Individual *of1 = (*population)[p1]->internalClone();
		Individual *of2 = (*population)[p2]->internalClone();
		double vcross = rand() / (RAND_MAX + 1.0);	
		if (vcross < pc) {  
			of1->crossover(of2);
			if (localSearchType == LOCAL_SEARCH_ALWAYS){
				of1->localSearch();
				of2->localSearch();
			} 
		}
		//TODO: ILS
		of1->mutation(pm);
		if (localSearchType == LOCAL_SEARCH_ALWAYS){
			of1->localSearch();
		}
		of2->mutation(pm);
		if (localSearchType == LOCAL_SEARCH_ALWAYS){
			of2->localSearch();
		}
		offSpring.push_back(of1);
		offSpring.push_back(of2);
	}
}

void MonoGA2::parentSelection(int &p1, int &p2){
	if ((survivalSelectionType == SURVIVAL_SELECTION_SS) || (survivalSelectionType == SURVIVAL_SELECTION_GENERATIONAL_ELIT) || (survivalSelectionType == SURVIVAL_SELECTION_REPLACEWORST_FROMPARENTANDOFFSPRING) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_COMPLETE) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PROBABILISTIC_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_SCALED_PROBABILISTIC_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CLEARING_PARENT_BINARY_TOURNAMENT_ELITIST_WINNERS_ABOVE_AVERAGE) || (survivalSelectionType == SURVIVAL_SELECTION_OBECTIVESCOMBINED_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CENTRAL_PART_OF_FRONT_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CDRW_GENERATIONAL_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_BEST_NO_DIVERSITY_PENALIZED_PARENT_BINARY_TOURNAMENT)){
		binaryTournamentSelection(p1, p2);
	} else if ((survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PARENT_RANDOM) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PROBABILISTIC_PARENT_RANDOM) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_SCALED_PROBABILISTIC_PARENT_RANDOM) || (survivalSelectionType == SURVIVAL_SELECTION_CLEARING_PARENT_RANDOM_ELITIST_WINNERS_ABOVE_AVERAGE)){	
		randomSelectionWithoutReplacement(p1, p2);
	} else {
		cerr << "Error interno: no se reconoce el survivalSelectionType" << endl;
	}
}

void MonoGA2::createOffSpring(){//Crear 2 hijos
	int p1, p2;
	parentSelection(p1, p2);
	Individual *of1 = (*population)[p1]->internalClone();
	Individual *of2 = (*population)[p2]->internalClone();
	double vcross = rand() / (RAND_MAX + 1.0);
	if (vcross < pc) {  
		of1->crossover(of2);
		//Ojo este no tengo claro si dejarlo o no...
		/*if (localSearchType == LOCAL_SEARCH_ALWAYS){
			of1->localSearch();
			of2->localSearch();
		} */
	}
	if (mutationType == MUTATION_TYPE_ILS){
		Individual *best = of1->internalClone();
		int notImprovement = 0;
		for (int i = 0; i < ILSIterations; i++){
			of1->mutation(pm);
			if (localSearchType == LOCAL_SEARCH_ALWAYS){
				of1->localSearch();
			}
			if (of1->getOptDirection(0) == MINIMIZE){
				if (of1->getObj(0) + 0.1 < best->getObj(0)){//OJO sumamos 0.1, tal vez para algun problema esto no sea correcto
					notImprovement = 0;
					delete best;
					best = of1->internalClone();
				} else {
					notImprovement++;
				}
			} else {
				if (of1->getObj(0) - 0.1 < best->getObj(0)){
					notImprovement = 0;
					delete best;
					best = of1->internalClone();
				} else {
					notImprovement++;
				}
			}
		}
		delete of1;
		of1 = best;

		best = of2->internalClone();
		notImprovement = 0;
		for (int i = 0; i < ILSIterations; i++){
			of2->mutation(pm);
			if (localSearchType == LOCAL_SEARCH_ALWAYS){
				of2->localSearch();
			}
			if (of2->getOptDirection(0) == MINIMIZE){
				if (of2->getObj(0) + 0.1 < best->getObj(0)){//OJO sumamos 0.1, tal vez para algun problema esto no sea correcto
					notImprovement = 0;
					delete best;
					best = of2->internalClone();
				} else {
					notImprovement++;
				}
			} else {
				if (of2->getObj(0) - 0.1 < best->getObj(0)){
					notImprovement = 0;
					delete best;
					best = of2->internalClone();
				} else {
					notImprovement++;
				}
			}
		}
		delete of2;
		of2 = best;
	} else {//Mutacion normal
		Individual *copyOf1 = of1->internalClone();
		of1->mutation(pm);
		if (localSearchType == LOCAL_SEARCH_ALWAYS){
			of1->localSearch();
		}
		if (!of1->isBetterEq(*copyOf1)){
			delete of1;
			of1 = copyOf1;
		} else {
			delete copyOf1;
		}
		Individual *copyOf2 = of2->internalClone();
		of2->mutation(pm);
		if (localSearchType == LOCAL_SEARCH_ALWAYS){
			of2->localSearch();
		}
		if (!of2->isBetterEq(*copyOf2)){
			delete of2;
			of2 = copyOf2;
		} else {
			delete copyOf2;
		}
	}
	offSpring.push_back(of1);
	offSpring.push_back(of2);
}

void MonoGA2::createOffSprings(){
	if (survivalSelectionType == SURVIVAL_SELECTION_SS){//Crear un unico hijo
		createOffSpring();
		delete(offSpring[1]);
		offSpring.pop_back();
	} else if ((survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PROBABILISTIC_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_SCALED_PROBABILISTIC_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PARENT_RANDOM) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PROBABILISTIC_PARENT_RANDOM) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_SCALED_PROBABILISTIC_PARENT_RANDOM)){//Crea dos hijos
		createOffSpring();
	} else if ((survivalSelectionType == SURVIVAL_SELECTION_GENERATIONAL_ELIT) || (survivalSelectionType == SURVIVAL_SELECTION_REPLACEWORST_FROMPARENTANDOFFSPRING) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_COMPLETE)){//Crea N hijos
		int desiredPopulationSize =  getPopulationSize();
		if (populationType == POPULATION_TYPE_SAW_TOOTH){
			desiredPopulationSize = meanPopSize + D - 2.0 * D / (T - 1) * ((getGeneration() - T * ((getGeneration() - 1) / T) - 1));
		}
		if (desiredPopulationSize > population->size()){
			int insertions = desiredPopulationSize - population->size();
			for (int i = 0; i < insertions; i++){
				offSpring.push_back((*population)[0]->internalClone());
				offSpring.back()->restart();
			}
		}
		if (survivalSelectionType == SURVIVAL_SELECTION_GENERATIONAL_ELIT){//Elitism: pone al mejor de los padres en la población de hijos
			//Buscamos el mejor
			int bestIndex = 0;
			for (int i = 1; i < population->size(); i++){
				if ((*population)[i]->isBetterEq(*(*population)[bestIndex])){
					bestIndex = i;
				}
			}
			offSpring.push_back((*population)[bestIndex]->internalClone());	
		}
		while(offSpring.size() < desiredPopulationSize){
			createOffSpring();
		}
		while(offSpring.size() > desiredPopulationSize){
			delete offSpring.back();
			offSpring.pop_back();
		}
	} else if ((survivalSelectionType == SURVIVAL_SELECTION_CLEARING_PARENT_RANDOM_ELITIST_WINNERS_ABOVE_AVERAGE) || (survivalSelectionType == SURVIVAL_SELECTION_CLEARING_PARENT_BINARY_TOURNAMENT_ELITIST_WINNERS_ABOVE_AVERAGE) || (survivalSelectionType == SURVIVAL_SELECTION_OBECTIVESCOMBINED_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CENTRAL_PART_OF_FRONT_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CDRW_GENERATIONAL_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_BEST_NO_DIVERSITY_PENALIZED_PARENT_BINARY_TOURNAMENT)){//Crea n hijos
		//Los preseleccionados por clearing ya están en el offSpring
		while(offSpring.size() < getPopulationSize()){
			createOffSpring();
		}
	} else if ((survivalSelectionType == SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5_PROBABILISTIC) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5_SCALED_PROBABILISTIC) || (survivalSelectionType == SURVIVAL_SELECTION_DIVERSITY_ADAPTIVE_GENERALIZED_CROWDING)){
		createOffspringWithRandomSelectedParents();
	} else {
		cerr << "Error interno: survivalSelectionType no reconocido" << endl;
		exit(-1);
	}
	if (offSpring.size() > getPopulationSize()){//Tal vez deberiamos quitar al peor, o al menos ver si supera el best, pues se pierde algo de tiempo en crear uno para "tirarlo"
		delete(offSpring[offSpring.size()-1]);	
		offSpring.pop_back();
	}

	if (localSearchType == LOCAL_SEARCH_RANDOM){//Ojo: en el caso generacional elitista podria hacer LS sobre un individuo que ya es local
		int lsSize = offSpring.size() * lsProp;
		vector<int> selected;
		selected.reserve(offSpring.size());
		for (int i = 0; i < offSpring.size(); i++){
			selected.push_back(i);
		}
		random_shuffle(selected.begin(), selected.end());
		for (int i = 0; i < lsSize; i++){
			offSpring[selected[i]]->localSearch();
		}
	} else if (localSearchType == LOCAL_SEARCH_BEST){
		int lsSize = offSpring.size() * lsProp;
		sort(offSpring.begin(), offSpring.end(), sortByObj0);
		for (int i = 0; i < lsSize; i++){
			offSpring[i]->localSearch();
		}
	}
}

double MonoGA2::getPopulationEntropy(){
	double entropy = 0;
	int genes = (*population)[0]->getNumberOfVar();
	for (int i = 0; i < genes; i++){
		map<int, int> repetitions; //Asumimos que son numeros enteros porque de todas formas si lo hacemos con numeros reales habra problemas
		for (int j = 0; j < getPopulationSize(); j++){
			repetitions[(*population)[j]->getVar(i)]++;
		}
		double genEntropy = 0;
		for (map<int, int>::iterator it = repetitions.begin(); it != repetitions.end(); it++){
			double times = (it->second);
			double prob = (times) / getPopulationSize();
			genEntropy += (prob * log(prob) / log(alleles));
		}
		entropy -= genEntropy;
	}
	return entropy / genes;
}

double MonoGA2::getPopulationEntropy2(){
	double entropy = 0;
	vector<int> genes, alleles;
	(*population)[0]->getGenesAndAlleles(genes, alleles);
	int g = genes.size();
	for (int i = 0; i < g; i++){
		map<int, int> repetitions; //Asumimos que son numeros enteros porque de todas formas si lo hacemos con numeros reales habra problemas
		for (int j = 0; j < getPopulationSize(); j++){
			(*population)[j]->getGenesAndAlleles(genes, alleles);
			repetitions[genes[i]]++;
		}
		double genEntropy = 0;
		for (map<int, int>::iterator it = repetitions.begin(); it != repetitions.end(); it++){
			double times = (it->second);
			double prob = (times) / getPopulationSize();
			genEntropy += (prob * log(prob) / log(alleles[i]));
		}
		entropy -= genEntropy;
	}
	return entropy / g;
}

void MonoGA2::survivalSelection(){
	if (survivalSelectionType == SURVIVAL_SELECTION_SS){
		//Buscamos el peor
		int worstIndex = 0;
		for (int i = 1; i < getPopulationSize(); i++){
			if (!(*population)[i]->isBetterEq(*(*population)[worstIndex])){
				worstIndex = i;
			}
		}
		if (offSpring[0]->isBetterEq(*(*population)[worstIndex])){
			delete (*population)[worstIndex];
			(*population)[worstIndex] = offSpring[0];
		} else {
			delete offSpring[0];
		}
	} else if ((survivalSelectionType == SURVIVAL_SELECTION_GENERATIONAL_ELIT) || (survivalSelectionType == SURVIVAL_SELECTION_CLEARING_PARENT_RANDOM_ELITIST_WINNERS_ABOVE_AVERAGE) || (survivalSelectionType == SURVIVAL_SELECTION_CLEARING_PARENT_BINARY_TOURNAMENT_ELITIST_WINNERS_ABOVE_AVERAGE)){//El elitismo se hizo pasando al mejor a los offspring
		for (int i = 0; i < population->size(); i++){
			delete (*population)[i];
		}
		(*population) = offSpring;
		/*for (int i = 0; i < getPopulationSize(); i++){
			if (i < population->size()){
				delete (*population)[i];
				(*population)[i] = offSpring[i];
			} else {//Con clearing se reduce population
				population->push_back(offSpring[i]);
			}
		}*/
	} else if (survivalSelectionType == SURVIVAL_SELECTION_REPLACEWORST_FROMPARENTANDOFFSPRING){
		//Nota este replace worst no es el que esta explicado en el libro de Eiben
		//Aqui nos estamos "cargando" los peores entre padres e hijos
		//En el libro de Eiben dice que se generan nu hijos (< n) y se borran los nu peores
		//padres.
		//Habria que hacer otro operador mas y a este llamarlo RPELACEWORSTFROMPARENTSANDOFFSPRING
		//Al otro lo podemos llamar GENITOR
		//Nota: en la pagina 27 si llama replace worst a un esquema en que se eligen los mejores entre
		//padres e hijos, pero puede llevar a confusion...
		//Tb. se podria considerar sin clones
		//En cualquier caso, este operador incluso con las modificaciones no es bueno a largo plazo
		for (int i = 0; i < offSpring.size(); i++){
			population->push_back(offSpring[i]);
		}
		sortPopulation();
		for (int i = 0; i < offSpring.size(); i++){
			delete ((*population)[population->size()-1]);
			population->pop_back();
		}
	} else if (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_COMPLETE){
		for (int i = 0; i < offSpring.size(); i++){
			double minDistance = DBL_MAX;
			int nearestIndex = 0;
			for (int j = 0; j < population->size(); j++){
				double dist = (*population)[j]->getDistance(*(offSpring[i]));
				if (dist < minDistance){
					minDistance = dist;
					nearestIndex = j;
				}
			}
			if ((offSpring)[i]->isBetterEq(*(*population)[nearestIndex])){
				delete (*population)[nearestIndex];
				(*population)[nearestIndex] = (offSpring)[i];
			} else {
				delete (offSpring)[i];
			}
		}
	} else if ((survivalSelectionType == SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5_PROBABILISTIC) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5_SCALED_PROBABILISTIC) || (survivalSelectionType == SURVIVAL_SELECTION_DIVERSITY_ADAPTIVE_GENERALIZED_CROWDING)){
		//Cada hijo se empareja con uno de sus padres, intentando minimizar la suma de las distancias entre ellos
		double entropy, scaleFactor;
		if (survivalSelectionType == SURVIVAL_SELECTION_DIVERSITY_ADAPTIVE_GENERALIZED_CROWDING){
			entropy = getPopulationEntropy();
			scaleFactor = initialScaleFactor * entropy / initialEntropy;
		}
		for (int i = 0; i < getPopulationSize(); i+=2){
			double sum1 = (*population)[parentIndexes[i]]->getDistance(*(offSpring)[i]) + (*population)[parentIndexes[i+1]]->getDistance(*(offSpring)[i+1]);
			double sum2 = (*population)[parentIndexes[i]]->getDistance(*(offSpring)[i+1]) + (*population)[parentIndexes[i+1]]->getDistance(*(offSpring)[i]);
			int p[2], o[2];
			if (sum1 < sum2){//Los empareja sin cruzar
				p[0] = parentIndexes[i];
				o[0] = i;
				p[1] = parentIndexes[i+1];
				o[1] = i + 1;
			} else {//Los empareja cruzados
				p[0] = parentIndexes[i];
				o[0] = i + 1;
				p[1] = parentIndexes[i+1];
				o[1] = i;
			}
			if (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5){
				for (int j = 0; j <= 1; j++){
					if ((offSpring[o[j]]->isBetterEq(*(*population)[p[j]]))){
						delete (*population)[p[j]];
						(*population)[p[j]] = (offSpring)[o[j]];
					} else {
						delete (offSpring[o[j]]);
					}
				}
			} else if (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5_PROBABILISTIC){
				for (int j = 0; j <= 1; j++){
					double fitnessO = offSpring[o[j]]->getObj(0);
					double fitnessP = (*population)[p[j]]->getObj(0);
					double r = generateRandomDouble0_Max(fitnessO + fitnessP);
					if (r < fitnessO){
						delete (*population)[p[j]];
						(*population)[p[j]] = (offSpring)[o[j]];
					} else {
						delete (offSpring[o[j]]);
					}
				}
			} else if (survivalSelectionType ==  SURVIVAL_SELECTION_DIVERSITY_ADAPTIVE_GENERALIZED_CROWDING){//Ojo: se requiere que los objetivos sean positivos
				for (int j = 0; j <= 1; j++){
					double fitnessO;
					double fitnessP;
					if ((*population)[0]->getOptDirection(0) == MAXIMIZE){
						fitnessO = offSpring[o[j]]->getObj(0);
						fitnessP = (*population)[p[j]]->getObj(0);
					} else {
						if ((offSpring[o[j]]->getObj(0) <= 0) || ((*population)[p[j]]->getObj(0) <= 0)){
							cerr << "Error: no se admiten objetivos <= 0" << endl;
							exit(-1);
						}
						fitnessO = 1.0 / offSpring[o[j]]->getObj(0);
						fitnessP = 1.0 / (*population)[p[j]]->getObj(0);
					}
					double r = generateRandomDouble0_Max(1);
					double offspringProb = 0;
					if (fitnessO > fitnessP){//Probabilidiad del offspring f(c)/(f(c) + scaleFactor * f(p))
						offspringProb = fitnessO / (fitnessO + scaleFactor * fitnessP);
					} else if (fitnessO < fitnessP){
						offspringProb = scaleFactor * fitnessO / (scaleFactor * fitnessO + fitnessP);
					} else {
						offspringProb = 0.5;
					}
					if (offspringProb >= r){//Selecciona al hijo
						delete (*population)[p[j]];
						(*population)[p[j]] = (offSpring)[o[j]];
					} else {
						delete (offSpring[o[j]]);
					}
				}
			} else if (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_MAHFOUD_A5_SCALED_PROBABILISTIC){
				//En este caso estamos haciendo una adaptacion. En el scaled probabilistic, se necesita definir el best de alguna forma
				//En el paper donde se propone, se integra con RESTRICTED_TOURNAMENT y el best es el mejor entre los hijos y el grupo elegido
				//Lo más parecido creo que es identificar el mejor entre los padres y los hijos
				Individual *bestInd = offSpring[o[0]];
				if (offSpring[o[1]]->isBetterEq(*bestInd)){
					bestInd = offSpring[o[1]];
				}
				if ((*population)[p[0]]->isBetterEq(*bestInd)){
					bestInd = (*population)[p[0]];
				}
				if ((*population)[p[1]]->isBetterEq(*bestInd)){
					bestInd = (*population)[p[1]];
				}
				double bestValue = bestInd->getObj(0);

				if ((*population)[0]->getOptDirection(0) == MAXIMIZE){
					for (int j = 0; j <= 1; j++){
						double numerator1 = bestValue - offSpring[o[j]]->getObj(0);
						double numerator2 = bestValue - (*population)[p[j]]->getObj(0);
						if (numerator1 + numerator2 <= 0){//Se puede dar si ambos tienen el optimo actual
							numerator1 = numerator2 = 1;
						}
						double r = generateRandomDouble0_Max(numerator1 + numerator2);
						if (r < numerator2){
							delete (*population)[p[j]];
							(*population)[p[j]] = (offSpring)[o[j]];
						} else {
							delete (offSpring[o[j]]);
						}
					}
				} else {//Minimize: suponemos que el objetivo siempre es positivo
					if ( (offSpring[o[0]]->getObj(0) <= 0) || (offSpring[o[1]]->getObj(0) <= 0) || ((*population)[p[0]]->getObj(0) <= 0) || ((*population)[p[1]]->getObj(0) <= 0)){
						cerr << "El objetivo no puede tomar valores negativos" << endl << flush;
						exit(-1);
					}
					for (int j = 0; j <= 1; j++){
						double numerator1 = 1.0/ bestValue - 1.0 / offSpring[o[j]]->getObj(0);
						double numerator2 = 1.0/ bestValue - 1.0 / (*population)[p[j]]->getObj(0);
						if (numerator1 + numerator2 <= 0){//Se puede dar si ambos tienen el optimo actual
							numerator1 = numerator2 = 1;
						}
						double r = generateRandomDouble0_Max(numerator1 + numerator2);
						if (r < numerator2){
							delete (*population)[p[j]];
							(*population)[p[j]] = (offSpring)[o[j]];
						} else {
							delete (offSpring[o[j]]);
						}
					}
				}
			} 
		} 
	} else if ((survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PARENT_RANDOM) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PROBABILISTIC_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PROBABILISTIC_PARENT_RANDOM) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_SCALED_PROBABILISTIC_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_SCALED_PROBABILISTIC_PARENT_RANDOM) ){
	
		double bestObj = offSpring[0]->getObj(0);
		if ( ((offSpring[1]->getObj(0) < bestObj) && (offSpring[1]->getOptDirection(0) == MINIMIZE)) || ((offSpring[1]->getObj(0) > bestObj) && (offSpring[1]->getOptDirection(0) == MAXIMIZE))){
			bestObj = offSpring[1]->getObj(0);
		}
		for (int i = 0; i <= 1; i++){//Para cada hijo creado
			//Elegimos w padres sin repeticion (en el paper no queda claro si es con repeticion o sin repeticion)
			//Los padres elegidos los vamos colocando al final de la poblacion, y asi no repetimos
			//El mas cercano de todos queda colocado al final
			//Para el segundo hijo el ultimo se descarta, ya que es el elegido para el primer hijo (o el propio hijo
			//si se produjo sustitucion)
			double minDistance = DBL_MAX;
			for (int j = 0; j < w; j++){
				if ((getPopulationSize() -1 - i - j) >= 0){//Si queda donde elegir. En el caso con w = N, para le segundo hijo solo se eligen N-1
					int random = getRandomInteger0_N(getPopulationSize() -1 - i - j);
					if ( (((*population)[random]->getObj(0) < bestObj) && ((*population)[random]->getOptDirection(0) == MINIMIZE)) || (((*population)[random]->getObj(0) > bestObj) && ((*population)[random]->getOptDirection(0) == MAXIMIZE))){
						bestObj = (*population)[random]->getObj(0);
					}
					//Ponemos el elegido aleatoriamente al final de la población
					swap((*population)[random], (*population)[getPopulationSize() - 1 - i - j]);
					double dist = offSpring[i]->getDistance(*(*population)[getPopulationSize() - 1 - i - j]);
					if (dist < minDistance){
						swap((*population)[getPopulationSize() - 1 - i - j], (*population)[getPopulationSize() - 1 - i]);
						minDistance = dist;
					}
				}
			}
			//El mas cercano queda en population[population.size() - 1 - i]
			if ((survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PARENT_RANDOM)){
				if (offSpring[i]->isBetterEq(*(*population)[getPopulationSize() - 1 - i])){
					delete ((*population)[getPopulationSize() - 1 - i]);
					(*population)[getPopulationSize() - 1 - i] = offSpring[i];
				} else {
					delete (offSpring[i]);
				}
			} else if ((survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PROBABILISTIC_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_PROBABILISTIC_PARENT_RANDOM)){
				double fitnessO = offSpring[i]->getObj(0);
				double fitnessP = (*population)[getPopulationSize() - 1 - i]->getObj(0);
				double r = generateRandomDouble0_Max(fitnessO + fitnessP);
				if (r < fitnessO){
					delete ((*population)[getPopulationSize() - 1 - i]);
					(*population)[getPopulationSize() - 1 - i] = offSpring[i];
				} else {
					delete (offSpring[i]);
				}
			} else if ((survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_SCALED_PROBABILISTIC_PARENT_BINARY_TOURNAMENT) || (survivalSelectionType == SURVIVAL_SELECTION_CROWDING_RESTRICTED_TOURNAMENT_SELECTION_SCALED_PROBABILISTIC_PARENT_RANDOM)){
				//Lo del bestObj no quedo claro que la forma en que se esta manejando sea la adecuada, de todas formas, estos probabilisticos no se han aplicado y en principio no parece
				//que de buenos resultados
				double numerator1 = bestObj - offSpring[i]->getObj(0);
				double numerator2 = bestObj - (*population)[getPopulationSize() - 1 - i]->getObj(0);
				if (numerator1 + numerator2 <= 0){//Se puede dar si ambos tienen el optimo actual
					numerator1 = numerator2 = 1;
				}
				double r = generateRandomDouble0_Max(numerator1 + numerator2);
				if (r < numerator2){
					delete (*population)[getPopulationSize() - 1 - i];
					(*population)[getPopulationSize() - 1 - i] = (offSpring)[i];
				} else {
					delete (offSpring[i]);
				}
			}
		}
	} else if (survivalSelectionType == SURVIVAL_SELECTION_OBECTIVESCOMBINED_PARENT_BINARY_TOURNAMENT){
		//Unimos padres e hijos
		for (int i = 0; i < offSpring.size(); i++){
			(*population).push_back(offSpring[i]);
		}
		offSpring.clear();

		//Reservamos memoria para almacenar el rango de fitness original y asignamos id a cada individuo
		for (int i = 0; i < (*population).size(); i++){
			(*population)[i]->setAuxDataSize(3);//Rango de fitness, SumDist, ID
			(*population)[i]->setAuxData(2, i);
		}

		//Calculamos todas las distancias, y las almacenamos para no repetir calculos
		vector< vector < double > > orderedDistances(population->size(), vector<double>(population->size(), 0));
		for (int i = 0; i < population->size(); i++){
			for (int j = i + 1; j < population->size(); j++){
				double distance = (*population)[i]->getDistance(*(*population)[j]);
				orderedDistances[i][j] = distance;
				orderedDistances[j][i] = distance;
			}
		}

		while((*population).size() != getPopulationSize()){//Eliminamos uno en cada paso hasta llegar al tam. deseado
			sort(population->begin(), population->end(), sortByObj0);//Ordenamos por fitness original (los mejores al principio)

			for (int i = 0; i < population->size(); i++){//Asignamos el rango (de size a 1)
				(*population)[i]->setAuxData(0, population->size() - i);
			}

			vector< vector < double > > distances(population->size());//Calculamos todas las distancias entre los que quedan (usando la matriz original)
			for (int i = 0; i < population->size(); i++){
				for (int j = i + 1; j < population->size(); j++){
					double distance = orderedDistances[(*population)[i]->getAuxData(2)][(*population)[j]->getAuxData(2)];
					/*safety check
					double distance2 = (*population)[i]->getDistance(*(*population)[j]);
					if (distance != distance2){
						cerr << "Error interno!!!!!!!!!!!!" << endl;
						cerr << "Son: " << distance << " y " << distance2 << endl;
						cerr << "Indices: " << (*population)[i]->getAuxData(2) << " y " << (*population)[j]->getAuxData(2) << endl;
						exit(-1);
					}*/
					distances[i].push_back(distance);
					distances[j].push_back(distance);
				}
			}

			for (int i = 0; i < population->size(); i++){//Calculamos la suma de distancias a los NClose mas cercanos
				sort(distances[i].begin(), distances[i].end());//Ordena de menor a mayor
				double sumDist = 0;
				for (int j = 0; j < NClose; j++){
					sumDist += distances[i][j];
				}
				if (distances[i][0] == 0){//Tiene un clon
					(*population)[i]->setAuxData(1, -1);
				} else {
					(*population)[i]->setAuxData(1, sumDist);
				}
			}
			auxIndex = 1;

			sort(population->begin(), population->end(), sortByAux);//Ordena de forma creciente por contribucion de diversidad (los primeros contribuyen menos)
			double worstFitness = DBL_MAX;
			int currentWorstIndex = 0;

			for (int i = 0; i < population->size(); i++){
				double fit;
				if ((*population)[i]->getAuxData(1) != -1){//Normal
					fit = (*population)[i]->getAuxData(0) + (1 - (double)(NElit)/population->size()) * (i + 1);
				} else {//Tiene clon
					fit = -1;
				}
				if (fit < worstFitness){
					worstFitness = fit;
					currentWorstIndex = i;
				} else if (fit == worstFitness){
					if ((*population)[currentWorstIndex]->isBetterEq(*(*population)[i])){
						currentWorstIndex = i;
					}
				}
			}
			//Eliminamos currentWorstIndex
			delete (*population)[currentWorstIndex];
			(*population)[currentWorstIndex] = (*population)[(*population).size()-1];
			population->resize(population->size()-1);
		}
	} else if (survivalSelectionType == SURVIVAL_SELECTION_CENTRAL_PART_OF_FRONT_PARENT_BINARY_TOURNAMENT){
		//Join population and offspring
		for (int i = 0; i < offSpring.size(); i++){
			population->push_back( (offSpring)[i] );
		}

		int inserted = 0;
		vector < vector < Individual * > > fronts;
		vector < Individual *> notClassified;
		vector< Individual* > survivors;
		vector<int> indexesBest(1, 0);
		double currentBestValue = (*population)[0]->getObj(0);
		//Select the individual with best "original" objective
		for (int i = 1; i < population->size(); i++){
			if ((*population)[i]->getObj(0) == currentBestValue){
				indexesBest.push_back(i);
			} else if ( ((*population)[0]->getOptDirection(0) == MINIMIZE) && ((*population)[i]->getObj(0) < currentBestValue)){
				indexesBest.clear();
				indexesBest.push_back(i);
				currentBestValue = (*population)[i]->getObj(0);
			} else if ( ((*population)[0]->getOptDirection(0) == MAXIMIZE) && ((*population)[i]->getObj(0) > currentBestValue)){
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
			//Calculate fronts
			int differentsInFront0;
			vector<int> nonDominatedIndexes;
			markNonDominatedNoRep(*population, nonDominatedIndexes);
			//Select a random individual from the first front
			int selectedIndex = nonDominatedIndexes[generateRandom(0, nonDominatedIndexes.size() - 1)];
			survivors.push_back((*population)[selectedIndex]);
			//cout << "Selecciona: " << survivors[survivors.size()-1]->getObj(0) << endl;
			//Recover original objective
			survivors[survivors.size()-1]->setObj(0, survivors[survivors.size()-1]->getAuxData(0));
			//Remove element from population. 
			(*population)[selectedIndex] = (*population)[population->size() - 1];
			population->resize(population->size() - 1);
		}
		//Eliminate remaining individuals
		for (int i = 0; i < population->size(); i++){
			delete (*population)[i];
			(*population)[i] = survivors[i];
		}
	} else if (survivalSelectionType == SURVIVAL_SELECTION_BEST_NO_DIVERSITY_PENALIZED_PARENT_BINARY_TOURNAMENT){
		//Nota: este se podria implementar de mejor forma sin la utilizacion de los modulos de multiobjectivizacion
		//pero por por implementarlo rapido por ahora se deja asi
		//Join population and offspring
		for (int i = 0; i < offSpring.size(); i++){
			population->push_back( (offSpring)[i] );
		}

		int inserted = 0;
		vector < vector < Individual * > > fronts;
		vector < Individual *> notClassified;
		vector< Individual* > survivors;
		vector<int> indexesBest(1, 0);
		double currentBestValue = (*population)[0]->getObj(0);
		//Select the individual with best "original" objective
		for (int i = 1; i < population->size(); i++){
			if ((*population)[i]->getObj(0) == currentBestValue){
				indexesBest.push_back(i);
			} else if ( ((*population)[0]->getOptDirection(0) == MINIMIZE) && ((*population)[i]->getObj(0) < currentBestValue)){
				indexesBest.clear();
				indexesBest.push_back(i);
				currentBestValue = (*population)[i]->getObj(0);
			} else if ( ((*population)[0]->getOptDirection(0) == MAXIMIZE) && ((*population)[i]->getObj(0) > currentBestValue)){
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
			//Buscamos el mejor (los cercanos estan penalizados y por tanto con mal objetivo)
			vector<int> indexesBest(1, 0);
			double currentBestValue = (*population)[0]->getObj(0);
			double currentBestDistance = (*population)[0]->getObj(1);
			//Select the individual with best "original" objective
			for (int i = 1; i < population->size(); i++){
				if (((*population)[i]->getObj(0) == currentBestValue) && ((*population)[i]->getObj(1) == currentBestDistance)){
					indexesBest.push_back(i);
				} else if (((*population)[0]->getOptDirection(0) == MINIMIZE) && (((*population)[i]->getObj(0) < currentBestValue) ||
				(((*population)[i]->getObj(0) == currentBestValue) && ((*population)[i]->getObj(1) > currentBestDistance)))){
					indexesBest.clear();
					indexesBest.push_back(i);
					currentBestValue = (*population)[i]->getObj(0);
					currentBestDistance = (*population)[i]->getObj(1);
				} else if (((*population)[0]->getOptDirection(0) == MAXIMIZE) && (((*population)[i]->getObj(0) > currentBestValue) ||
				(((*population)[i]->getObj(0) == currentBestValue) && ((*population)[i]->getObj(1) > currentBestDistance)))){
					indexesBest.clear();
					indexesBest.push_back(i);
					currentBestValue = (*population)[i]->getObj(0);
					currentBestDistance = (*population)[i]->getObj(1);
				}
			}
			int selectedIndex = indexesBest[generateRandom(0, indexesBest.size() - 1)];
			survivors.push_back((*population)[selectedIndex]);
			survivors[survivors.size()-1]->setObj(0, survivors[survivors.size()-1]->getAuxData(0));//Recuperar valor, por si estaba penalizado
			(*population)[selectedIndex] = (*population)[population->size() - 1];
			population->resize(population->size() - 1);
		}
		//Eliminate remaining individuals
		for (int i = 0; i < population->size(); i++){
			delete (*population)[i];
			(*population)[i] = survivors[i];
		}
	} else if (survivalSelectionType == SURVIVAL_SELECTION_CDRW_GENERATIONAL_PARENT_BINARY_TOURNAMENT){
		//Buscamos a los individuos que son peores
		for (int i = 0; i < offSpring.size(); i++){
			int worstIndex = -1;
			double worstContrib = DBL_MAX;
			for (int j = 0; j < population->size(); j++){
				if (!((*population)[j]->isBetterEq(*offSpring[i]))){
					if (minPopulationDistances[j] < worstContrib){
						worstIndex = j;
						worstContrib = minPopulationDistances[j];
					}
				}
			}
			bool changed = false;
			if (worstIndex != -1){
				double minDistance = DBL_MAX;
				for (int j = 0; j < population->size(); j++){
					if (j != worstIndex){
						minDistance = min(minDistance, (*population)[j]->getDistance(*offSpring[i]));
					}
				}
				if (minDistance > worstContrib){
					changed = true;
					delete (*population)[worstIndex];
					(*population)[worstIndex] = offSpring[i];
					for (int j = 0; j < population->size(); j++){
						populationDistances[worstIndex][j] = populationDistances[j][worstIndex] = (*population)[worstIndex]->getDistance(*(*population)[j]);
					}
					updateMinPopulationDistances();
				}
			} 
			if (!changed){//Replace worst strategy
				int worstIndex = 0;
				//Look for the worst
				for (int j = 1; j < population->size(); j++){
					if ((*population)[worstIndex]->isBetterEq(*(*population)[j])){
						worstIndex = j;
					}
				}
				if (offSpring[i]->isBetterEq(*(*population)[worstIndex])){
					delete (*population)[worstIndex];
					(*population)[worstIndex] = offSpring[i];
					for (int j = 0; j < population->size(); j++){
						populationDistances[worstIndex][j] = populationDistances[j][worstIndex] = (*population)[worstIndex]->getDistance(*(*population)[j]);
					}
					updateMinPopulationDistances();
				} else {
					delete (offSpring)[i];
				}
			}
		}
	} else {
		cerr << "Error interno: survivalSelectionType no reconocido" << endl;
		exit(-1);
	}
}

void MonoGA2::getSolution(MOFront *p) {
	//double ent = getPopulationEntropy();
	//cout << ent << endl;
	p->insert(best);
}

void MonoGA2::printInfo(ostream &os) const {
	os << "MonoObjective Genetic Algorithm"  << endl;
	os << "Number of Evaluations = " << getEvaluations() << endl;
	os << "Mutation Probability = " << pm << endl;
	os << "Crossover Probability = " << pc << endl;
	os << "Population Size = " << getPopulationSize() << endl;
	os << "Survival Selection Type = " << survivalSelectionType << endl;
	/*for (int i = 0; i < entropyInformation.size(); i++){
		os << "Entropy: " << i * 1 << " " << entropyInformation[i] << endl;
	}*/
}

void MonoGA2::multiObjectivize(vector <Individual*> *p, vector<Individual*> *off, vector<Individual*> *reference){
	int start = getSampleInd()->getNumberOfObj() - getNumMultiObjectivizationPlugins();
	for (int i = start; i < getSampleInd()->getNumberOfObj(); i++){
		getMultiObjectivizationPlugin(i - start)->convertToMultiObjective(p, off, reference, i, getGeneration(), getCompletedRatio());
	}
}
