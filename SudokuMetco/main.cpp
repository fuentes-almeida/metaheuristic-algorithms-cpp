#include <iostream>
#include <vector>
#include <string>
#include "Sudoku.h"
#include <time.h>

using namespace std;

int main(int argc, char **argv)
{
    srand(time(NULL));
    vector <string> params;
    params.push_back(argv[1]);

    Sudoku S1;
    S1.init(params);
    S1.restart();
    S1.evaluate();
    //S1.dependentMutation(1.0);



    cout << "Hello world!" << endl;
    return 0;
}
