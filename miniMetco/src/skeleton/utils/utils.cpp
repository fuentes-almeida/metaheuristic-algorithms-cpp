/***********************************************************************************
 * AUTORES
 *   - Ofelia González Pérez
 *   - Gara Miranda Valladares
 *   - Carlos Segura González
 * 
 * FECHA
 *    Noviembre 2007
 * ********************************************************************************/

#include "float.h"
#include "utils.h"
#include <sys/time.h>
#include <iostream>
#include <sstream>
#include <strings.h>
#include <map>
#include <math.h>

using namespace std;

int objOrder;
// Separar una cadena en conjunto de cadenas usando como separador el espacio
vector<string> splitString(string s){
	vector<string> cadenas;
	int first = -1; 
	int next = 0;
	while(next != (int)string::npos){
		next = s.find(" ", first + 1);
		if ((next != (int)string::npos) && (next != first + 1)){
			cadenas.push_back(s.substr(first + 1, next - first - 1));
		}
		if ((next == (int)string::npos) && (first != (int)s.size() - 1)){
			cadenas.push_back(s.substr(first + 1, s.size() - first - 1));
		}
		first = next;
	}
	return cadenas;
}

// Buscar un string en un array de strings
int getIndex(string strToSearch, const string *array, int arraySize){
	for (int i = 0; i < arraySize; i++)
		if (strcasecmp(array[i].c_str(), strToSearch.c_str()) == 0)
			return (i);
	return -1;
}

// Inicializar la semilla para generacion de numeros aleatorios
void initRandomSeed(int mult){
	struct timeval t1;
	gettimeofday(&t1, NULL);
	srand ((unsigned int)(t1.tv_usec + t1.tv_sec) * mult);
}

void initFixedRandomSeed(int seed){
	srand (seed);
}

/****************************************************************************
*                                                                           *
* RandomUniform generates an equally distributed random number              *
* in the interval [0,1]. For further reference see Press,                   *
* W.H. et alii, Numerical Recipes in C, Cambridge University                *
* Press, 1992.                                                              *
*                                                                           *
*****************************************************************************/

double randomUniform(double minValue,double maxValue)
{
	long j;
	long k;
	static long idum;
	static long idum2=123456789;
	static long iy=0;
	static long iv[NTAB];
	double result;

	if (iy == 0)
		idum = SEED;

	if (idum <= 0)
	{
		if (-idum < 1)
			idum = 1;
		else
			idum = -idum;

		idum2 = idum;

		for (j=NTAB+7; j>=0; j--)
		{
			k = idum / IQ1;
			idum = IA1 * (idum - k*IQ1) - k*IR1;
			if (idum < 0) idum += IM1;
			if (j < NTAB) iv[j] = idum;
		}

		iy = iv[0];
	}

	k = idum / IQ1;
	idum = IA1 * (idum - k*IQ1) - k*IR1;

	if (idum < 0)
		idum += IM1;

	k = idum2 / IQ2;
	idum2 = IA2 * (idum2 - k*IQ2) - k*IR2;

	if (idum2 < 0)
		idum2 += IM2;

	j = iy / NDIV;
	iy = iv[j] - idum2;
	iv[j] = idum;

	if (iy < 1)
		iy += IMM1;

	result = AM * iy;

	if (result > RNMX)
		result = RNMX;

	result = minValue + result * (maxValue - minValue);
	return(result);
}

//Generacion de números aleatorios entre a y b sin repeticion
void generateRandomValues(const int a, const int b, const int n, vector<int> &values){
	vector<int> index;
	values.clear();
	if (n == 0)
		return;
	for (unsigned int i = a; i <= b; i++)
		index.push_back(i);

	if (n > index.size()){
		cerr << "Incorrect usage of generateRandomValues. Can not generate without repetitions" << endl;
		exit(-1);
	}
	for (int i = 0; i < n; i++) {
		int newIndex = (int) (((double)(b-a+1) - i)*rand()/(RAND_MAX+1.0));
		values.push_back(index[newIndex]);
		index[newIndex] = index[index.size()-i-1];
	}
}

int generateRandom(const int minValue, const int maxValue){
	return (int) (((double)(maxValue + 1 - minValue)*rand()/(RAND_MAX+1.0))) + minValue;
}

int getRandomInteger0_N(int n){
	return (int) ((n + 1.0)*rand()/(RAND_MAX+1.0));
}

//Nota se usa un valor truncado de pi porque en el extremo no está definido
double getRandomCauchy(double mean, double scale){
	double p = generateRandomDouble0_Max(1);
	return (mean + scale * tan(3.14159* (p - 0.5)));
}

