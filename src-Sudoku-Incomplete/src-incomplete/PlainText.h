/************************************************************************************************
 * AUTHORS
 *    Carlos Segura González
 *
 * DESCRIPTION
 *   - Clase para guardar la salida a fichero en formato de texto plano
 ************************************************************************************************/


#ifndef __PLAIN_TEXT_H__
#define __PLAIN_TEXT_H__

#include <vector>

#include "Individual.h"
#include "OutputPrinter.h"

using namespace std;

class PlainText : public OutputPrinter {
	public:
		virtual bool init(const vector<string> &params);
		virtual void printSolution(EA *, bool end);
		void print(const string &str);
		virtual void finish();
	private:
		ofstream outputFile;
};
#endif
