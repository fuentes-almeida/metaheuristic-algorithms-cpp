#include "Sudoku.h"
#include <limits.h>
#include <set>
#include <iostream>
#include <fstream>
using namespace std;

const SudokuProblem *Sudoku::sudokuProblem=NULL;
int Sudoku::nInds = 0;

//constructor de un tablero de Sudoku
SudokuProblem::SudokuProblem(const string &fileName)
{
	ifstream file(fileName.c_str());
	if (!file.is_open()){
		cerr << "File " << fileName << " could not be opened" << endl;
		exit(-1);
	}

	int aux;
	vector<int> row;
	for (int i=0;i<N;i++){
			row.clear();
			for (int j=0;j<N;j++){
				file>>aux;
				row.push_back(aux);
			}
			tablero.push_back(row);
	}
	file.close();

	cout<<"Creacion de tablero"<<endl;
	for (int i=0;i<N;i++){
		for (int j=0;j<N;j++)
			cout<<tablero[i][j]<<" ";
		cout<<endl;}
		cout<<endl;
}

//inicializacion del problema del Sudoku
bool Sudoku::init (const vector<string> &params)
{
	if (params.size() != 1){
		cout << "Parametros incorrectos (fichero_instancia)" << endl;
		return false;
	}
	sudokuProblem = new SudokuProblem(params[0]);

	setNumberOfObj(1);
	setNumberOfVar(N*N);
	return true;
}

//genera un vector de aleatorios del 1 al N
void Sudoku::generateRandomVector()
{
	permutation.clear();
	for (int i=1;i<=N;i++)
		permutation.push_back(i);

	for (int i=0;i<N;i++)
	{
		int aux1=rand()%N;
		int aux2=rand()%N;
		int aux3=permutation[aux1];
		permutation[aux1]=permutation[aux2];
		permutation[aux2]=aux3;
	}

}

// Generacion de individuo aleatorio: se asigna una permutacion a cada bloque del sudoku
void Sudoku::generateRandom()
{
	//copiamos el contenido del tablero al individuo
	for (int i=0;i<N;i++)
		for (int j=0;j<N;j++)
			setVar(i*N+j,sudokuProblem->getValue(i,j));

	//recorremos todos los bloques del individuo
	for (int i=0;i<N;i++)
	{
		//generamos un vector de aleatorios entre 1 y 9
		generateRandomVector();
		//recorremos todas las posiciones del vector
		for (int j=0;j<N;j++)
		{
			int flag=0;
			//verificamos si el valor ya esta en el bloque
			for (int k=0;k<N;k++)
			if (permutation[j]==sudokuProblem->getValue(x(i,k),y(i,k)))
			{
				flag=1;
				break;
			}
			if (flag==1)
				continue;
			//si el valor no esta en el bloque, lo asignamos a una posicion donde haya un cero
			else{
				for (int k=0;k<N;k++)
					if (getVar(x(i,k)*N+y(i,k))==0)
					{
						setVar(x(i,k)*N+y(i,k),permutation[j]);
						break;
					}
				}
		}
	}
    cout<<"Individuo generado"<<endl;
	for (int i=0;i<N;i++){
		for (int j=0;j<N;j++)
			cout<<getVar(i*N+j)<<" ";
		cout<<endl;}
		cout<<endl;
}

//Calcula el valor de fitnes
void Sudoku::calculateFitness(void)
{
	setObj(0,0);
	objective=1;
	//se cuentan las repeticiones en renglones y columnas
	for (int k=0;k<N;k++)
		for (int i=0;i<N;i++)
			for (int j=i+1;j<N;j++)
			{
				//renglones
				if (getVar(k*N+i)==getVar(k*N+j))
				{
					objective+=1;
					Penalty[k*N+i]+=1;
					Penalty[k*N+j]+=1;
					if (sudokuProblem->getValue(k,i)>0 || sudokuProblem->getValue(k,j)>0)
						{
						    objective+=100;
                            Penalty[k*N+i]+=100;
                            Penalty[k*N+j]+=100;
						}
				}
				//columnas
				if (getVar(i*N+k)==getVar(j*N+k))
				{
                    objective+=1;
                    Penalty[i*N+k]+=1;
                    Penalty[j*N+k]+=1;
                    if (sudokuProblem->getValue(i,k)>0 ||sudokuProblem->getValue(j,k)>0)
                        {
                            objective+=100;
                            Penalty[i*N+k]+=100;
                            Penalty[j*N+k]+=100;
                        }

				}

			}
	setObj(0,objective);
	setFitnessValue(objective);
	cout<<objective<<endl;

    cout<<"Penalizaciones:"<<endl;
	for (int i=0;i<N;i++){
		for (int j=0;j<N;j++)
			cout<<Penalty[i*N+j]<<"\t";
		cout<<endl;}
		cout<<endl;
}

