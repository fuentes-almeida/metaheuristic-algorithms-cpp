/***********************************************************************
* Autor: Carlos Segura González
* Fecha: 28/04/12
* Descripcion: clase para la representacion de un individuo del sudoku
* Se redujo la funcionalidad para su presentacion en la Escuela de
* Computo Evolutivo 2016.
* En esta version solo se admite la representacion basada en
* permutaciones de los bloques.
************************************************************************/

#ifndef _SUDOKU_INDIVIDUAL_H_
#define _SUDOKU_INDIVIDUAL_H_

#include "Individual.h"
#include "Sudoku_Problem.h"

class Sudoku : public Individual {
	public:
		Sudoku(){}
		~Sudoku(){}
		void restart();//generate a random solution
		void evaluate();
		Individual *clone() const;
		void print(ostream &out) const;
		void localSearch();
		bool init(const vector<string> &params);
		void mutation(double pm);
		void crossover(Individual *i2);
		double getDistance(Individual &ind2) ;
		unsigned int getOptDirection(int obj) const { return MINIMIZE; }
	private:

		static Sudoku_Problem *instance;
		vector < int > data[9];//Cada vector representa es una permutacion que representa la soluciones para cada bloque
};

#endif
