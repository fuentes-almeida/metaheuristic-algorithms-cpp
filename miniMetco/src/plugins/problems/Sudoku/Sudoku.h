#ifndef SUDOKU_H
#define SUDOKU_H

#include "Individual.h"
#include "sys/time.h"

#define N 9
#define x(a,b) (3*(a/3)+b/3)
#define y(a,b) (3*(a%3)+b%3)

class SudokuProblem{
public:
  SudokuProblem(const string &fileName);
  int inline getValue(const int val1, const int val2) const { return tablero[val1][val2]; }
private:
  //doble arreglo que contiene los datos del tablero
  vector<vector<int> > tablero;
};

class Sudoku : public Individual{
  public:
    //Constructores y destructores
    Sudoku (){
        nInds++;
        Penalty= new int[N*N];
        }
    virtual ~Sudoku(){
        nInds--;
        delete [] Penalty;
        }

    //Metodos de Individual
    bool init (const vector<string> &params);
		void evaluate (void);
		void restart(void);
    void deserialized();
		void dependentMutation( double pm );
		//void maintainOnlyObj();

    double inline getMinimum(const int i) const { return 0; }
    double inline getMaximum(const int i) const { return 0; }
    unsigned int inline getOptDirection(const int i) const { return MINIMIZE; }
    Individual* clone() const;

    double getDistance(Individual &ind2) ;


  private:
    // Generacion aleatoria de un individuo
    void generateRandom();
    void generateRandomVector();

    // Busquedas locales
    void localSearch(); // Este metodo simplemente tiene una llamada a localSearch1 o 2, en funcion de la que queramos usar

    //Crossover
    void dependentCrossover(Individual *ind);

    // Calcular el fitness
    void calculateFitness();
    //void calculatePartialFitness(const int initX, const int endX, const int initY, const int endY);

    static const SudokuProblem *sudokuProblem;
    static int nInds;
    vector <int> permutation;
    double mutProb;
    double objective;//Lo mantenemos como copia, pues algunos algoritmos modifican obj0, por considerarlo fitness
    int *Penalty;
};

#endif
