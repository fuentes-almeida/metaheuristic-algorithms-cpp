#include "Sudoku.h"

extern "C" {
	Individual *maker(){
		return new Sudoku();
	}
}