// Incluido por Antonio J. Nebro
// Modificado por Carlos Segura
// Comprueba si ind1 domina a ind2
// ind1 domina a ind2 si todos sus objetivos son "mejores" (max-min)  
// o iguales y al menos uno de ellos es mejor
int dominanceTest (const Individual *ind1, const Individual *ind2) {
	int flag      ; 
	int dominate1 ;
	int dominate2 ;
	dominate1 = 0 ;
	dominate2 = 0 ;
	
	for(unsigned int i = 0; i < ind1->getNumberOfObj(); i++) {
		if (ind1->getInternalOptDirection(i) == MAXIMIZE){ 
			if (ind1->getObj(i) > ind2->getObj(i)) 
				flag = -1 ;
			else if (ind1->getObj(i) < ind2->getObj(i)) 
				flag = 1 ;
			else 
				flag = 0 ;
		} else {                          // Objetivo a minimizar
			if (ind1->getObj(i) < ind2->getObj(i)) 
				flag = -1;
			else if (ind1->getObj(i) > ind2->getObj(i))
				flag = 1 ;
			else
				flag = 0 ;
		}
		
		if (flag == -1)
			dominate1 = 1 ;
		if (flag == 1)
			dominate2 = 1 ;
	}
    
  if ((dominate1 == 0) && (dominate2 == 0)){//iguales
    return NON_DOMINANCES_EQUALS;  // Non dominated solutions
  }
	if ((dominate1 == 1) && (dominate2 == 1)){//no iguales
		return NON_DOMINANCES_NOT_EQUALS;
	}
  if (dominate1 == 1) {
    return FIRST_DOMINATES; // solution1 dominates
  }
  return SECOND_DOMINATES;    // solution2 dominates
}

void calculateRanks(const vector<Individual*> &pop, vector < vector <int > > &ranks){
	ranks.clear();
	vector<int> *indexesCurrentPop = new vector<int>;
	vector<Individual *> *currentPop = new vector<Individual*>;
	for (int i = 0; i < pop.size(); i++){
		currentPop->push_back(pop[i]);
		indexesCurrentPop->push_back(i);
	}
	vector<bool> dominated;
	do { //Until all individuals are ranked
		vector<Individual *> *newPop;
		vector<int> *indexesNewPop;
		dominated.clear();
		dominated.insert(dominated.begin(), currentPop->size(), false);
		ranks.push_back(vector<int>());
		for (int i = 0; i < currentPop->size(); i++) {
			if (dominated[i]) // i is dominated
				continue;
			for (int j = i+1; j < currentPop->size(); j++) {
				if (dominated[j])
					continue;
				int dominanceInformation = dominanceTest((*currentPop)[j], (*currentPop)[i]);
				if (dominanceInformation == FIRST_DOMINATES){
					dominated[i] = true;
					break;
				} else if (dominanceInformation == SECOND_DOMINATES){
					dominated[j] = true;
				}
			}
		}
		newPop = new vector<Individual*>;
		indexesNewPop = new vector<int>;
		for (int i = 0; i < currentPop->size(); i++){
			if (!dominated[i]){
				ranks[ranks.size()-1].push_back((*indexesCurrentPop)[i]);
			} else{
				newPop->push_back((*currentPop)[i]);
				indexesNewPop->push_back((*indexesCurrentPop)[i]);
			}
		}
		// Update the currentPop
		delete currentPop;
		delete indexesCurrentPop;
		currentPop = newPop;
		indexesCurrentPop = indexesNewPop;
	} while (currentPop->size() != 0);
	delete currentPop;
	delete indexesCurrentPop;
}

void markDominated(const vector<Individual*> &pop, vector<int> &dominatedIndexes){
	dominatedIndexes.clear();
	vector<bool> dominated(pop.size(), false);
	for (int i = 0; i < pop.size(); i++){
		if (dominated[i])
			continue;
		for (int j = i + 1; j < pop.size(); j++){
			if (dominated[j])
				continue;
			int dominance = dominanceTest(pop[i], pop[j]);
			if (dominance == FIRST_DOMINATES){
				dominated[j] = true;
				dominatedIndexes.push_back(j);
			} else if (dominance == SECOND_DOMINATES){
				dominated[i] = true;
				dominatedIndexes.push_back(i);
				break;
			}
		}
	}
}