//Calculo de la distancia Hamming entre 2 individuos
double Sudoku::getDistance(Individual &ind2)
{
	int actualDist = 0;
	for (int i = 0; i < getNumberOfVar(); i++){
		if ((int)(getVar(i)) != (int)(ind2.getVar(i))){
			actualDist++;
		}
	}
	return actualDist;
}

//Clonacion de un individuo
Individual* Sudoku::clone() const
{
	Sudoku *newInd = new Sudoku();
	newInd->sudokuProblem=sudokuProblem;
	newInd->objective=objective;
	if (objective != getObj(0)){
		cerr << "Error interno: objective y getObj(0) diferentes" << endl;
		exit(-1);
	}
	return newInd;
	cout<<"Individuo clonado"<<endl;
}

//Mutacion
void Sudoku::dependentMutation(double pm)
{
	int index1,index2;
	//recorremos cada bloque
	for (int i=0;i<N;i++)
			if (1.0*rand()/RAND_MAX < pm)
			{
					do { index1=rand()%N; }
					while(sudokuProblem->getValue(x(i,index1),y(i,index1))>0);

					do { index2=rand()%N; }
					while(sudokuProblem->getValue(x(i,index2),y(i,index2))>0 || index1==index2);

					int aux1=getVar(x(i,index1)*N+y(i,index1));
					int aux2=getVar(x(i,index2)*N+y(i,index2));

					setVar(x(i,index1)*N+y(i,index1),aux2);
					setVar(x(i,index2)*N+y(i,index2),aux1);
			}

    cout<<"Individuo mutado"<<endl;
	for (int i=0;i<N;i++){
		for (int j=0;j<N;j++)
			cout<<getVar(i*N+j)<<" ";
		cout<<endl;}
		cout<<endl;
}

//Cruza uniforme de dos individuos por bloques
void Sudoku::dependentCrossover(Individual *ind)
{
	//recorremos todos los bloques haciendo la cruza con prob pm
	for (int block=0;block<N;block++)
		if (1.0*rand()/RAND_MAX < 0.5)
		{
			for (int i=0;i<N;i++)
			{
				int aux1 = getVar(x(block,i)*N+y(block,i));
				int aux2 = ind->getVar(x(block,i)*N+y(block,i));

		    setVar(x(block,i)*N+y(block,i),aux2);
				ind->setVar(x(block,i)*N+y(block,i),aux1);
			}
		}
}

//Reinicio de la poblacion
void Sudoku::restart(void)
{
	generateRandom();
	calculateFitness();
	localSearch();
}

void Sudoku::deserialized()
{
  evaluate();
}

void Sudoku::evaluate(void)
{
	calculateFitness();
}

void Sudoku::localSearch()
{
    int imax,jmax,pmax=0;
    int countmax=0;
    for (int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            if (sudokuProblem->getValue(i,j)==0)
            {
                if (Penalty[i*N+j]>pmax)
                {
                    pmax=Penalty[i*N+j];
                    countmax=0;
                }
                if (Penalty[i*N+j]==pmax)
                    countmax++;
            }

    int selec1=rand()%countmax+1;


    countmax=0;
    for (int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            if (sudokuProblem->getValue(i,j)==0)
            {
                if (Penalty[i*N+j]==pmax && countmax<selec1)
                    countmax++;
                if (countmax==selec1)
                {
                    imax=i;
                    jmax=j;
                    countmax++;
                }
            }

    int conflictBlock=x(imax,jmax);
    int index1=y(imax,jmax);
    int index2;

    do{index2=rand()%N;
    }while (sudokuProblem->getValue(x(conflictBlock,index2),y(conflictBlock,index2))>0 || index2==index1);

    int aux1=getVar(x(conflictBlock,index1)*N+y(conflictBlock,index1));
    int aux2=getVar(x(conflictBlock,index2)*N+y(conflictBlock,index2));

    setVar(x(conflictBlock,index1)*N+y(conflictBlock,index1),aux2);
    setVar(x(conflictBlock,index2)*N+y(conflictBlock,index2),aux1);

    cout<<"Individuo mejorado"<<endl;
	for (int i=0;i<N;i++){
		for (int j=0;j<N;j++)
			cout<<getVar(i*N+j)<<" ";
		cout<<endl;}
		cout<<endl;
}
