/***********************************************************************************
 * AUTHORS
 *   - Carlos Segura González
 * 
 * *********************************************************************************/


#ifndef MULTIOBJNEARESTINDDISTTHRESHOLDPOPPERCENTREFERENCE
#define MULTIOBJNEARESTINDDISTTHRESHOLDPOPPERCENTREFERENCE

#define LINEAR 1
#define LINEAR_PIECEWISE 2
#include "MultiObjectivization.h"

class MultiObjNearestIndDistThresholdPopPercentReference : public MultiObjectivization {
  public:
	  void convertToMultiObjective(vector<Individual *> *parents, vector<Individual *> *offSprings, vector<Individual *> *reference,  int ordObj, int numberGeneration, double completeRatio);
		void convertToMultiObjectiveWithRef(vector<Individual *> *population, vector<Individual *> *reference, int ordObj, double completeRatio);
	  int getOptDirection() { return MAXIMIZE; }
    bool init(const vector<string> &params);
		void setParameters(vector<double> &parameters);
  private:
    double pThreshold, pThresholdInit, pThresholdEnd;
		double distance, distanceInit, distanceEnd;
		double p1, p2;
		int DUpdateType;
};

#endif
