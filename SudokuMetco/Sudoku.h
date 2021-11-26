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

    // Imprimir el individuo
    //friend ostream& operator<< (ostream &os, const Sudoku &SudokuIndividual);
    void dependentCrossover(Individual *ind);
    void localSearch(); // Este metodo simplemente tiene una llamada a localSearch1 o 2, en funcion de la que queramos usar
  private:
    // Generacion aleatoria de un individuo
    void generateRandom();
    void generateRandomVector();

    // Obtenencion de numero asignado a una posicion
		//inline int getGrid(const int x, const int y) const { return (int) getVar(y * N + x); }
		//inline void setGrid(const int x, const int y, const int value) { setVar(y * N + x, value); }

    // Actualiza el fitness y num. de aparicion de parajes, considerando los valores en
		// las casillas (x1,y1) y (x2,y2)
		//void update(const int x1, const int y1, const int x2, const int y2);
		//void updatePartial(const int x1, const int y1, const int x2, const int y2);

    // Busquedas locales


    // Busqueda local basada en cambiar 1 unico elemento cada vez
    //void initLocalSearch1();
    //void localSearch1();
    //void endLocalSearch1();
    //bool improvePosition(const int x, const int y);

    // Busqueda local basada en cambiar 2 elementos contiguos cada vez
    //void initLocalSearch2();
    //void localSearch2();
    //void endLocalSearch2();
    //bool improvePosition(const Position &p1, const Position &p2);
    //bool improvePosition(const Position &p1, const Position &p2, const Position &p3);

    // Mutacion uniforme: cambia cada elemento con una determinada probabilidad
    //void mutationUniform();
    //void mutationUniformFixedSize(double pm);
    //void mutationUniformValid();
    // Mutacion basada en ventanas
    //void mutationWindow();
    //Reordenacion: no hay cambio de valores
    //void mutationReorder();

    //Crossover
    //void subStringCrossover(TwoDPack *ind);


    // Obtencion del coste asociado a hacer 1 asignacion
    //int getAssignationCost(const int x, const int y, const int value);

    // Obtencion del coste asociado a hacer 2 asignaciones
    //int getAssignationCost(const int x1, const int y1, const int v1, const int x2, const int y2, const int v2);
    //int getAssignationCost(const int x1, const int y1, const int v1, const int x2, const int y2, const int v2, const int x3, const int y3, const int v3);

    // Obtencion de coste asociado a cualquier asignacion de 1 casilla
    //void getAssignationCosts(const int x, const int y, vector<int> &costValues);

    // Suprimir una asignacion
    //int removeAssignation(const int x, const int y);

    // Hacer una asignacion
    //void makeAssignation(const int x, const int y, const int value);

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
