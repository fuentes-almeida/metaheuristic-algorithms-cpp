/***********************************************************************************
 * AUTORES
 *   - Carlos Segura González
 ************************************************************************************/

#include "Individual.h"
#include <stdlib.h>

// Clone operators
Individual* Individual::internalClone(void) const{
	Individual *nuevo = clone();
	nuevo->internalClone(this);
	return nuevo;
}

void Individual::internalClone(const Individual *ind){
	multiObjectivizationsPlugins = ind->multiObjectivizationsPlugins;
	setNumberOfObj(ind->getNumberOfObj());
	for (unsigned int i = 0; i < ind->obj.size(); i++)
		obj[i] = ind->obj[i];
	setAuxDataSize(ind->auxData.size());
	for (unsigned int i = 0; i < ind->auxData.size(); i++)
		auxData[i] = ind->auxData[i];
}

double Individual::getDistance(Individual &ind2){//calculate euclidean distance
	cout << "Error: getDistance llamado sin reimplementar" << endl;
	exit(-1);
	return 0;
}

bool Individual::isBetterEq(const Individual &ind2) const {
	if (getOptDirection(0) == MINIMIZE){
		return getObj(0) <= ind2.getObj(0);
	} else {
		return getObj(0) >= ind2.getObj(0);
	}
}

//Este metodo devuelve la direccion de otpimizacion de cada objetivo
//Es posible que algunos objetivos sean ficticios, es decir, objetivos
//auxiliares intregrados a traves de una multiobjectivizacion.
//En funcion de por que objetivo se pregunte, llama al getOptDirection
//del hijo o al de la clase que representa el objetivo auxiliar
unsigned int Individual::getInternalOptDirection(const int i) const {
	int realObjectives = getNumberOfObj() - multiObjectivizationsPlugins.size();
	if (i < realObjectives){
		return getOptDirection(i);
	} else {
		return multiObjectivizationsPlugins[i - realObjectives]->getOptDirection();
	}
}

