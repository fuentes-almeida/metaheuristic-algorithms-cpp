/***********************************************************************************
 * AUTORES
 *   - Carlos Segura González
 * 
 * DESCRIPCION
 *   - Clase para gestionar la salida del programa. Se debe heredar e implementar
 *   los 4 metodos virtuales
 **********************************************************************************/


#ifndef __OUTPUT_PRINTER_H_
#define __OUTPUT_PRINTER_H_

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>
 
#include "EA.h"

using namespace std;

class EA;

class OutputPrinter {
public:
	virtual bool init(const vector<string> &params) = 0;
	virtual void printSolution(EA *ga, bool end) = 0;
	virtual void print(const string &str) = 0;
	virtual void finish() = 0;
};

#endif
