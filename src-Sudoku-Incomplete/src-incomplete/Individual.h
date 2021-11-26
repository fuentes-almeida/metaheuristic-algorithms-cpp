/***********************************************************************************
 * AUTORES
 *   - Carlos Segura Gonzalez
 * 
 * DESCRIPCION
 *   Clase padre para la representacion de individuos (problemas)
 *   
 *   --> Pasos en la creacion de un individuo
 *     - El usuario al heredar debe implementar los metodos evaluate, init, clone
 *     mutation y crossover
 *       
 *       +virtual void mutation (double pm)
 *         El metodo mutation debe realizar una mutacion sobre el propio 
 *         individuo.
 *
 *       +virtual void crossover (Individual *ind)
 *          El metodo de crossover permite la creacion de 2 nuevos individuos
 *          a partir de otros 2 individuos. Los 2 individuos originales son
 *          el individuo que se pasa como parametro y el propio individuo (this)
 *          Los 2 nuevos individuos generados se "guardan" sobre el
 *          individuo que se pasa como parametro y sobre el propio individuo.
 *
 *       +virtual bool init(const vector<string> &params)
 *          Este metodo se encarga de inicializar el individuo a partir de los
 *          parametros pasados por linea de comandos.
 *          Ademas de las inicializaciones propias del individuo se deben 
 *          realizar las siguientes operaciones:
 *            - Fijar el numero de objetivos: setNumberOfObj(int)
 *          Retorna un valor booleano:
 *          - True: la inicializacion fue correcta.
 *          - False: cualquier tipo de error. Se aboarta la ejecucion.
 *
 *       +virtual void evaluate (void)
 *          Este metodo se encarga de calcular los valores objetivos a partir de
 *          los valores de las variables del individuo. 
 *          Los objetivos los debe almacenar en el vector obj.
 *          Otra opcion es que tras mutar y hacer cruce se evalue de forma automatica,
 *          en ese caso, no hay que hacer nada en el evaluate.
 *
 *       +virtual Individual* clone (void)
 *          Este metodo sirve para realizar la clonacion de un individuo. El usuario
 *          debe crear un individuo de la clase correspondiente y devolverlo, haciendo
 *          copia de cualquier atributo interno.
 *          Los valores de los objetivos son copiados de forma automatica.
 *
 *        +virtual void restart()
 *           Este metodo se usa para crear nuevos individuos en la poblacion inicial. 
 *
 *        +virtual void print(ostream &os)
 *           Este metodo por defecto imprime los valores de las variables del individuo
 *           seguido de los valores de los objetivos.
 *           En caso de querer mostrar por pantalla algun dato mas del individuo o
 *           en otro formato distinto, se puede sobreescribir este metodo
 *    - Opcionalmente se pueden sobreescribir los siguientes metodos:
 *
 *        +virtual double getDistance(Individual &ind2);
 *           Debe retornar la distancia entre el individuo actual e ind2. Solo es 
 *           necesario implementarlo si el algoritmo de optimizacion que se va a usar
 *           lo requiere
 *        
 **************************************************************************************/


#ifndef __INDIVIDUAL_H__
#define __INDIVIDUAL_H__

#include <iostream>
#include <vector>
#include <string>
 
#include "MultiObjectivization.h"

using namespace std;

class MultiObjectivization;

#define MINIMIZE 0
#define MAXIMIZE 1

class Individual {
	public:
		Individual (void) { }
		virtual ~Individual (void) {};
		virtual bool init(const vector<string> &params) = 0;
		virtual void evaluate (void) = 0;
		unsigned int inline   getNumberOfObj      (void)        const { return obj.size();           }
		long double inline    getObj              (const int i) const { return obj[i];               }
		unsigned int inline   getNumberOfAuxData  (void)        const { return auxData.size();       }
		double inline         getAuxData          (const int i) const { return auxData[i];           }
		unsigned int inline   getAuxDataSize      (void)        const { return auxData.size();       }
		inline MultiObjectivization* getMultiObjectivizationPlugin (int index) const { return multiObjectivizationsPlugins[index]; }
		inline int getNumMultiObjectivizationPlugins(void) const            { return  multiObjectivizationsPlugins.size(); }

		// Inline Setters
		void inline setObj              (const int i, const long double value) { obj[i] = value;       }
		void inline setAuxData          (const int i, const double value) { auxData[i] = value;        }
		void inline setNumberOfObj      (const int n)                     { obj.resize(n);             }
		void inline setAuxDataSize      (const int n)                     { auxData.resize(n);         }
		void setMultiObjectivizationPlugins (const vector<MultiObjectivization*> &multi) { this->multiObjectivizationsPlugins = multi; }

		// Clone of individuals 
		Individual *internalClone(void) const;
		void internalClone(const Individual *);
		bool isBetterEq(const Individual &ind2) const ;
		unsigned int getInternalOptDirection(const int i) const ;
	
		// Impresion de los individuos 
		virtual void print(ostream &os) const = 0;

		virtual double getDistance(Individual &ind2);
		virtual void restart(void) = 0;
		virtual void mutation (double pm) = 0;
		virtual void crossover (Individual *ind) = 0;
		virtual void localSearch() = 0;

	private:

		virtual unsigned int  getOptDirection     (const int i) const = 0;
		virtual Individual* clone (void) const = 0; // Operador de clonacion: debe ser reimplementado en la clase hija
		
		vector<MultiObjectivization*> multiObjectivizationsPlugins;
		vector<long double> obj;//Objetivos
		vector<double> auxData;//Datos auxiliares
};

#endif