void markNonDominatedNoRep(const vector<Individual*> &pop, vector<int> &nonDominatedIndexes){
	nonDominatedIndexes.clear();
	vector<bool> dominated(pop.size(), false);
	for (int i = 0; i < pop.size(); i++){
		if (dominated[i])
			continue;
		for (int j = i + 1; j < pop.size(); j++){
			if (dominated[j])
				continue;
			int dominance = dominanceTest(pop[i], pop[j]);//Siempre compara uno menor con uno mayor, en caso que sean iguales, marca solo el segundo como dominado
			if (dominance == FIRST_DOMINATES){
				dominated[j] = true;
			} else if (dominance == SECOND_DOMINATES) {
				dominated[i] = true;
				break;
			} else if (dominance == NON_DOMINANCES_EQUALS){
				dominated[j] = true;
			}
		}
	}
	for (int i = 0; i < pop.size(); i++){
		if (dominated[i] == false){
			nonDominatedIndexes.push_back(i);
		}
	}
}

void markNonDominated(const vector<Individual*> &pop, vector<int> &nonDominatedIndexes){
	nonDominatedIndexes.clear();
	vector<bool> dominated(pop.size(), false);
	for (int i = 0; i < pop.size(); i++){
		if (dominated[i])
			continue;
		for (int j = i + 1; j < pop.size(); j++){
			if (dominated[j])
				continue;
			int dominance = dominanceTest(pop[i], pop[j]);
			if (dominance == FIRST_DOMINATES){
				dominated[j] = true;
			} else if (dominance == SECOND_DOMINATES) {
				dominated[i] = true;
				break;
			} 
		}
	}
	for (int i = 0; i < pop.size(); i++){
		if (dominated[i] == false){
			nonDominatedIndexes.push_back(i);
		}
	}
}


//Clasify at least (front1Size + front1Size*incr)  
void FastNondominatedSort2(vector<Individual *> *p, vector < vector<Individual*> > &fronts, vector<Individual *> &notClassified, int &differentsInFront0, double incr){

	differentsInFront0 = 0;
	int classified = 0;
	fronts.clear();
	notClassified.clear();
	vector < vector < int > > dominates(p->size(), vector<int>());//dominates[i] --> individuals dominated by individual i
	vector < int > dominatedBy(p->size(), 0); //dominatedBy[i] --> i is dominated by such number of individuals
	vector < bool > inserted(p->size(), false);//inserted[i] --> true: individual i has been inserted in a front

	//Assign Identifier to Individuals
	bool spaceAssigned = false;
	int spaceAssignedForId = 0;
	for (int i = 0; i < p->size(); i++){
		int space = (*p)[i]->requestAlgorithmData();
		if (!spaceAssigned){
			spaceAssigned = true;
			spaceAssignedForId = space;
		} else if (space != spaceAssignedForId) {
			cerr << "Internal Error: different space assigned: " << space << " " << spaceAssignedForId << endl;
		}
		(*p)[i]->setAlgorithmData(spaceAssignedForId, i);
	}

	//Calculate F1
	vector<int> equals(p->size(), 1);
	fronts.push_back(vector <Individual *> ());
	for (int i = 0; i < p->size(); i++){
		for (int j = i + 1; j < p->size(); j++){
			int dominanceInfo = dominanceTest((*p)[i], (*p)[j]);
			if (dominanceInfo == FIRST_DOMINATES){
				dominates[i].push_back(j);
				dominatedBy[j]++;
			} else if (dominanceInfo == SECOND_DOMINATES){
				dominates[j].push_back(i);
				dominatedBy[i]++;
			} else if (dominanceInfo == NON_DOMINANCES_EQUALS){
				equals[i]++;
				equals[j]++;
			}
		}
		if (dominatedBy[i] == 0){
			fronts[0].push_back((*p)[i]);
			inserted[i] = true;
			classified++;
		}
	}

	int minClassified = fronts[0].size() * incr;

	//Calculate diffent elements
	map<int, int> count;//cuantos hay que indiquen q tiene n iguales
	for (int i = 0; i < fronts[0].size(); i++){
		count[equals[fronts[0][i]->getAlgorithmData(spaceAssignedForId)]]++;
	}
	//Dividiendo el numero de elementos que indican que tienen n iguales, por n
	//y sumando obtenemos el numero de elementos diferentes
	for (map<int, int>::iterator i = count.begin(); i != count.end(); i++){
		differentsInFront0 += (i->second / i->first);
	}

	
	//Calculate remaining F
	while((fronts[fronts.size() - 1].size() != 0) && (classified < minClassified)){
		fronts.push_back(vector <Individual *>() );
		vector<Individual *> &fi = fronts[fronts.size() - 2];
		for (int i = 0; i < fi.size(); i++){
			for (int j = 0; j < dominates[fi[i]->getAlgorithmData(spaceAssignedForId)].size(); j++){
				dominatedBy[dominates[fi[i]->getAlgorithmData(spaceAssignedForId)][j]]--;
				if (dominatedBy[dominates[fi[i]->getAlgorithmData(spaceAssignedForId)][j]] == 0){
					fronts[fronts.size() - 1].push_back((*p)[dominates[fi[i]->getAlgorithmData(spaceAssignedForId)][j]]);
					inserted[dominates[fi[i]->getAlgorithmData(spaceAssignedForId)][j]] = true;
					classified++;
				}
			}
		}
	}
	if (fronts[fronts.size() - 1].size() == 0){
		fronts.pop_back();
	}
	for (int i = 0; i < p->size(); i++){
		if (inserted[i] == false){
			notClassified.push_back((*p)[i]);
		}
		(*p)[i]->releaseAlgorithmData();
	}
}


