#ifndef GENETIC_HPP_INCLUDED
#define GENETIC_HPP_INCLUDED

#define BITS 20

using namespace std;
typedef double (*function)(int,double*);

void copyIndividual(bitset<20> *src, bitset<20> *dst, int d);
void EvaluateFitness(int N, int d, bitset<20> **P, double *Fitness, bitset<20> *Elit, function fx, double *bestResult);
void InitPopulation(int N, int d, bitset<20> **P);
void Selection(int N, int d, bitset<20> **P, bitset<20> **P1, double *Fitness);
void CrossOver_uniform(bitset<20> *x1, bitset<20> *x2, bitset<20> *y1, bitset<20> *y2, int d);
void Reproduction(int N, int d, bitset<20> **P1, bitset<20> **P2, double pc);
void Mutation(int N, int d, bitset<20> **P2);
void Replacement(int N, int d, bitset<20> **P2, bitset<20> **P3, bitset<20> *Elit);
void GeneticAlgorithm(int N, int d, function fx, double xmin, double xmax, double *bestResult);

bitset<20> **RequestBitSetMatrix(int data_size1, int data_size2);
bitset<20> *RequestBitSetVector(int data_size);
void freeBitSetMatrix(bitset<20> **pointer);

#endif // GENETIC_H_INCLUDED
