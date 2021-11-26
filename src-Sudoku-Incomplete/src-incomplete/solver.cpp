/***********************************************************************************
 * AUTORES
 *   - Carlos Segura Gonzalez
 * 
 * DESCRIPCION
 * 	 - Es el main del programa. Simplemente lee los argumentos y va creando los objetos necesarios (Sudoku, MonoEA
 * 	 PlainText) y finalmente llama al metodo run del MonoEA
 *
 * ******************************************************************************************************************/


#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <libgen.h>
#include <iostream>
#include <fstream>
#include <string.h>

#include "Sudoku.h"
#include "MonoEA.h"
#include "Individual.h"
#include "PlainText.h"
#include "MultiObjectivization.h"

#define MINIMUM_ARGS 5
#define ARG_OUTPUTPATH          1
#define ARG_OUTPUTFILE          2
#define ARG_CRITSTOPVALUE       3
#define ARG_PRINTPERIOD         4
#define ARG_SEED                5

void argumentError (char *programName) {		
	cout << "Correct usage: " << programName << " outputPath outputFile critStopSeconds printPeriod seed parametros_algoritmo [! parametros_problema] " << endl;
 	exit (-1);
}

int main (int argc, char *argv[]) {
	//Llamada correcta al programa
	if (argc < MINIMUM_ARGS + 1){
		argumentError(argv[0]);
  }

	// Initialize parameters
	string outputPath = argv[ARG_OUTPUTPATH];
	vector<string> outputPrinterParams(1, (outputPath + "/" + argv[ARG_OUTPUTFILE]));
	OutputPrinter *outputPrinter = new PlainText();
	outputPrinter->init(outputPrinterParams);
	double critStopValue = atof(argv[ARG_CRITSTOPVALUE]);
	int printPeriod      = atoi(argv[ARG_PRINTPERIOD]);
	srand(atoi(argv[ARG_SEED]) + 1);
	//Parametros algoritmo
	vector<string> algorithmArgs;
	int currentArg;
	for (currentArg = 6; currentArg < argc; currentArg++){
		if (argv[currentArg][0] == '!'){
			break;
		}
		algorithmArgs.push_back(argv[currentArg]);
	}
	//Parametros problema
	vector<string> problemArgs;
	if ((currentArg != argc) && (argv[currentArg][0] == '!')){
		currentArg++;
		for (; currentArg < argc; currentArg++){
			problemArgs.push_back(argv[currentArg]);
		}
	}

	//cargamos individuo
	Sudoku ind; 
	if (!ind.init(problemArgs)){
		cout << "Error en la inicializacion del Sudoku" << endl;
		exit(-1);
	}

	// Initiate the corresponding EA
	EA *ga = new MonoEA();
	ga->setSampleInd(&ind);
	ga->init(algorithmArgs);
	ga->setStoppingCriterion(critStopValue);
	ga->setPrintPeriod(printPeriod);
	//ga->setMultiObjectivizationPlugins(vector<MultiObjectivization *>(1, mo));
	ga->setOutputPrinter(outputPrinter);
	// Runs the evolurionary process
	ga->run();

	// Program Output
	outputPrinter->printSolution(ga, true);
	outputPrinter->finish();

	//Finishing
	delete (ga);
	exit(EXIT_SUCCESS);
}