//Clasify p in fronts. When at least minN has been classified, the remaining are not classified
void FastNondominatedSort(vector<Individual *> *p, vector < vector<Individual*> > &fronts, vector<Individual *> &notClassified, int &differentsInFront0, int minClassified){

	differentsInFront0 = 0;
	int classified = 0;
	fronts.clear();
	notClassified.clear();
	vector < vector < int > > dominates(p->size(), vector<int>());//dominates[i] --> individuals dominated by individual i
	vector < int > dominatedBy(p->size(), 0); //dominatedBy[i] --> i is dominated by such number of individuals
	vector < bool > inserted(p->size(), false);//inserted[i] --> true: individual i has been inserted in a front

	//Assign Identifier to Individuals
	bool spaceAssigned = false;
	int spaceAssignedForId = 0;
	for (int i = 0; i < p->size(); i++){
		int space = (*p)[i]->requestAlgorithmData();
		if (!spaceAssigned){
			spaceAssigned = true;
			spaceAssignedForId = space;
		} else if (space != spaceAssignedForId) {
			cerr << "Internal Error: different space assigned: " << space << " " << spaceAssignedForId << endl;
		}
		(*p)[i]->setAlgorithmData(spaceAssignedForId, i);
	}

	//Calculate F1
	vector<int> equals(p->size(), 1);
	fronts.push_back(vector <Individual *> ());
	for (int i = 0; i < p->size(); i++){
		for (int j = i + 1; j < p->size(); j++){
			int dominanceInfo = dominanceTest((*p)[i], (*p)[j]);
			if (dominanceInfo == FIRST_DOMINATES){
				dominates[i].push_back(j);
				dominatedBy[j]++;
			} else if (dominanceInfo == SECOND_DOMINATES){
				dominates[j].push_back(i);
				dominatedBy[i]++;
			} else if (dominanceInfo == NON_DOMINANCES_EQUALS){
				equals[i]++;
				equals[j]++;
			}
		}
		if (dominatedBy[i] == 0){
			fronts[0].push_back((*p)[i]);
			inserted[i] = true;
			classified++;
		}
	}

	//Calculate diffent elements
	map<int, int> count;//cuantos hay que indiquen q tiene n iguales
	for (int i = 0; i < fronts[0].size(); i++){
		count[equals[fronts[0][i]->getAlgorithmData(spaceAssignedForId)]]++;
	}
	//Dividiendo el numero de elementos que indican que tienen n iguales, por n
	//y sumando obtenemos el numero de elementos diferentes
	for (map<int, int>::iterator i = count.begin(); i != count.end(); i++){
		differentsInFront0 += (i->second / i->first);
	}

	
	//Calculate remaining F
	while((fronts[fronts.size() - 1].size() != 0) && (classified < minClassified)){
		fronts.push_back(vector <Individual *>() );
		vector<Individual *> &fi = fronts[fronts.size() - 2];
		for (int i = 0; i < fi.size(); i++){
			for (int j = 0; j < dominates[fi[i]->getAlgorithmData(spaceAssignedForId)].size(); j++){
				dominatedBy[dominates[fi[i]->getAlgorithmData(spaceAssignedForId)][j]]--;
				if (dominatedBy[dominates[fi[i]->getAlgorithmData(spaceAssignedForId)][j]] == 0){
					fronts[fronts.size() - 1].push_back((*p)[dominates[fi[i]->getAlgorithmData(spaceAssignedForId)][j]]);
					inserted[dominates[fi[i]->getAlgorithmData(spaceAssignedForId)][j]] = true;
					classified++;
				}
			}
		}
	}
	if (fronts[fronts.size() - 1].size() == 0){
		fronts.pop_back();
	}
	for (int i = 0; i < p->size(); i++){
		if (inserted[i] == false){
			notClassified.push_back((*p)[i]);
		}
		(*p)[i]->releaseAlgorithmData();
	}
}

