/*********************************************************************************************
 * AUTHORS
 *   Carlos Segura González
 *
 * DESCRIPTION
 *   Plugin base class to represent score algorithms which are executed by the slave islands 
 *********************************************************************************************/


#ifndef LOCAL_SCORE_ALGORITHM_H
#define LOCAL_SCORE_ALGORITHM_H

#include "ScoreAlgorithm.h"
#include "MOFront.h"

#define SCORE_WITH_NONDOM 1
#define SCORE_WITH_POPULATION 2

class LocalScoreAlgorithm : public ScoreAlgorithm {
	public:
		LocalScoreAlgorithm();
		virtual ~LocalScoreAlgorithm() {}
		virtual long double score(MOFront *);//Executed in slaves with the archive
		virtual long double score(vector<Individual*> *); //Executed in slaves with the population
		virtual int getScoreType() const = 0;
		virtual void updateScore(const long double breakFrom, const long double arriveTo, const int numImprovements, int alg, int conf, int islandId) = 0; //Executed in Coordinator 
		virtual long double estimateScore(const long double breakFromScore, const int alg, const int conf, const int islandId, const double rndValue, const int numberOfConfigurations) = 0; // Executed in Coordinator
};

#endif
