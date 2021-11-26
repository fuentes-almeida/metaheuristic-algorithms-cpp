/***********************************************************************
* Autor: Carlos Segura González
* Fecha: 28/10/12
* Descripcion: clase para la representación de una instancia del Sudoku 
************************************************************************/

#ifndef _SUDOKU_PROBLEM_H_
#define _SUDOKU_PROBLEM_H_

#include <vector>
#include <map>
#include <string>
#include "utils.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <set>
#include <string.h>


using namespace std;

class Sudoku_Problem {
	public:
		void initiallize(const string &extraPenaltyStr, const string &instanceFile);
		vector<int> getDimensionChoices(int i);//Obtener los datos no usados en un bloque
		int getDimensionChoicesSize(int i);

		int getPenalizations(const vector<int> *data);
		void print(ostream &out, const vector<int> *data) const;
	private:
		bool useExtraPenalty;
		
		vector<int> unusedBlock[9];//Numeros no usados en cada bloque
		vector<int> fixedDataColumn[9];
		vector<int> fixedDataBlock[9];
		vector<int> fixedDataRow[9];

		vector< pair <int, int > > dataRowAll[9];//for printing

		vector< pair <int, int > > dataColumnToGen[9];
		vector< pair<int, int> > dataBlockToGen[9];
		vector< pair <int, int > > dataRowToGen[9];
};


#endif

