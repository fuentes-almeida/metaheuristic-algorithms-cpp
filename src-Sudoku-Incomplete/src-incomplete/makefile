CPP = g++ 
OPT = -Wall -pedantic

solver: EA.o Individual.o MonoEA.o MultiObjNearestIndDist.o PlainText.o Sudoku.o Sudoku_Problem.o utils.o Sudoku.h MonoEA.h Individual.h PlainText.h MultiObjectivization.h OutputPrinter.h EA.h 
	$(CPP) $(OPT) -O2 -o $@ EA.o Individual.o MonoEA.o MultiObjNearestIndDist.o PlainText.o Sudoku.o Sudoku_Problem.o utils.o solver.cpp

EA.o: EA.cpp EA.h Individual.h OutputPrinter.h MultiObjectivization.h
	$(CPP) $(OPT) -O2 -c -o $@ EA.cpp

Individual.o: Individual.cpp Individual.h MultiObjectivization.h
	$(CPP) $(OPT) -O2 -c -o $@ Individual.cpp

MonoEA.o: MonoEA.cpp MonoEA.h Individual.h EA.h MultiObjectivization.h OutputPrinter.h utils.h MultiObjNearestIndDist.h
	$(CPP) $(OPT) -O2 -c -o $@ MonoEA.cpp

MultiObjNearestIndDist.o: MultiObjNearestIndDist.cpp MultiObjNearestIndDist.h EA.h MultiObjectivization.h Individual.h
	$(CPP) $(OPT) -O2 -c -o $@ MultiObjNearestIndDist.cpp

PlainText.o: PlainText.cpp PlainText.h Individual.h MultiObjectivization.h OutputPrinter.h
	$(CPP) $(OPT) -O2 -c -o $@ PlainText.cpp

Sudoku.o: Sudoku.cpp Sudoku.h Individual.h MultiObjectivization.h utils.h Sudoku_Problem.h
	$(CPP) $(OPT) -O2 -c -o $@ Sudoku.cpp

Sudoku_Problem.o: Sudoku_Problem.cpp Sudoku_Problem.h utils.h
	$(CPP) $(OPT) -O2 -c -o $@ Sudoku_Problem.cpp

utils.o: utils.cpp utils.h Individual.h MultiObjectivization.h
	$(CPP) $(OPT) -O2 -c -o $@ utils.cpp

clean:
	rm solver EA.o Individual.o MonoEA.o MultiObjNearestIndDist.o PlainText.o Sudoku.o Sudoku_Problem.o utils.o
