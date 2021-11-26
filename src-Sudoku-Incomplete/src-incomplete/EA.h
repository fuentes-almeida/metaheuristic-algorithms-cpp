/****************************************************************************************
 * AUTORES
 *   - Carlos Segura Gonzalez
 * 
 * Descripcion 
 *   Clase base de la que heredan los algoritmos evolutivos especificos
 *   Provee metodos relacionados con el criterio de parada, creacion de la poblacion
 *   inicial, impresion de resultados intermedios y el metodo run, que ejecuta 
 *   generaciones llamando al metodo runGeneration que se debe implementar 
 *   en la clase hija.
 *
 *   Para instanciar un algoritmo evolutivo se debe heredar de esta clase e implementar
 *   los siguientes metodos:
 *	 - void runGeneration() = 0;
 *	 		Este metodo debe ejecutar una generacion del algoritmo
 *
 *	 - virtual bool init(const vector<string> &params) = 0;
 *	    Realiza la inicializacion del algoritmo. Recibe los parametros que el usuario pase
 *	    en la linea de comandos
 *
 ****************************************************************************************/

#ifndef __EA_H__
#define __EA_H__

#include <vector>
#include <string>
#include "Individual.h"
#include "OutputPrinter.h"
#include "MultiObjectivization.h"

using namespace std;

class OutputPrinter;

class EA {
	public:
		EA();
		virtual ~EA (void);
		void run();//Ejecucion secuencial
		bool hasFinished();
		void setPopulationSize          (const unsigned int pSize) { this->pSize = pSize;                                     }
		void setSampleInd               (Individual *ind)     { sampleInd = ind;                                         }
		void setOutputPrinter           (OutputPrinter *op)   { outputPrinter = op;                                      }
		void setPrintPeriod             (const int pp)        { printPeriod = nextPrint = pp;                            }
		void setMultiObjectivizationPlugins (const vector<MultiObjectivization*> &multi) { this->multiObjectivizationsPlugins = multi; }
		void setStoppingCriterion       (const double cr)     { critStopValue = cr; }
		inline unsigned int getPopulationSize (void) const             { return pSize;                                  }
		inline Individual* getIndividual           (int i)const             { return (*population)[i];                       }
		inline int getGeneration                   (void) const             { return currentGeneration;                      }
		inline Individual *getSampleInd            (void) const             { return sampleInd;                              } 
		inline int getNextPrint                    (void) const             { return nextPrint;                              }
		inline MultiObjectivization* getMultiObjectivizationPlugin (int index) const { return multiObjectivizationsPlugins[index];    }
		inline int getNumMultiObjectivizationPlugins(void) const            { return  multiObjectivizationsPlugins.size(); }
		int getCurrentPopulationSize() { return population->size(); }
		double getCompletedRatio();
		double getElapsedTime();
		virtual bool init(const vector<string> &params) = 0;

	protected:
		vector<Individual*>* population;
		OutputPrinter *outputPrinter;
	
	private:
		void runGenerationInc();
		virtual void runGeneration() = 0;
		void fillPopWithNewIndsAndEvaluate();
		
		int generations, currentGeneration;
		unsigned int pSize;
		double critStopValue;
		double startTime;
		vector<MultiObjectivization*> multiObjectivizationsPlugins;
		void checkPrint();
		Individual *sampleInd;
		int printPeriod;
		int nextPrint;
};

#endif