void crowOrder(vector<Individual*> &pop) {
	double difference;
	
	// Clear the fitness of the individuals
	for (unsigned int i = 0; i < pop.size(); i++)
		(pop)[i]->setFitnessValue(0);
	
	// Calculate the distances (for each objective)
	for (unsigned int i = 0; i < (pop)[0]->getNumberOfObj(); i++) {
		objOrder = i;
		sort(pop.begin(), pop.end(), orderByObjective);
		//En los limities, en caso de igualdad ponemos el que mayor valor actual tenga de fitness
		//Es para tratar un poco mejor las repeticiones 
		//De todas formas, el algoritmo no se comporta bien frente a las repeticiones
		//En algunos articulos se elimina de uno en uno, y no todoas a la vez, para evitar
		//estos problemas (aunque eso conllevaria mas computo)
		for (int j = 1; j < pop.size(); j++){
			if (pop[j]->getObj(i) != pop[0]->getObj(i)){
				break;
			}
			if (pop[j]->getFitnessValue() > pop[0]->getFitnessValue()){
				Individual *tmp = pop[0];
				pop[0] = pop[j];
				pop[j] = tmp;
			}
		}

		for (int j = pop.size() - 2; j >= 0; j--){
			if (pop[j]->getObj(i) != pop[pop.size() - 1]->getObj(i)){
				break;
			}
			if (pop[j]->getFitnessValue() > pop[pop.size()-1]->getFitnessValue()){
				Individual *tmp = pop[pop.size()-1];
				pop[pop.size()-1] = pop[j];
				pop[j] = tmp;
			}
		}
		
		difference = (pop)[pop.size()-1]->getObj(i) - (pop)[0]->getObj(i);
		if (difference == 0)
			difference = 1;
		(pop)[0]->setFitnessValue(DBL_MAX);
		(pop)[pop.size()-1]->setFitnessValue(DBL_MAX);
		for (unsigned int j = 1; j < pop.size()-1; j++) {
			if ((pop)[j]->getFitnessValue() != DBL_MAX) {
				(pop)[j]->setFitnessValue((((pop)[j+1]->getObj(i) - (pop)[j-1]->getObj(i))/(difference)) + (pop)[j]->getFitnessValue() );
			}
		}
	}
	
	// Ordenamos por el valor calculado
	sort(pop.begin(), pop.end(), ordByFitness);
}

// Función auxiliar para ordenación por objetivos
bool orderByObjective(Individual *i1, Individual *i2) {
	return (i1->getObj(objOrder) < i2->getObj(objOrder));
}

void changeFirst(string &str, int n, string &result){
	stringstream newStr;
	bool changed = false;
	for (int i = 0; i < str.size(); i++){
		if ((changed) || (str[i] != '$')){
			newStr << str[i];
		} else {
			changed = true;
			newStr << n;
		}
	}
	result = newStr.str();
}

// Calculate the distance between two individuals
int hammingDistance(const Individual* i1, const Individual* i2) {
   int dist = 0;
   for (unsigned int i = 0; i < i1->getNumberOfVar(); i++) {
      if (i1->getVar(i) != i2->getVar(i)) {
	 dist++;
      }				
   }
   return dist;
}

double generateRandomDouble0_Max(double maxValue){
	return (double)(rand()) / RAND_MAX * maxValue;
}

double generateRandomDoubleMinMax(double min, double max){
	return ((double)(rand()) / RAND_MAX * (max - min)) + min;
}

double z2, lastMean, lastDesv;
bool z2Generated = false;

double generateRandomNormal(double mean, double desv){
	double v1, v2, w, z1;
	if ((z2Generated) && (mean == lastMean) && (desv == lastDesv)){/* use value from previous call */
		z1 = z2;
		z2Generated = false;
	} else {
			do {
				v1 = ((2.0*rand()/(RAND_MAX))) - 1.0;
				v2 = ((2.0*rand()/(RAND_MAX))) - 1.0;
				w = v1 * v1 + v2 * v2;
			} while (( w >= 1.0 ) || (w == 0));

			w = sqrt( (-2.0 * log( w ) ) / w );
			z1 = v1 * w;
			z2 = v2 * w;
			z2Generated = true;
		}
	lastMean = mean;
	lastDesv = desv;
	return( mean + z1 * desv );
}
