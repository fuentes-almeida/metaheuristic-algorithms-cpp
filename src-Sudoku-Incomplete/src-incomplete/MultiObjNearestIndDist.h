/***********************************************************************************
 * AUTHORS
 *   - Carlos Segura González
 * Descripcion
 *   Clase que sirve para incluir un objetivo auxiliar y penalizaciones en un 
 *   problema.
 *   El objetivo auxiliar es el DCN - Distance to Closest Neighbour - y se maneja
 *   una distancia para manejar las penalizaciones. Esta distancia se decrementa
 *   de forma lineal desde distanceInit (al principio de la optimizacion) hasta
 *   distanceEnd al final de la optimizacion.
 *   Si hay un individuo que tiene una distancia a un individuo ya seleccionado
 *   menor que el valor de distancia de referencia, es penalizado, poniendo
 *   su objetivo original a un valor muy grande: esto tiene el efecto con MonoEA
 *   y MultiDyn que no sera seleccionado
 * *********************************************************************************/


#ifndef MULTIOBJNEARESTINDDIST
#define MULTIOBJNEARESTINDDIST

#include "MultiObjectivization.h"

class MultiObjNearestIndDist : public MultiObjectivization {
  public:
	  void convertToMultiObjective(vector<Individual *> *parents, vector<Individual *> *reference,  int ordObj, int numberGeneration, double completeRatio);
		void convertToMultiObjectiveWithRef(vector<Individual *> *population, vector<Individual *> *reference, int ordObj);
	  int getOptDirection() { return MAXIMIZE; }
    bool init(const vector<string> &params);
  private:
		double distance, distanceInit, distanceEnd;
};

#endif
