#include <iostream>
#include <stdlib.h>

using namespace std;

int getRandomInteger0_N(int n){
	return (int) ((n + 1.0)*rand()/(RAND_MAX+1.0));
}

int main(int argc, char *argv[]){
	if (argc != 6){
		cout << "Error: xSize ySize numbers creationRatio seed" << endl;
		return -1;
	}
	int xsize = atoi(argv[1]);
	int ysize = atoi(argv[2]);
	int numbers = atoi(argv[3]);
	double createRatio = atof(argv[4]);
	int seed = atoi(argv[5]);
	srandom(seed);
	cout << xsize << " " << ysize << " " << numbers << endl;
	for (int n1 = 0; n1 < numbers; n1++){
		for (int n2 = 0; n2 < numbers; n2++){
			double r = (double)(random()) / RAND_MAX;
			if (createRatio > r){
				int value = getRandomInteger0_N(999999); 
				cout << n1 << " " << n2 << " " << value << endl;
			}
		}
	}
	cout << -1 << endl;

}
