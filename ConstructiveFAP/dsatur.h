#ifndef DSATUR_H_INCLUDED
#define DSATUR_H_INCLUDED

void FSFAP_Evaluator(int **Weights, int** Neighbours,int t_num,int* solution);
void GenerateGraph(char* filename,int **Weights,int **Neighbours,int t_num);
int EdgeSelection(int *Sat_degree, int** Neighbours,int t_num);
int FrequencyAssignment(int imax,int freq_num,int t_num,int **Assgnmt_Map,int *Transmitters, int**Neighbours, int**Weights);
void UpdateGraph(int imax,int fmin, int t_num, int freq_num,int**Weights, int** Neighbours, int **Assgnmt_Map, int* Sat_degree, int* Transmitters);
void DSATURoptimization(int *Transmitters, int **Weights,int **Neighbours, int t_num, int freq_num);

#endif // DSATUR_H_INCLUDED
